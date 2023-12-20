///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <stack>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorBC3.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

ContractorBC3::ContractorBC3(SharedDag dag, size_t i, Variable v)
      : f_(dag, i, v),
        peeler_(Param::GetDblParam("BC3_PEEL_FACTOR")),
        maxiter_(Param::GetIntParam("BC3_ITER_LIMIT"))
{
   newton_ = new UniIntervalNewton();
}

ContractorBC3::~ContractorBC3()
{
   delete newton_;
}

double ContractorBC3::getPeelFactor() const
{
   return peeler_.getFactor();
}

void ContractorBC3::setPeelFactor(double f)
{
   peeler_.setFactor(f);
}

size_t ContractorBC3::getMaxIter() const
{
   return maxiter_;
}

void ContractorBC3::setMaxIter(size_t val)
{
   maxiter_ = val;
}

UniIntervalNewton* ContractorBC3::getNewton() const
{
   return newton_;
}

Proof ContractorBC3::shrinkLeft(const Interval& x, Interval& res)
{
   return shrink(x, res, splitLeft, peelLeft);
}

Proof ContractorBC3::shrinkRight(const Interval& x, Interval& res)
{
   return shrink(x, res, splitRight, peelRight);   
}

Proof ContractorBC3::isConsistent(const Interval& x)
{
   Interval e = f_.eval(x);
   const Interval& image = f_.getFun()->getImage();

   if (e.isEmpty())
      return Proof::Empty;

   else if (!image.overlaps(e))
      return Proof::Empty;

   else if (image.contains(e))
      return Proof::Inner;

   else
      return Proof::Maybe;
}

Scope ContractorBC3::scope() const
{
   return f_.getFun()->scope();
}

bool ContractorBC3::splitLeft(const Interval& x, Interval& x1, Interval& x2)
{
   double c = x.midpoint();
   x1 = Interval(c,x.right());
   x2 = Interval(x.left(), c);
   return x.strictlyContains(c);
}

bool ContractorBC3::splitRight(const Interval& x, Interval& x1, Interval& x2)
{
   double c = x.midpoint();
   x1 = Interval(x.left(), c);
   x2 = Interval(c,x.right());
   return x.strictlyContains(c);
}

void ContractorBC3::peelLeft(const Interval& x, IntervalPeeler& peeler,
                             Interval& b, Interval& r)
{
   b = peeler.peelLeft(x);
   r = Interval(b.right(), x.right());
}

void ContractorBC3::peelRight(const Interval& x, IntervalPeeler& peeler,
                              Interval& b, Interval& r)
{
   b = peeler.peelRight(x);
   r = Interval(x.left(), b.left());
}

Proof ContractorBC3::shrink(const Interval& x, Interval& res,
                            SplitFun split_fun, PeelFun peel_fun)
{
   std::stack<Interval> stak;
   Interval b, z, z1, z2;
   Proof proof;
   size_t nbiter = 0;

   stak.push(x);
   while (!stak.empty())
   {
      Interval y( stak.top() );
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
            if (split_fun(z,z1,z2))
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

Proof ContractorBC3::contract(IntervalBox& box)
{
   Interval lsol, rsol;
   Proof proof, certif;

   Variable v = f_.getVar();
   Interval img = f_.getFun()->getImage();

   // first interval evaluation that also thickens the function
   Interval e = f_.update(box);

   // consistency checking
   if (e.isEmpty())
      return Proof::Empty;

   if (!e.overlaps(img))
      return Proof::Empty;

   if (img.contains(e))
      return Proof::Inner;

   // shrinks the left bound
   proof = shrinkLeft(box.get(v), lsol);

   if (proof == Proof::Empty)
      return Proof::Empty;

   // shrinks the right bound
   Interval y(lsol.left(), box.get(v).right());
   certif = shrinkRight(y, rsol);

   // assigns the contracted domain in V
   box.set(v, lsol | rsol);

   return std::max(proof,certif);
}

void ContractorBC3::print(std::ostream& os) const
{
   os << "BC3 contractor #" << f_.getFun()->index();
}

} // namespace
