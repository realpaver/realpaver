///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalNewton.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

IntervalNewton::IntervalNewton(IntervalFunctionVector F)
      : Contractor(),
        F_(F),
        jac_(F.nbFuns(), F.nbVars()),
        val_(F.nbFuns()),
        y_(F.nbVars()),
        b_(F.nbFuns()),
        c_(F.scope()),
        gs_(nullptr),
        maxiter_(Param::GetIntParam("NEWTON_ITER_LIMIT")),
        xtol_(Param::GetTolParam("NEWTON_XTOL")),
        dtol_(Param::GetTolParam("NEWTON_DTOL"))
{
   ASSERT(F.nbVars() == F.nbFuns(),
          "Interval Newton defined with a non-square system");

   gs_ = new IntervalGaussSeidel();
}

IntervalNewton::~IntervalNewton()
{
   delete gs_;
}

Scope IntervalNewton::scope() const
{
   return F_.scope();
}

Proof IntervalNewton::contract(IntervalRegion& X)
{
   bool iter = true;
   Proof proof = Proof::Maybe;
   size_t nb_steps = 0;

   do
   {
      ++ nb_steps;

      F_.evalDiff(X, val_, jac_);

      if (!val_.containsZero())
      {
         proof = Proof::Empty;
         iter = false;
         continue;
      }

      X.midpointOnScope(scope(), c_);
      F_.pointEval(c_, val_);

      if (val_.isEmpty())
      {
         iter = false;
         continue;
      }

      makeY(X);      // y := X - c
      b_ = -val_;    // b := -F(c)

      Proof certif = gs_->contractPrecond(jac_, y_, b_);
            
      if (certif == Proof::Empty)
      {
         proof = Proof::Empty;
         iter = false;
         continue;
      }

      bool hasxtol, hasdtol;
      certif = reduceX(X, hasxtol, hasdtol);   // X := X inter (y + c)

      if (certif == Proof::Empty)
      {
         proof = Proof::Empty;
         iter = false;
         continue;
      }

      if (certif == Proof::Feasible)
         proof = Proof::Feasible;

      // checks the stopping criteria
      if (nb_steps > maxiter_ || hasxtol || hasdtol)
         iter = false;
   }
   while (iter);

   return proof;
}

void IntervalNewton::makeY(IntervalRegion& X)
{
   // y := X - c
   int i = 0;
   for (const auto& v : scope())
   {
      y_.set(i, X.get(v) - c_.get(v));
      i = i+1;
   }
}

Proof IntervalNewton::reduceX(IntervalRegion& X, bool& hasxtol, bool& hasdtol)
{
   int i = 0;
   Proof proof = Proof::Feasible;

   hasxtol = true;
   hasdtol = true;

   for (const auto& v : scope())
   {
      Interval dom = X.get(v),
               z = y_.get(i) + c_.get(v);
      
      if (dom.isDisjoint(z))
         return Proof::Empty;

      if (!dom.strictlyContains(z))
         proof = Proof::Maybe;

      Interval reduced = dom & z;

      if (!dtol_.haveDistTolerance(reduced, dom))
         hasdtol = false;

      if (!xtol_.hasTolerance(reduced))
         hasxtol = false;

      X.set(v, reduced);
      i = i+1;
   }
   return proof;
}

void IntervalNewton::print(std::ostream& os) const
{
   os << "Interval Newton";
}

} // namespace
