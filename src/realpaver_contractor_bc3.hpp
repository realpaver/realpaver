// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONTRACTOR_BC3_HPP
#define REALPAVER_CONTRACTOR_BC3_HPP

#include "realpaver_contractor.hpp"
#include "realpaver_interval_slicer.hpp"
#include "realpaver_newton.hpp"
#include "realpaver_thick_fun.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of contractors implementing the BC3Revise operator.
 *****************************************************************************/
class Bc3Contractor : public Contractor {
public:
   // constructor of a BC3 contractor associated with the i-th function of
   // the given dag and the variable index iv, 0 <= i < dag->nbFun()
   Bc3Contractor(Dag* dag, size_t i, size_t iv);

   // destructor
   ~Bc3Contractor();

   // copy protection
   Bc3Contractor(const Bc3Contractor&) = delete;
   Bc3Contractor& operator=(const Bc3Contractor&) = delete;

   // management of the maximum number of steps in the shrink methods
   int maxSteps() const;
   void setMaxSteps(const int& val);

   // Newton operator
   Newton* getNewton() const;

   // Override
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(Box& B);
   void print(std::ostream& os) const;

private:
   ThickFun f_;               // univariate thick interval function
   IntervalPeeler peeler_;    // peeling at interval bounds
   size_t smax_;              // maximum number of steps in shrink
   Newton* newton_;           // interval Newton method

   // split functions
   typedef bool (*SplitFun)(const Interval& x, Interval& x1, Interval& x2);
   static  bool SplitLeft  (const Interval& x, Interval& x1, Interval& x2);
   static  bool SplitRight (const Interval& x, Interval& x1, Interval& x2);

   // peeling functions
   typedef void (*PeelFun)(const Interval& x, IntervalPeeler& peeler,
                           Interval& b, Interval& r);
   static  void PeelLeft  (const Interval& x, IntervalPeeler& peeler,
                          Interval& b, Interval& r);
   static  void PeelRight (const Interval& x, IntervalPeeler& peeler,
                          Interval& b, Interval& r);

   // shrink functions
   Proof shrinkLeft(const Interval& x, Interval& res);
   Proof shrinkRight(const Interval& x, Interval& res);
   Proof shrink(const Interval& x, Interval& res,
                SplitFun split_fun, PeelFun peel_fun);

   // consistency checking
   Proof isConsistent(const Interval& x);
};

inline int Bc3Contractor::maxSteps() const
{
   return smax_;
}

inline void Bc3Contractor::setMaxSteps(const int& val)
{
   ASSERT(val > 0, "bad parameter in the BC3 contractor");

   smax_ = val;
}

inline Newton* Bc3Contractor::getNewton() const
{
   return newton_;
}

inline Proof Bc3Contractor::shrinkLeft(const Interval& x, Interval& res)
{
   return shrink(x, res, SplitLeft, PeelLeft);
}

inline Proof Bc3Contractor::shrinkRight(const Interval& x, Interval& res)
{
   return shrink(x, res, SplitRight, PeelRight);   
}

inline Proof Bc3Contractor::isConsistent(const Interval& x)
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

inline Scope Bc3Contractor::scope() const
{
   return f_.getFun()->scope();
}

} // namespace

#endif
