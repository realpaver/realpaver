///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BC3_CONTRACTOR_HPP
#define REALPAVER_BC3_CONTRACTOR_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/IntervalNewton.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/ThickIntervalFunction.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the BC3Revise contractor implementing box consistency.
///
/// It applies to a bounded thick interval function with form a <= F(x) <= b.
/// Given x in X, it finds the outermost consistent values by combining
/// search with an interval Newton method. It returns the interval [c, d]
/// such that c is the smallest value in X verifying a <= F(c) <= b and d
/// is the greatest value in X such that a <= F(d) <= b. If there is no
/// consistent value in X, it returns the empty set. This is the theory.
///
/// In practice, we use a tolerance to check the consistency of small intervals
/// at the bounds of domains. The peel factor is a percentage of the width of
/// an interval.
//////////////////////////////////////////////////////////////////////////////////
class BC3Contractor : public Contractor {
public:
   /// Creates a contractor
   /// @param dag a DAG
   /// @param i index of a function in the DAG
   /// @param v a variable occurring in the function
   ///
   /// This does not own the dag.
   BC3Contractor(Dag* dag, size_t i, Variable v);

   /// Destructor
   ~BC3Contractor();

   /// No copy
   BC3Contractor(const BC3Contractor&) = delete;

   /// No assignment
   BC3Contractor& operator=(const BC3Contractor&) = delete;

   /// @return the peel factor
   double getPeelFactor() const;

   /// Sets the peel factor
   /// @param f f/100 is a percentage with f >= 0.0 and f <= 100.0
   void setPeelFactor(double f);

   /// @return the maximum number of steps in the iterative method
   size_t getMaxIter() const;

   /// Sets the maximum number of steps in the iterative method
   /// @param val new value
   void setMaxIter(size_t val);

   /// @return the Newton operator enclosed
   ///
   /// Useful to change its parameters
   IntervalNewton* getNewton() const;

   ///@{
   /// Overrides (Contractor)
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalRegion& reg);
   void print(std::ostream& os) const;
   ///@}

private:
   ThickIntervalFunction f_;  // univariate thick interval function
   IntervalPeeler peeler_;    // peeling at interval bounds
   size_t maxiter_;           // maximum number of steps in shrink
   IntervalNewton* newton_;   // interval Newton method

   // split functions
   typedef bool (*SplitFun)(const Interval& x, Interval& x1, Interval& x2);
   static  bool splitLeft  (const Interval& x, Interval& x1, Interval& x2);
   static  bool splitRight (const Interval& x, Interval& x1, Interval& x2);

   // peeling functions
   typedef void (*PeelFun)(const Interval& x, IntervalPeeler& peeler,
                           Interval& b, Interval& r);
   static  void peelLeft  (const Interval& x, IntervalPeeler& peeler,
                          Interval& b, Interval& r);
   static  void peelRight (const Interval& x, IntervalPeeler& peeler,
                          Interval& b, Interval& r);

   // shrink functions
   Proof shrinkLeft(const Interval& x, Interval& res);
   Proof shrinkRight(const Interval& x, Interval& res);
   Proof shrink(const Interval& x, Interval& res,
                SplitFun split_fun, PeelFun peel_fun);

   // consistency checking
   Proof isConsistent(const Interval& x);
};

} // namespace

#endif
