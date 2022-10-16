///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/Logger.hpp"
#include "realpaver/Preprocessor.hpp"

namespace realpaver {

bool Preprocessor::allVarsRemoved() const
{
   return vvm_.empty();
}

bool Preprocessor::isFake(Variable v) const
{
   auto it = fake_.find(v);
   return it != fake_.end();
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
   
bool Preprocessor::apply(const Problem& src, Problem& dest)
{
   IntervalRegion reg = src.getDomains();
   return apply(src, reg, dest);
}

bool Preprocessor::apply(const Problem& src, IntervalRegion& reg, Problem& dest)
{
   ASSERT(src.nbVars() == reg.size(), "Preprocessing error");
   ASSERT(!src.isEmpty(), "Preprocessing error");
   ASSERT(dest.isEmpty(), "Preprocessing error");

   Objective obj = src.getObjective();

   LOG_MAIN("Preprocessing");

   // test empty domains
   for (size_t i=0; i<src.nbVars(); ++i)
   {
      Variable v        = src.varAt(i);
      Interval domain   = reg.get(v);

      if (domain.isEmpty())
      {
         LOG_MAIN("Empty domain of variable: " << v.getName());
         return false;
      }
   }

   // propagation
   bool ok = propagate(src, reg);
   if (!ok) return false;

   // satisfaction tests
   for (size_t i=0; i<src.nbCtrs(); ++i)
   {
      Constraint c = src.ctrAt(i);
      Proof proof = c.isSatisfied(reg);

      if (proof == Proof::Empty)
      {
         LOG_INTER("Constraint violated (normally does not arise): " << c);
         return false;
      }

      else if (proof == Proof::Inner)
      {
         LOG_INTER("Inactive constraint: " << c);
         nbc_ = nbc_ + 1;
      }
      
      else
      {
         active_.push_back(c);
      }
   }

   LOG_MAIN("Number of inactive constraints: " << nbc_);

   // rewrites the variables
   vvm_.clear();
   vim_.clear();
   fake_.clear();
   nbc_ = 0;

   for (size_t i=0; i<src.nbVars(); ++i)
   {
      Variable v        = src.varAt(i);
      Interval domain   = reg.get(v);
      bool isContinuous = v.isContinuous();
      Tolerance tol     = v.getTolerance();

      bool isFixed = isContinuous ? domain.isCanonical() :
                                    domain.isSingleton();

      bool isFake = !(occursInActiveConstraint(v) || obj.dependsOn(v));
   
      if (isFake)
      {
         LOG_INTER("Fixes and removes " << v.getName() << " := " << domain);
         fake_.insert(std::make_pair(v, domain));
      }
      else if (isFixed)
      {
         LOG_INTER("Fixes and removes " << v.getName() << " := " << domain);
         vim_.insert(std::make_pair(v, domain));
      }
      else
      {
         // creates a clone of the variable in the other problem
         Variable w = isContinuous ?
                           dest.addRealVar(domain, v.getName()) :
                           dest.addIntVar(domain, v.getName());
         w.setTolerance(tol);

         // new map entry
         vvm_.insert(std::make_pair(v, w));
      }
   }

   LOG_MAIN("Number of removed variables: " << vim_.size() + fake_.size());

   // rewrites the constraints
   for (Constraint input : active_)
   {
      ConstraintFixer fixer(&vvm_, &vim_);
      input.acceptVisitor(fixer);
      Constraint c = fixer.getConstraint();
      
      if (c.isConstant())
      {
         LOG_INTER("Constraint with no variable: " << c);
      }
      
      else dest.addCtr(c);
   }

   // checks the range of the objective function
   Interval dobj = obj.getTerm().eval(src.getDomains());
   if (dobj.isEmpty())
   {
      LOG_MAIN("Empty range of the objective function");
      return false;
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

   return true;
}

bool Preprocessor::propagate(const Problem& problem, IntervalRegion& reg)
{
   // AC1 propagation algorithm
   bool modified;
   do
   {
      modified = false;
      IntervalRegion save(reg);

      for (size_t i=0; i<problem.nbCtrs(); ++i)
      {
         Constraint c = problem.ctrAt(i);
         Proof proof = c.contract(reg);
         if (proof == Proof::Empty)
         {
            LOG_INTER("Constraint violated: " << c);
            return false;
         }
      }

      if (!save.equals(reg)) modified = true;
   }
   while (modified);
   return true;
}

bool Preprocessor::occursInActiveConstraint(const Variable& v) const
{
   for (Constraint c : active_)
      if (c.dependsOn(v)) return true;

   return false;
}

Scope Preprocessor::trueScope() const
{
   Scope sco;
   for (auto p : vim_) sco.insert(p.first);
   for (auto p : vvm_) sco.insert(p.first);
   return sco;
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

Scope Preprocessor::fakeScope() const
{
   Scope sco;
   for (auto p : fake_) sco.insert(p.first);
   return sco;   
}

} // namespace
