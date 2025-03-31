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
 * @file   ContractorBC3Revise.cpp
 * @brief  BC3Revise contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/ContractorBC3Revise.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Param.hpp"
#include <stack>

namespace realpaver {

ContractorBC3Revise::ContractorBC3Revise(SharedDag dag, size_t i, Variable v)
    : f_(dag, i, v)
    , peeler_(Param::GetDblParam("BC3_PEEL_FACTOR"))
    , maxiter_(Param::GetIntParam("BC3_ITER_LIMIT"))
{
   newton_ = new IntervalNewtonUni();
}

ContractorBC3Revise::~ContractorBC3Revise()
{
   delete newton_;
}

double ContractorBC3Revise::getPeelFactor() const
{
   return peeler_.getFactor();
}

void ContractorBC3Revise::setPeelFactor(double f)
{
   peeler_.setFactor(f);
}

size_t ContractorBC3Revise::getMaxIter() const
{
   return maxiter_;
}

void ContractorBC3Revise::setMaxIter(size_t val)
{
   maxiter_ = val;
}

IntervalNewtonUni *ContractorBC3Revise::getNewton() const
{
   return newton_;
}

Proof ContractorBC3Revise::shrinkLeft(const Interval &x, Interval &res)
{
   return shrink(x, res, splitLeft, peelLeft);
}

Proof ContractorBC3Revise::shrinkRight(const Interval &x, Interval &res)
{
   return shrink(x, res, splitRight, peelRight);
}

Proof ContractorBC3Revise::isConsistent(const Interval &x)
{
   Interval e = f_.eval(x);
   const Interval &image = f_.getFun()->getImage();

   if (e.isEmpty())
      return Proof::Empty;

   else if (!image.overlaps(e))
      return Proof::Empty;

   else if (image.contains(e))
      return Proof::Inner;

   else
      return Proof::Maybe;
}

Scope ContractorBC3Revise::scope() const
{
   return f_.getFun()->scope();
}

bool ContractorBC3Revise::splitLeft(const Interval &x, Interval &x1, Interval &x2)
{
   double c = x.midpoint();
   x1 = Interval(c, x.right());
   x2 = Interval(x.left(), c);
   return x.strictlyContains(c);
}

bool ContractorBC3Revise::splitRight(const Interval &x, Interval &x1, Interval &x2)
{
   double c = x.midpoint();
   x1 = Interval(x.left(), c);
   x2 = Interval(c, x.right());
   return x.strictlyContains(c);
}

void ContractorBC3Revise::peelLeft(const Interval &x, IntervalPeeler &peeler, Interval &b,
                                   Interval &r)
{
   b = peeler.peelLeft(x);
   r = Interval(b.right(), x.right());
}

void ContractorBC3Revise::peelRight(const Interval &x, IntervalPeeler &peeler,
                                    Interval &b, Interval &r)
{
   b = peeler.peelRight(x);
   r = Interval(x.left(), b.left());
}

Proof ContractorBC3Revise::shrink(const Interval &x, Interval &res, SplitFun split_fun,
                                  PeelFun peel_fun)
{
   std::stack<Interval> stak;
   Interval b, z, z1, z2;
   Proof proof;
   size_t nbiter = 0;

   stak.push(x);
   while (!stak.empty())
   {
      Interval y(stak.top());
      stak.pop();

      if (++nbiter > maxiter_)
      {
         res = y;
         return Proof::Maybe;
      }

      // is the bound of y consistent ?
      peel_fun(y, peeler_, b, z);
      proof = isConsistent(b);

      if (proof != Proof::Empty)
      {
         res = b;
         return proof;
      }
      else
      {
         proof = newton_->contract(f_, z);

         if (proof == Proof::Feasible)
         {
            res = z;
            return proof;
         }
         else if (proof != Proof::Empty)
         {
            if (split_fun(z, z1, z2))
            {
               stak.push(z1);
               stak.push(z2);
            }
            else
            {
               res = z;
               return Proof::Maybe;
            }
         }
      }
   }

   res = Interval::emptyset();
   return Proof::Empty;
}

Proof ContractorBC3Revise::contract(IntervalBox &B)
{
   Interval lsol, rsol;
   Proof proof, certif;

   Variable v = f_.getVar();
   Interval img = f_.getFun()->getImage();

   // thickens the function
   f_.update(B);

   // evaluates the function
   Interval e = f_.eval(B.get(v));

   // consistency checking
   if (e.isEmpty())
      return Proof::Empty;

   if (!e.overlaps(img))
      return Proof::Empty;

   if (img.contains(e))
      return Proof::Inner;

   // shrinks the left bound
   proof = shrinkLeft(B.get(v), lsol);

   if (proof == Proof::Empty)
      return Proof::Empty;

   // shrinks the right bound
   Interval y(lsol.left(), B.get(v).right());
   certif = shrinkRight(y, rsol);

   // assigns the contracted domain in V
   B.set(v, lsol | rsol);

   return std::max(proof, certif);
}

void ContractorBC3Revise::print(std::ostream &os) const
{
   os << "BC3Revise contractor #" << f_.getFun()->index();
}

} // namespace realpaver
