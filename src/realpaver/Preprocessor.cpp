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
   auto it = fake_.find(v.getId());
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
   return apply(src, src.getDomains(), dest);
}

bool Preprocessor::apply(const Problem& src, const IntervalRegion& reg,
                         Problem& dest)
{
   ASSERT(src.nbVars() == reg.size(), "Preprocessing error");
   ASSERT(!src.isEmpty(), "Preprocessing error");
   ASSERT(dest.isEmpty(), "Preprocessing error");

   LOG_MAIN("Preprocessing");

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

      if (domain.isEmpty())
      {
         LOG_MAIN("Empty domain of variable: " << v.getName());
         return false;
      }

      if (src.isFakeVar(v))
      {
         LOG_INTER("Useless variable: " << v.getName());
         fake_.insert(v.getId());
         continue;
      }

      bool isFixed = isContinuous ? domain.isCanonical() :
                                    domain.isSingleton();

      if (isFixed)
      {
         LOG_INTER("Fixes " << v.getName() << " := " << domain);
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

   LOG_MAIN("Number of fixed variables: " << vim_.size());
   LOG_MAIN("Number of fake variables: " << fake_.size());

   // this region is useful if some variables are not fixed and in this case
   // Y is assigned to dest.getDomains(); otherwise it is useless and it is
   // assigned to src.getDomains() in order to prevent errors
   IntervalRegion Y = (vvm_.empty()) ? src.getDomains() : dest.getDomains();

   // rewrites the constraints
   for (size_t i=0; i<src.nbCtrs(); ++i)
   {
      ConstraintFixer fixer(&vvm_, &vim_);
      src.ctrAt(i).acceptVisitor(fixer);
      Constraint c = fixer.getConstraint();

      Proof proof = c.isSatisfied(Y);

      if (proof == Proof::Empty)
      {
         LOG_MAIN("Constraint violated: " << c);
         return false;
      }
      else if (proof == Proof::Inner)
      {
         LOG_INTER("Inactive constraint: " << c);
         nbc_ = nbc_ + 1;
      }
      else
      {
         dest.addCtr(c);
      }
   }

   LOG_MAIN("Number of inactive constraints: " << nbc_);

   Objective obj = src.getObjective();

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

Scope Preprocessor::trueScope() const
{
   Scope sco;

   for (auto p : vim_) sco.insert(p.first);
   for (auto p : vvm_) sco.insert(p.first);

   return sco;
}

} // namespace
