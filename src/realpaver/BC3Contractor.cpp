///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <stack>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/BC3Contractor.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

BC3Contractor::BC3Contractor(Dag* dag, size_t i, Variable v)
      : f_(dag, i, v),
        peeler_(Param::getDblParam("BC3_PEEL_FACTOR")),
        maxiter_(Param::getIntParam("BC3_ITER_LIMIT"))
{
   newton_ = new IntervalNewton();
}

BC3Contractor::~BC3Contractor()
{
   delete newton_;
}

double BC3Contractor::getPeelFactor() const
{
   return peeler_.getFactor();
}

void BC3Contractor::setPeelFactor(double f)
{
   peeler_.setFactor(f);
}

size_t BC3Contractor::getMaxIter() const
{
   return maxiter_;
}

void BC3Contractor::setMaxIter(size_t val)
{
   maxiter_ = val;
}

IntervalNewton* BC3Contractor::getNewton() const
{
   return newton_;
}

Proof BC3Contractor::shrinkLeft(const Interval& x, Interval& res)
{
   return shrink(x, res, splitLeft, peelLeft);
}

Proof BC3Contractor::shrinkRight(const Interval& x, Interval& res)
{
   return shrink(x, res, splitRight, peelRight);   
}

Proof BC3Contractor::isConsistent(const Interval& x)
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

Scope BC3Contractor::scope() const
{
   return f_.getFun()->scope();
}

bool BC3Contractor::dependsOn(const Bitset& bs) const
{
   return f_.dependsOn(bs);
}

bool BC3Contractor::splitLeft(const Interval& x, Interval& x1, Interval& x2)
{
   double c = x.midpoint();
   x1 = Interval(c,x.right());
   x2 = Interval(x.left(), c);
   return x.strictlyContains(c);
}

bool BC3Contractor::splitRight(const Interval& x, Interval& x1, Interval& x2)
{
   double c = x.midpoint();
   x1 = Interval(x.left(), c);
   x2 = Interval(c,x.right());
   return x.strictlyContains(c);
}

void BC3Contractor::peelLeft(const Interval& x, IntervalPeeler& peeler,
                             Interval& b, Interval& r)
{
   b = peeler.peelLeft(x);
   r = Interval(b.right(), x.right());
}

void BC3Contractor::peelRight(const Interval& x, IntervalPeeler& peeler,
                              Interval& b, Interval& r)
{
   b = peeler.peelRight(x);
   r = Interval(x.left(), b.left());
}

Proof BC3Contractor::shrink(const Interval& x, Interval& res,
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

Proof BC3Contractor::contract(IntervalRegion& reg)
{
   Interval lsol, rsol;
   Proof proof, certif;

   Variable v = f_.getVar();
   const Interval& img = f_.getFun()->getImage();

   // first interval evaluation that also thickens the function
   Interval e = f_.update(reg);

   // consistency checking
   if (e.isEmpty())
      return Proof::Empty;

   if (!e.overlaps(img))
      return Proof::Empty;

   if (img.contains(e))
      return Proof::Inner;

   // shrinks the left bound
   proof = shrinkLeft(reg.get(v), lsol);

   if (proof == Proof::Empty)
      return Proof::Empty;

   // shrinks the right bound
   Interval y(lsol.left(), reg.get(v).right());
   certif = shrinkRight(y, rsol);

   // assigns the contracted domain in V
   reg.set(v, lsol | rsol);

   return std::max(proof,certif);
}

void BC3Contractor::print(std::ostream& os) const
{
   os << "BC3 contractor #" << f_.getFun()->index();
}

} // namespace
