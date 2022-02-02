// This file is part of Realpaver. License: see COPYING file.

#include <stack>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/contractor_bc3.hpp"
#include "realpaver/param.hpp"

namespace realpaver {

Bc3Contractor::Bc3Contractor(Dag* dag, size_t i, size_t iv) :
   f_(dag, i, iv),
   peeler_(Param::DefBC3PeelWidth()),
   smax_(Param::DefBC3MaxSteps())
{
   newton_ = new IntervalNewton();
}

Bc3Contractor::~Bc3Contractor()
{
   delete newton_;
}

int Bc3Contractor::maxSteps() const
{
   return smax_;
}

void Bc3Contractor::setMaxSteps(const int& val)
{
   ASSERT(val > 0, "bad parameter in the BC3 contractor");

   smax_ = val;
}

IntervalNewton* Bc3Contractor::getNewton() const
{
   return newton_;
}

Proof Bc3Contractor::shrinkLeft(const Interval& x, Interval& res)
{
   return shrink(x, res, SplitLeft, PeelLeft);
}

Proof Bc3Contractor::shrinkRight(const Interval& x, Interval& res)
{
   return shrink(x, res, SplitRight, PeelRight);   
}

Proof Bc3Contractor::isConsistent(const Interval& x)
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

Scope Bc3Contractor::scope() const
{
   return f_.getFun()->scope();
}

bool Bc3Contractor::dependsOn(const Bitset& bs) const
{
   return f_.dependsOn(bs);
}

bool Bc3Contractor::SplitLeft(const Interval& x, Interval& x1, Interval& x2)
{
   double c = x.midpoint();
   x1 = Interval(c,x.right());
   x2 = Interval(x.left(), c);
   return x.strictlyContains(c);
}

bool Bc3Contractor::SplitRight(const Interval& x, Interval& x1, Interval& x2)
{
   double c = x.midpoint();
   x1 = Interval(x.left(), c);
   x2 = Interval(c,x.right());
   return x.strictlyContains(c);
}

void Bc3Contractor::PeelLeft(const Interval& x, IntervalPeeler& peeler,
                             Interval& b, Interval& r)
{
   b = peeler.peelLeft(x);
   r = Interval(b.right(), x.right());
}

void Bc3Contractor::PeelRight(const Interval& x, IntervalPeeler& peeler,
                              Interval& b, Interval& r)
{
   b = peeler.peelRight(x);
   r = Interval(x.left(), b.left());
}

Proof Bc3Contractor::shrink(const Interval& x, Interval& res,
                            SplitFun split_fun, PeelFun peel_fun)
{
   std::stack<Interval> stak;
   Interval b, z, z1, z2;
   Proof proof;
   size_t num_steps = 0;

   stak.push(x);
   while (!stak.empty())
   {
      Interval y( stak.top() );
      stak.pop();
      
      if (++num_steps > smax_)
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

Proof Bc3Contractor::contract(IntervalVector& X)
{
   Interval lsol, rsol;
   Proof proof, certif;

   size_t iv = f_.getVarIndex();
   const Interval& img = f_.getFun()->getImage();

   // first interval evaluation that also thickens the function
   Interval e = f_.update(X);

   // consistency checking
   if (e.isEmpty())
      return Proof::Empty;

   if (!e.overlaps(img))
      return Proof::Empty;

   if (img.contains(e))
      return Proof::Inner;

   // shrinks the left bound
   proof = shrinkLeft(X[iv], lsol);

   if (proof == Proof::Empty)
      return Proof::Empty;

   // shrinks the right bound
   Interval y(lsol.left(), X[iv].right());
   certif = shrinkRight(y, rsol);

   // assigns the contracted domain in V
   X.set(iv, lsol | rsol);

   return std::max(proof,certif);
}

void Bc3Contractor::print(std::ostream& os) const
{
   os << "BC3 contractor #" << f_.getFun()->index();
}

} // namespace
