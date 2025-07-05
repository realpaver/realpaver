/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   Preprocessor.cpp
 * @brief  Preprocessor of problems
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintFixer.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Preprocessor.hpp"
#include "realpaver/TermFixer.hpp"

namespace realpaver {

Preprocessor::Preprocessor(SharedProblem src)
    : src_(src)
    , dest_(nullptr)
    , bridge_(nullptr)
    , inactive_()
    , active_()
    , unfeasible_(false)
    , timer_()
{
   ASSERT(src != nullptr, "Preprocessing error: empty problem");
   ASSERT(!src->isEmpty(), "Preprocessing error: empty problem");
}

SharedProblem Preprocessor::srcProblem() const
{
   return src_;
}

SharedProblem Preprocessor::destProblem() const
{
   return dest_;
}

void Preprocessor::apply(bool on)
{
   if (!on)
   {
      dest_ = src_;
      return;
   }

   // resets this
   inactive_.clear();
   active_.clear();
   unfeasible_ = false;

   // makes the new problem and the bridge
   dest_ = std::make_shared<Problem>();
   bridge_ = std::make_shared<ProblemBridge>(src_, dest_);

   LOG_MAIN("Preprocessing");
   LOG_INTER("Box: " << (bridge_->srcRegion()));

   timer_.reset();
   timer_.start();
   applyImpl();
   timer_.stop();
}

void Preprocessor::applyImpl()
{
   DomainBox &box = bridge_->srcRegion();
   Objective obj = src_->getObjective();

   // test empty domains
   for (size_t i = 0; i < src_->nbVars(); ++i)
   {
      Variable v = src_->varAt(i);
      Domain *domain = box.get(v);

      if (domain->isEmpty())
      {
         LOG_MAIN("Empty domain of variable: " << v.getName());
         unfeasible_ = true;
         return;
      }
   }

   // propagation
   bool ok = propagate(box);
   if (!ok)
   {
      unfeasible_ = true;
      return;
   }

   LOG_INTER("Contracted box: " << (box));

   // satisfaction tests
   for (size_t i = 0; i < src_->nbCtrs(); ++i)
   {
      Constraint c = src_->ctrAt(i);
      Proof proof = c.isSatisfied(box);

      if (proof == Proof::Empty)
      {
         LOG_INTER("Constraint violated (normally does not arise): " << c);
         unfeasible_ = true;
         return;
      }

      else if (proof == Proof::Inner || c.isBoundConstraint())
      {
         LOG_INTER("Inactive constraint: " << c);
         inactive_.push_back(c);
      }

      else
      {
         active_.push_back(c);
      }
   }

   LOG_MAIN("Number of inactive constraints: " << inactive_.size());

   // rewrites the variables
   for (size_t i = 0; i < src_->nbVars(); ++i)
   {
      Variable v = src_->varAt(i);
      Domain *domain = box.get(v);

      bool isFixed = domain->isCanonical();
      bool isUsed = false;
      if (occursInActiveConstraint(v))
         isUsed = true;
      if (src_->hasObjective() && obj.dependsOn(v))
         isUsed = true;
      if (occursInAlias(v))
         isUsed = true;

      if (isFixed)
      {
         LOG_INTER("Variable fixed: " << v.getName() << " := " << (*domain));
         bridge_->insertFixed(v, *domain);
      }
      else if (!isUsed)
      {
         LOG_INTER("Variable unused: " << v.getName() << " := " << (*domain));
         bridge_->insertFixed(v, *domain);
      }
      else
      {
         LOG_INTER("Variable used: " << v.getName());
         bridge_->insertVar(v, *domain);
      }
   }

   LOG_MAIN("Number of removed variables: " << bridge_->nbFixedVars());

   // rewrites the objective function; it must be done before processing the constraints
   // because the status of the objective variable can be changed from unfixed to fixed
   if (src_->hasObjective())
   {
      Variable z = obj.getVar();
      Term f = obj.getTerm();
      Interval e = f.eval(box)->intervalHull();
      Domain *dom = z.getDomain();
      dom->contract(e);

      if (dom->isEmpty())
      {
         LOG_MAIN("Empty range of the objective function");
         unfeasible_ = true;
         return;
      }

      // simplifies the objective function
      TermFixer fixer(bridge_);
      f.acceptVisitor(fixer);

      if (fixer.getTerm().isConstant())
      {
         LOG_INTER("Fixed objective function: " << fixer.getTerm());
         if (bridge_->isUnfixed(z))
         {
            bridge_->removeUnfixedVar(z);
            bridge_->insertFixed(z, *dom);
         }
      }
      else
      {
         Variable dz = bridge_->srcToDestVar(z);
         if (obj.isMinimization())
            dest_->addObjective(minimize(dz, fixer.getTerm()));
         else
            dest_->addObjective(maximize(dz, fixer.getTerm()));
      }
   }

   // rewrites the aliases
   for (size_t i = 0; i < src_->nbAliases(); ++i)
   {
      Alias als = src_->aliasAt(i);
      Interval e = als.term().eval(box)->intervalHull();
      if (e.isEmpty())
      {
         unfeasible_ = true;
         LOG_INTER("Evaluation of alias " << als.name() << " -> empty");
         return;
      }

      TermFixer fixer(bridge_);
      als.term().acceptVisitor(fixer);
      Term rt = fixer.getTerm();
      if (rt.isConstant())
      {
         LOG_INTER("Fixed alias: " << als.name() << " = " << rt);
      }
      else
      {
         Alias ra(als.name(), rt);
         dest_->addAlias(ra);
      }
   }

   // rewrites the constraints
   for (Constraint input : active_)
   {
      ConstraintFixer fixer(bridge_);
      input.acceptVisitor(fixer);
      Constraint rc = fixer.getConstraint();

      if (rc.isConstant())
      {
         LOG_INTER("Fixed constraint: " << rc);
         inactive_.push_back(input);
      }
      else
         dest_->addCtr(rc);
   }
}

bool Preprocessor::propagate(DomainBox &box)
{
   // AC1 propagation algorithm
   bool modified;

   do
   {
      modified = false;
      DomainBox save(box);

      for (size_t i = 0; i < src_->nbCtrs(); ++i)
      {
         Constraint c = src_->ctrAt(i);
         Proof proof = c.contract(box);
         if (proof == Proof::Empty)
         {
            LOG_INTER("Constraint violated: " << c);
            return false;
         }
      }

      if (!save.equals(box))
         modified = true;
   } while (modified);

   return true;
}

bool Preprocessor::occursInActiveConstraint(const Variable &v) const
{
   for (Constraint c : active_)
      if (c.dependsOn(v))
         return true;
   return false;
}

bool Preprocessor::occursInAlias(const Variable &v) const
{
   for (size_t i = 0; i < src_->nbAliases(); ++i)
   {
      if (src_->aliasAt(i).term().dependsOn(v))
         return true;
   }
   return false;
}

size_t Preprocessor::nbInactiveCtrs() const
{
   return inactive_.size();
}

Constraint Preprocessor::getInactiveCtr(size_t i) const
{
   ASSERT(i < inactive_.size(), "Bad access to an inactive constraint");
   return inactive_[i];
}

bool Preprocessor::isUnfeasible() const
{
   return unfeasible_;
}

bool Preprocessor::isSolved() const
{
   return unfeasible_ || bridge_->allVarsFixed();
}

double Preprocessor::elapsedTime() const
{
   return timer_.elapsedTime();
}

bool Preprocessor::allVarsFixed() const
{
   return bridge_->allVarsFixed();
}

Variable Preprocessor::srcToDestVar(const Variable &v) const
{
   return bridge_->srcToDestVar(v);
}

size_t Preprocessor::nbFixedVars() const
{
   return bridge_->nbFixedVars();
}

size_t Preprocessor::nbUnfixedVars() const
{
   return bridge_->nbUnfixedVars();
}

Variable Preprocessor::getFixedVar(size_t i) const
{
   return bridge_->getFixedVar(i);
}

Variable Preprocessor::getUnfixedVar(size_t i) const
{
   return bridge_->getUnfixedVar(i);
}

Interval Preprocessor::getFixedDomain(const Variable &v) const
{
   return bridge_->getFixedDomain(v);
}

bool Preprocessor::isFixed(const Variable &v) const
{
   return bridge_->isFixed(v);
}

bool Preprocessor::isUnfixed(const Variable &v) const
{
   return bridge_->isUnfixed(v);
}

DomainBox Preprocessor::fixedRegion() const
{
   return bridge_->fixedRegion();
}

DomainBox Preprocessor::destRegion() const
{
   return bridge_->destRegion();
}

Scope Preprocessor::destScope() const
{
   return bridge_->destScope();
}

Scope Preprocessor::fixedScope() const
{
   return bridge_->fixedScope();
}

Scope Preprocessor::unfixedScope() const
{
   return bridge_->unfixedScope();
}

void Preprocessor::print(std::ostream &os) const
{
   os << "Status: ";
   if (unfeasible_)
   {
      os << "infeasible" << std::endl;
      return;
   }
   else
      os << "possibly feasible" << std::endl;

   os << "Fixed variables: ";
   for (size_t i = 0; i < nbFixedVars(); ++i)
   {
      if (i > 0)
         os << ", ";
      os << getFixedVar(i).getName() << ":" << getFixedDomain(getFixedVar(i));
   }
   os << std::endl;

   os << "Unfixed variables: ";
   for (size_t i = 0; i < nbUnfixedVars(); ++i)
   {
      if (i > 0)
         os << ", ";
      os << getUnfixedVar(i).getName();
   }
   os << std::endl;

   os << "Inactive constraints:" << std::endl;
   for (const auto &c : inactive_)
      os << "   " << c << std::endl;

   os << "Active constraints:" << std::endl;
   for (const auto &c : active_)
      os << "   " << c << std::endl;
}

} // namespace realpaver
