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

bool Preprocessor::areAllVarFixed() const
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

size_t Preprocessor::getNbVarFixed() const
{
   return nbv_;
}

size_t Preprocessor::getNbCtrRemoved() const
{
   return nbc_;
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

   LOG_COMPONENT("-- Preprocessing of problem --");

   vvm_.clear();
   vim_.clear();
   nbv_ = nbc_ = 0;

   for (size_t i=0; i<src.nbVars(); ++i)
   {
      Variable v        = src.varAt(i);
      Interval domain   = reg.get(v);
      bool isContinuous = v.isContinuous();
      Tolerance tol     = v.getTolerance();

      if (domain.isEmpty())
      {
         LOG_COMPONENT("   empty variable domain: " << v.getName());
         return false;
      }

      bool isFixed = isContinuous ? domain.isCanonical() :
                                    domain.isSingleton();

      if (isFixed)
      {
         LOG_COMPONENT("   replaces " << v.getName() << " by " << domain);
         vim_.insert(std::make_pair(v, domain));
         nbv_ = nbv_ + 1;
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

   IntervalRegion Y = dest.getDomains();

   // rewrites the constraints
   for (size_t i=0; i<src.nbCtrs(); ++i)
   {
      ConstraintFixer fixer(&vvm_, &vim_);
      src.ctrAt(i).acceptVisitor(fixer);
      Constraint c = fixer.getConstraint();

      Proof proof = c.isSatisfied(Y);

      if (proof == Proof::Empty)
      {
         LOG_COMPONENT("   constraint violated " << c);
         return false;
      }
      else if (proof == Proof::Inner)
      {
         LOG_COMPONENT("   inactive constraint " << c);
         nbc_ = nbc_ + 1;
      }
      else
      {
         dest.addCtr(c);
      }
   }

   Objective obj = src.getObjective();

   // checks the range of the objective function
   Interval dobj = obj.getTerm().eval(src.getDomains());
   if (dobj.isEmpty()) return false;

   // simplifies the objective function
   TermFixer fixer(&vvm_, &vim_);
   obj.getTerm().acceptVisitor(fixer);

   if ((!obj.isConstant()) && fixer.getTerm().isConstant())
   {
      LOG_COMPONENT("   fixed objective " << fixer.getTerm());
   }
   else
   {
      if (obj.isMinimization())
         dest.addObjective(minimize(fixer.getTerm()));

      else
         dest.addObjective(maximize(fixer.getTerm()));
   }

   // detects fake variables
   for (size_t i=0; i<dest.nbVars(); ++i)
   {
      Variable v = dest.varAt(i);
      if (dest.isFakeVar(v))
         LOG_COMPONENT("   unconstrained variable " << v.getName());
   }

   return true;
}

} // namespace
