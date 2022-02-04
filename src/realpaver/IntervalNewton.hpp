///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_NEWTON_HPP
#define REALPAVER_INTERVAL_NEWTON_HPP

#include "realpaver/Inflator.hpp"
#include "realpaver/Tolerance.hpp"
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
/// - the width of Xk is smaller than a given tolerance
/// - the distance between Xk and Xk-1 is smaller than a given tolerance
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
/// - the distance between Xk and Xk-1 is smaller than a given tolerance
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

   /// Sets a limit of iterations of the iterative methods
   /// @param n new value of the limit
   void setMaxIterations(int n);

   /// @return the maximum number of iterations of the iterative method
   int getMaxIterations() const;

   /// @return the tolerance on the width of an interval in the contraction
   ///         method
   Tolerance getXTol() const;

   /// Sets the tolerance on the width of an interval in the contraction method
   /// @param tol absolute or relative tolerance
   void setXTol(const Tolerance& tol);

   /// @return the tolerance on the distance between two consecutive intervals
   ///         in the contraction method
   Tolerance getDTol() const;

   /// Sets the tolerance on the distance between two consecutive intervals
   /// in the contraction method
   /// @param tol absolute or relative tolerance
   void setDTol(const Tolerance& tol);

   /// @return the tolerance on the distance between two consecutive intervals
   ///         in the local search method
   Tolerance getLocalDTol() const;

   /// Sets the tolerance on the distance between two consecutive intervals
   /// in the local search method
   /// @param tol absolute or relative tolerance
   void setLocalDTol(const Tolerance& tol);

   /// @returns the inflator used by the local search method
   Inflator getInflator() const;

   /// Sets the inflator used by the local search method
   /// @param inflator the new inflator
   void setInflator(const Inflator& inflator);

private:
   int maxiter_;
   Tolerance xtol_;
   Tolerance dtol_;
   Tolerance ldtol_;
   Inflator inflator_;
};

} // namespace

#endif
