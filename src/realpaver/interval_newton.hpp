///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, a reliable interval solver of nonlinear   //
//                                 constraint satisfaction and optimization  //
//                                 problems over the real numbers.           //
//                                                                           //
// Copyright (C) 2020-2022 Laboratoire des Sciences du Numérique de Nantes   //
//                                                                           //
// Realpaver is a software distributed under the terms of the MIT License.   //
// See the file COPYING.                                                     //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_NEWTON_HPP
#define REALPAVER_INTERVAL_NEWTON_HPP

#include "realpaver/inflator.hpp"
#include "realpaver/uni_fun.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This represents a univariate interval Newton operator.
///
/// Given a univariate interval function f(x) and an interval X, the
/// contraction method calculates a new interval Y included in X such that
/// Y contains the solution set {x in X : f(x) = 0}. It returns a certificate
/// of proof having one of the following values:
/// - Proof::Empty if f has no solution in X (infeasibility)
/// - Proof::Maybe if no proof is derived
/// - Proof::Feasible if the existence of a solution (zero of f in X) is proven
/// - Proof::Inner if Y is included in the solution set
///
/// The contraction method stops on the interval Xk if one of the following
/// conditions is verified:
/// - Xk is empty
/// - the precision of Xk (absolute or relative width) is smaller
///   than an epsilon
/// - Xk does not improve enough the previous interval
/// - the limit on the number of iterations is reached
///
/// Given a univariate interval function f(x) and an interval X, the local
/// search method tries to find an enclosure of a zero of f in X and to prove
/// its existence. It uses the midpoint of X as a starting point. The input
/// interval X can be reduced to one point representing e.g. an approximate
/// solution obtained from a local (non reliable) solver.
///
/// The local search method stops on the interval Xk when a zero is found if
/// one of the following conditions is verified:
/// - Xk is empty
/// - the existence of a zero in Xk is proven
/// - the limit on the number of iterations is reached
/// - the method diverges
///////////////////////////////////////////////////////////////////////////////
class IntervalNewton {
public:
   /// Creates an interval Newton operator
   /// The parameters are assigned to the default values.
   IntervalNewton();

   /// Contraction method
   /// @param f a univariate function
   /// @param x an interval that is possibly contracted
   /// @return a certificate of proof
   Proof contract(UniFun& f, Interval& x);

   /// Step of the contraction method
   /// @param f a univariate function
   /// @param x an interval that is possibly contracted
   /// @return a certificate of proof
   ///
   /// The interval x is contracted as the intersection of x and
   /// the set hull( c - f(c) / f'(x) ) where c is the midpoint of x.
   Proof step(UniFun& f, Interval& x);

   /// Local search method
   /// @param f a univariate function
   /// @param x an interval
   /// @return a certificate of proof
   Proof localSearch(UniFun& f, Interval& x);

   /// Step of the local search method
   /// @param f a univariate function
   /// @param x an interval
   /// @return a certificate of proof
   Proof localStep(UniFun& f, Interval& x);

   /// Gets the improvement factor of the contraction method
   IntervalImprovement getImprovement() const;

   /// Sets the improvement factor of the contraction method
   /// @param imp new value of the improvement factor
   void setImprovement(const IntervalImprovement& imp);

   /// Sets a limit of iterations of the iterative methods
   /// @param n new value of the limit
   void setMaxIterations(int n);

   /// @return the maximum number of iterations of the iterative method
   int getMaxIterations() const;

   /// @return the precision used to stop the contraction method
   IntervalPrecision getPrecision() const;

   /// Sets the precision used to stop the contraction method
   /// @param prec new value of the precision
   void setPrecision(const IntervalPrecision& prec);

   /// @returns the inflator used by the local search method
   Inflator getInflator() const;

   /// Sets the inflator used by the local search method
   /// @param inflator the new inflator
   void setInflator(const Inflator& inflator);

private:
   IntervalImprovement imp_;    // improvement factor used to stop contract
   int maxiter_;        // limit on the number of iterations
   IntervalPrecision prec_;
   Inflator inflator_;  // inflation operator for the local search
};

} // namespace

#endif
