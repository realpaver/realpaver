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

Preprocessor::Preprocessor()
      : vvm_(),
        vim_(),
        active_(),
        nbc_(0),
        unfeasible_(false),
        timer_()
{}

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
   
void Preprocessor::apply(const Problem& src, Problem& dest)
{
   IntervalRegion reg = src.getDomains();
   apply(src, reg, dest);
}

void Preprocessor::apply(const Problem& src, IntervalRegion& reg, Problem& dest)
{
   timer_.reset();
   timer_.start();
   applyImpl(src, reg, dest);
   timer_.stop();
}

void Preprocessor::applyImpl(const Problem& src, IntervalRegion& reg,
                             Problem& dest)
{
   ASSERT(src.nbVars() == reg.size(), "Preprocessing error");
   ASSERT(!src.isEmpty(), "Preprocessing error");
   ASSERT(dest.isEmpty(), "Preprocessing error");

   Objective obj = src.getObjective();

   // resets this
   vvm_.clear();
   vim_.clear();
   active_.clear();
   nbc_ = 0;
   unfeasible_ = false;

   LOG_MAIN("Preprocessing");
   LOG_INTER("Region: " << reg);

   // test empty domains
   for (size_t i=0; i<src.nbVars(); ++i)
   {
      Variable v        = src.varAt(i);
      Interval domain   = reg.get(v);

      if (domain.isEmpty())
      {
         LOG_MAIN("Empty domain of variable: " << v.getName());
         unfeasible_ = true;
         return;
      }
   }

   // propagation
   bool ok = propagate(src, reg);
   if (!ok)
   {
      unfeasible_ = true;
      return;
   }

   LOG_INTER("Contracted region: " << reg);

   // satisfaction tests
   for (size_t i=0; i<src.nbCtrs(); ++i)
   {
      Constraint c = src.ctrAt(i);
      Proof proof = c.isSatisfied(reg);

      if (proof == Proof::Empty)
      {
         LOG_INTER("Constraint violated (normally does not arise): " << c);
         unfeasible_ = true;
         return;
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
         Variable w = isContinuous ?
                           dest.addRealVar(domain, v.getName()) :
                           dest.addIntVar(domain, v.getName());
         w.setTolerance(tol);

         // new map entry
         vvm_.insert(std::make_pair(v, w));
      }
   }

   LOG_MAIN("Number of removed variables: " << vim_.size());

   // rewrites the constraints
   for (Constraint input : active_)
   {
      ConstraintFixer fixer(&vvm_, &vim_, reg);
      input.acceptVisitor(fixer);
      Constraint c = fixer.getConstraint();
      
      if (c.isConstant())
      {
         LOG_INTER("Constraint with no variable: " << c);
         nbc_ = nbc_ + 1;
      }

      else dest.addCtr(c);
   }

   // checks the range of the objective function
   Interval dobj = obj.getTerm().eval(src.getDomains());
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
   
   // integer variables
   Scope sco = reg.scope();
   for (auto v : sco)
   {
      if (v.isDiscrete())
      {
         Interval rnd = round(reg.get(v));
         reg.set(v, rnd);

         if (rnd.isEmpty()) return false;
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
   return nbc_;
}

size_t Preprocessor::nbFixedVars() const
{
   return vim_.size();
}

IntervalRegion Preprocessor::fixedRegion() const
{
   ASSERT(vim_.size() > 0, "Fixed region required but no fixed variable");

   IntervalRegion reg(fixedScope());
   for (auto p : vim_)
      reg.set(p.first, p.second);

   return reg;
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
