///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Preprocessor.hpp"

namespace realpaver {

Preprocessor::Preprocessor()
      : vvm_(),
        vim_(),
        box_(nullptr),
        inactive_(),
        active_(),
        unfeasible_(false),
        timer_()
{}

Preprocessor::~Preprocessor()
{
   if (box_!=nullptr) delete box_;
}

bool Preprocessor::allVarsFixed() const
{
   return vvm_.empty();
}

bool Preprocessor::hasFixedDomain(Variable v) const
{
   auto it = vim_.find(v);
   return it != vim_.end();
}

Interval Preprocessor::getFixedDomain(Variable v) const
{
   ASSERT(hasFixedDomain(v), "Domain of " << v.getName() << " is not fixed");

   auto it = vim_.find(v);
   return it->second;
}

Variable Preprocessor::srcToDestVar(Variable v) const
{
   ASSERT(!hasFixedDomain(v), "Domain of " << v.getName() << " is fixed");

   auto it = vvm_.find(v);
   return it->second;
}

Scope Preprocessor::destScope() const
{
   Scope sco;
   for (auto p : vvm_)
      sco.insert(p.second);

   return sco;
}

IntervalBox Preprocessor::destRegion() const
{
   IntervalBox box(destScope());
   for (auto p : vvm_)
      box.set(p.second, box_->get(p.first));

   return box;
}

void Preprocessor::apply(const Problem& src, Problem& dest)
{   
   ASSERT(!src.isEmpty(), "Preprocessing error");
   ASSERT(dest.isEmpty(), "Preprocessing error");

   // resets this
   vvm_.clear();
   vim_.clear();
   inactive_.clear();
   active_.clear();
   unfeasible_ = false;

   // makes the interval box used for propagation
   if (box_!=nullptr) delete box_;
   box_ = new IntervalBox(src.scope());

   LOG_MAIN("Preprocessing");
   LOG_INTER("Region: " << (*box_));

   timer_.reset();
   timer_.start();
   applyImpl(src, dest);
   timer_.stop();
}

void Preprocessor::applyImpl(const Problem& src, Problem& dest)
{
   Objective obj = src.getObjective();

   // test empty domains
   for (size_t i=0; i<src.nbVars(); ++i)
   {
      Variable v        = src.varAt(i);
      Interval domain   = box_->get(v);

      if (domain.isEmpty())
      {
         LOG_MAIN("Empty domain of variable: " << v.getName());
         unfeasible_ = true;
         return;
      }
   }

   // propagation
   bool ok = propagate(src, *box_);
   if (!ok)
   {
      unfeasible_ = true;
      return;
   }

   LOG_INTER("Contracted box: " << (*box_));

   // satisfaction tests
   for (size_t i=0; i<src.nbCtrs(); ++i)
   {
      Constraint c = src.ctrAt(i);
      Proof proof = c.isSatisfied(*box_);

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
   for (size_t i=0; i<src.nbVars(); ++i)
   {
      Variable v        = src.varAt(i);
      Interval domain   = box_->get(v);
      bool isReal       = v.isReal();

      bool isFixed = isReal ? domain.isCanonical() : domain.isSingleton();
      bool isFake = !(occursInActiveConstraint(v) || obj.dependsOn(v));
   
      if (isFake)
      {
         LOG_INTER("Fixes and removes " << v.getName() << " := " << domain);
         vim_.insert(std::make_pair(v, domain));
      }
      else if (isFixed)
      {
         LOG_INTER("Fixes and removes " << v.getName() << " := " << domain);
         vim_.insert(std::make_pair(v, domain));
      }
      else
      {
         // creates a clone of the variable in the other problem
         Variable w = dest.addClonedVar(v);

         // assigns the reduced domain to it
         std::unique_ptr<Domain> wdom(v.getDomain()->clone());
         wdom->contract(domain);
         w.setDomain(std::move(wdom));

         // new map entry
         vvm_.insert(std::make_pair(v, w));
      }
   }

   LOG_MAIN("Number of removed variables: " << vim_.size());

   // rewrites the constraints
   for (Constraint input : active_)
   {
      ConstraintFixer fixer(&vvm_, &vim_, *box_);
      input.acceptVisitor(fixer);
      Constraint c = fixer.getConstraint();

      if (c.isConstant())
      {
         LOG_INTER("Constraint with no variable: " << c);
         inactive_.push_back(input);
      }

      else dest.addCtr(c);
   }

   // checks the range of the objective function
   Interval dobj = obj.getTerm().eval(*box_);
   if (dobj.isEmpty())
   {
      LOG_MAIN("Empty range of the objective function");
      unfeasible_ = true;
      return;
   }

   // simplifies the objective function
   TermFixer fixer(&vvm_, &vim_);
   obj.getTerm().acceptVisitor(fixer);

   if ((!obj.isConstant()) && fixer.getTerm().isConstant())
   {
      LOG_INTER("Fixed objective function: " << fixer.getTerm());
   }
   else
   {
      if (obj.isMinimization())
         dest.addObjective(MIN(fixer.getTerm()));

      else
         dest.addObjective(MAX(fixer.getTerm()));
   }
}

bool Preprocessor::propagate(const Problem& problem, IntervalBox& box)
{
   // AC1 propagation algorithm
   bool modified;
   int nbsteps = Param::GetIntParam("PROPAGATION_ITER_LIMIT");

   do
   {
      modified = false;
      --nbsteps;
      IntervalBox save(box);

      for (size_t i=0; i<problem.nbCtrs(); ++i)
      {
         Constraint c = problem.ctrAt(i);
         Proof proof = c.contract(box);
         if (proof == Proof::Empty)
         {
            LOG_INTER("Constraint violated: " << c);
            return false;
         }
      }

      if (!save.equals(box)) modified = true;
   }
   while (modified && nbsteps>0);

   // variables with disconnected domains
   Scope sco = box.scope();
   for (auto v : sco)
   {
      if (!v.getDomain()->isConnected())
      {
         Interval y = box.get(v);
         v.getDomain()->contractInterval(y);
         box.set(v, y);

         if (y.isEmpty()) return false;
      }
   }

   return true;
}

bool Preprocessor::occursInActiveConstraint(const Variable& v) const
{
   for (Constraint c : active_)
      if (c.dependsOn(v)) return true;

   return false;
}

Scope Preprocessor::fixedScope() const
{
   Scope sco;
   for (auto p : vim_) sco.insert(p.first);
   return sco;
}

Scope Preprocessor::unfixedScope() const
{
   Scope sco;
   for (auto p : vvm_) sco.insert(p.first);
   return sco;
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

size_t Preprocessor::nbFixedVars() const
{
   return vim_.size();
}

size_t Preprocessor::nbUnfixedVars() const
{
   return vvm_.size();
}

Variable Preprocessor::getFixedVar(size_t i) const
{
   ASSERT(i < nbFixedVars(),
          "Bad access to a fixed variable in a preprocessor");

   auto it = vim_.begin();
   std::advance(it, i);
   return (*it).first;
}

Variable Preprocessor::getUnfixedVar(size_t i) const
{
   ASSERT(i < nbUnfixedVars(),
          "Bad access to a-n unfixed variable in a preprocessor");
   
   auto it = vvm_.begin();
   std::advance(it, i);
   return (*it).first;
}

IntervalBox Preprocessor::fixedRegion() const
{
   ASSERT(vim_.size() > 0, "Fixed region required but no fixed variable");

   IntervalBox box(fixedScope());
   for (auto p : vim_)
      box.set(p.first, p.second);

   return box;
}

bool Preprocessor::isUnfeasible() const
{
   return unfeasible_;
}

bool Preprocessor::isSolved() const
{
   return unfeasible_ || allVarsFixed();
}

double Preprocessor::elapsedTime() const
{
   return timer_.elapsedTime();
}

} // namespace
