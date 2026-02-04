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

#include "Variable.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintFixer.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Preprocessor.hpp"
#include "realpaver/TermFixer.hpp"
#include <memory>

namespace realpaver {

Preprocessor::Preprocessor(SharedProblem src)
    : src_(src)
    , dest_(nullptr)
    , bridge_(nullptr)
    , inactive_()
    , active_()
    , infeasible_(false)
    , timer_()
    , on_(false)
{
   REALPAVER_ASSERT(src != nullptr, "Preprocessing error: empty problem");
   REALPAVER_ASSERT(!src->isEmpty(), "Preprocessing error: empty problem");
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
   if (!(on_ = on))
   {
      dest_ = src_;
      return;
   }

   // resets this
   inactive_.clear();
   active_.clear();
   infeasible_ = false;

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
         infeasible_ = true;
         return;
      }
   }

   // propagation
   bool ok = propagate(box);
   if (!ok)
   {
      infeasible_ = true;
      return;
   }

   // evaluates the objective function and assigns the domain of the corresponding
   // variable since this is not done by the propagation algorithm (there is no constraint
   // of the form z = f(...))
   if (src_->hasObjective())
   {
      Variable z = obj.getVar();
      Term f = obj.getTerm();
      Interval e = f.eval(box)->intervalHull();
      box.get(z)->contract(e);

      if (box.get(z)->isEmpty())
      {
         LOG_MAIN("Empty range of the objective function");
         infeasible_ = true;
         return;
      }
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
         infeasible_ = true;
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
      if (v.getStatus() == VariableStatus::Objective)
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
   if (src_->hasObjective() && (!bridge_->isFixed(obj.getVar())))
   {
      Variable z = obj.getVar();
      Term f = obj.getTerm();
      std::unique_ptr<Domain> dom(box.get(z)->clone());

      Variable zdest = bridge_->srcToDestVar(z);
      zdest.setDomain(std::move(dom));

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
         infeasible_ = true;
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
   REALPAVER_ASSERT(i < inactive_.size(), "Bad access to an inactive constraint");
   return inactive_[i];
}

bool Preprocessor::isUnfeasible() const
{
   return infeasible_;
}

bool Preprocessor::isSolvedOptimal() const
{
   if (!on_)
      return false;

   if (infeasible_)
      return false;

   if (src_->hasObjective() && !dest_->hasObjective())
      return true;

   return false;
}

bool Preprocessor::isSolved() const
{
   if (!on_)
      return false;

   if (infeasible_)
      return true;

   if (bridge_ != nullptr && bridge_->allVarsFixed())
      return true;

   if (src_->hasObjective() && !dest_->hasObjective())
      return true;

   return false;
}

double Preprocessor::elapsedTime() const
{
   return timer_.elapsedTime();
}

bool Preprocessor::allVarsFixed() const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->allVarsFixed();
}

Variable Preprocessor::srcToDestVar(const Variable &v) const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->srcToDestVar(v);
}

size_t Preprocessor::nbFixedVars() const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->nbFixedVars();
}

size_t Preprocessor::nbUnfixedVars() const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->nbUnfixedVars();
}

Variable Preprocessor::getFixedVar(size_t i) const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->getFixedVar(i);
}

Variable Preprocessor::getUnfixedVar(size_t i) const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->getUnfixedVar(i);
}

Interval Preprocessor::getFixedDomain(const Variable &v) const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->getFixedDomain(v);
}

bool Preprocessor::isFixed(const Variable &v) const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->isFixed(v);
}

bool Preprocessor::isUnfixed(const Variable &v) const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->isUnfixed(v);
}

DomainBox Preprocessor::fixedRegion() const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->fixedRegion();
}

DomainBox Preprocessor::destRegion() const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->destRegion();
}

Scope Preprocessor::destScope() const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->destScope();
}

Scope Preprocessor::fixedScope() const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->fixedScope();
}

Scope Preprocessor::unfixedScope() const
{
   REALPAVER_ASSERT(on_, "Preprocessor inactive");
   return bridge_->unfixedScope();
}

bool Preprocessor::isOn() const
{
   return on_;
}

void Preprocessor::print(std::ostream &os) const
{
   if (!on_)
   {
      os << "preprocessor inactive" << std::endl;
      return;
   }

   os << "Status: ";
   if (infeasible_)
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
