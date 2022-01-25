// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_NEWTON_HPP
#define REALPAVER_NEWTON_HPP

#include "realpaver/inflator.hpp"
#include "realpaver/stopping.hpp"
#include "realpaver/uni_fun.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of operators implementing the univariate interval Newton method.
 * 
 * Given a univariate interval function f(x) and an interval I, the contraction
 * method approximates the set {x in X : f(x) = 0} given a domain X for x.
 * 
 * The local search method tries to find an enclosure of a zero, i.e. an
 * element of the set {x in X : f(x) = 0}.
 *****************************************************************************/
class Newton {
public:
   // constructor
   // the parameters are assigned to the default values
   Newton();

   // contraction method
   // returns a certificate of proof
   Proof contract(UniFun& f, Interval& x);

   // Newton step of the contraction method
   // x := x inter hull( c - f(c) / f'(x) ) with c = midpoint(x)
   Proof step(UniFun& f, Interval& x);

   // local search of a zero of the function from the midpoint of x
   // returns a certificate of proof
   Proof localSearch(UniFun& f, Interval& x);

   // Newton step of the local search method
   Proof localStep(UniFun& f, Interval& x);

   // improvement factor used to stop the contract method
   Improvement improvement() const;
   void setImprovement(const Improvement& val);

   // management of the maximum number of steps of the iterative methods
   int maxSteps() const;
   void setMaxSteps(const int& val);

   // management of the precision used to stop the contract method
   Precision precision() const;
   void setPrecision(const Precision& val);

   // management of the inflation operator
   Inflator inflator() const;
   void setInflator(const Inflator& val);

private:
   Improvement imp_;    // improvement factor used to stop contract
   int smax_;           // maximum number of steps
   Precision prec_;     // threshold on the precision
   Inflator inflator_;  // inflation operator for the local search
};

inline Improvement Newton::improvement() const
{
   return imp_;
}

inline void Newton::setImprovement(const Improvement& val)
{
   imp_ = val;
}

inline int Newton::maxSteps() const
{
   return smax_;
}

inline void Newton::setMaxSteps(const int& val)
{
   ASSERT(val > 0, "bad parameter in the interval Newton method");

   smax_ = val;
}

inline Precision Newton::precision() const
{
   return prec_;
}

inline void Newton::setPrecision(const Precision& val)
{
   prec_ = val;
}

inline Inflator Newton::inflator() const
{
   return inflator_;
}

inline void Newton::setInflator(const Inflator& val)
{
   inflator_ = val;
}

} // namespace

#endif
