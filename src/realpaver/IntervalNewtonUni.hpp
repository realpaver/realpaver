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
 * @file   IntervalNewtonUni.hpp
 * @brief  Univariate interval Newton operator
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_INTERVAL_NEWTON_UNI_HPP
#define REALPAVER_INTERVAL_NEWTON_UNI_HPP

#include "realpaver/Inflator.hpp"
#include "realpaver/IntervalFunctionUni.hpp"

namespace realpaver {

/**
 * @brief Univariate interval Newton operator.
 *
 *  Given a univariate interval function f(x) and an interval X, the
 *  contraction method calculates a new interval Y included in X such that
 *  Y contains the solution set {x in X : f(x) = 0}. It returns a certificate
 *  of proof having one of the following values:
 *  - Proof::Empty if f has no solution in X (infeasibility)
 *  - Proof::Maybe if no proof is derived
 *  - Proof::Feasible if the existence of a solution (zero of f in X) is proven
 *  - Proof::Inner if Y is included in the solution set
 *
 *  The contraction method stops on the interval Xk if one of the following
 *  conditions is verified:
 *  - Xk is empty
 *  - the width of Xk is smaller than a given tolerance
 *  - the distance between Xk and Xk-1 is smaller than a given tolerance
 *  - the limit on the number of iterations is reached
 *
 *  Given a univariate interval function f(x) and an interval X, the local
 *  search method tries to find an enclosure of a zero of f in X and to prove
 *  its existence. It uses the midpoint of X as a starting point. The input
 *  interval X can be reduced to one point representing e.g. an approximate
 *  solution obtained from a local (non reliable) solver.
 *
 *  The local search method stops on the interval Xk when a zero is found if
 *  one of the following conditions is verified:
 *  - Xk is empty
 *  - the existence of a zero in Xk is proven
 *  - the limit on the number of iterations is reached
 *  - the method diverges
 */
class IntervalNewtonUni {
public:
   /**
    * @brief Constructor.
    *
    * The parameters are assigned to the default values.
    */
   IntervalNewtonUni();

   /**
    * @brief Contraction method.
    *
    * Contracts x with respect to f = 0 using the interval Newton method and
    * returns a certificate of proof
    */
   Proof contract(IntervalFunctionUni& f, Interval& x);

   /**
    * @brief Contraction method.
    *
    * Contracts x with respect to f = 0 by combining the interval Newton method
    * with search and returns a certificate of proof
    */
   Proof contractWithSearch(IntervalFunctionUni& f, Interval& x);

   /**
    * @brief Step of the contraction method.
    *
    * The interval x is contracted as the intersection of x and the set
    * hull( c - f(c) / f'(x) ) where c is the midpoint of x.
    */
   Proof step(IntervalFunctionUni& f, Interval& x);

   /**
    * @brief Local search and certification method.
    *
    * Applies an inflation-based algorithm to f = 0 and x and returns a
    * certificate of proof.
    */
   Proof localSearch(IntervalFunctionUni& f, Interval& x);

   /// Step of the local search method
   Proof localStep(IntervalFunctionUni& f, Interval& x);

   /// Sets a limit of iterations of the iterative methods
   void setMaxIter(size_t n);

   /// Returns the maximum number of iterations of the iterative methods
   size_t getMaxIter() const;

   /// Returns the improvement factor of the contraction method
   double getTol() const;

   /// Sets the improvement factor of the contraction method
   void setTol(const double& tol);

   /// Returns the inflator used by the local search method
   Inflator& getInflator();

   /// Sets the inflator used by the local search method
   void setInflator(const Inflator& inflator);

private:
   size_t maxiter_;     // maxmum number of iterations
   double tol_;         // improvement factor
   Inflator inflator_;  // inflator

   Proof shrinkLeft(IntervalFunctionUni& f, Interval& x);
   Proof shrinkRight(IntervalFunctionUni& f, Interval& x);
};

} // namespace

#endif
