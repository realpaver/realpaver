///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_NEWTON_UNI_HPP
#define REALPAVER_INTERVAL_NEWTON_UNI_HPP

#include <functional>
#include "realpaver/Inflator.hpp"
#include "realpaver/Tolerance.hpp"
#include "realpaver/IntervalFunctionUni.hpp"

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
/// - the distance between Xk and Xk-1 is greater than a given tolerance
/// - the method diverges
///////////////////////////////////////////////////////////////////////////////
class IntervalNewtonUni {
public:
   /// Creates an interval Newton operator
   /// The parameters are assigned to the default values.
   IntervalNewtonUni();

   /// Contraction method
   /// @param f a univariate function
   /// @param x an interval that is possibly contracted
   /// @return a certificate of proof
   Proof contract(IntervalFunctionUni& f, Interval& x);

   /// Combines the contraction method with search
   /// @param f a univariate function
   /// @param x an interval that is possibly contracted
   /// @return a certificate of proof
   Proof search(IntervalFunctionUni& f, Interval& x);

   /// Step of the contraction method
   /// @param f a univariate function
   /// @param x an interval that is possibly contracted
   /// @return a certificate of proof
   ///
   /// The interval x is contracted as the intersection of x and
   /// the set hull( c - f(c) / f'(x) ) where c is the midpoint of x.
   Proof step(IntervalFunctionUni& f, Interval& x);

   /// Local search and certification method
   /// @param f a univariate function
   /// @param x an interval
   /// @return a certificate of proof
   Proof localSearch(IntervalFunctionUni& f, Interval& x);

   /// Step of the local search method
   /// @param f a univariate function
   /// @param x an interval
   /// @return a certificate of proof
   Proof localStep(IntervalFunctionUni& f, Interval& x);

   /// Sets a limit of iterations of the iterative methods
   /// @param n new value of the limit
   void setMaxIter(size_t n);

   /// @return the maximum number of iterations of the iterative method
   size_t getMaxIter() const;

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
   /// @param tol absolute or relative tolerance`
   ///
   /// An iteration stops if the distance between two consecutive is greater
   /// than the tolerance, i.e. the method diverges.
   void setLocalDTol(const Tolerance& tol);

   /// @returns the inflator used by the local search method
   Inflator& getInflator();

   /// Sets the inflator used by the local search method
   /// @param inflator the new inflator
   void setInflator(const Inflator& inflator);

private:
   size_t maxiter_;
   Tolerance xtol_;
   Tolerance dtol_;
   Tolerance ldtol_;
   Inflator inflator_;

   Proof shrinkLeft(IntervalFunctionUni& f, Interval& x);
   Proof shrinkRight(IntervalFunctionUni& f, Interval& x);
};

} // namespace

#endif
