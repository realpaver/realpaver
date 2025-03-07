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
 * @file   IntervalGaussSeidel.hpp
 * @brief  Interval Gauss Seidel method
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_INTERVAL_GAUSS_SEIDEL_HPP
#define REALPAVER_INTERVAL_GAUSS_SEIDEL_HPP

#include "realpaver/IntervalMatrix.hpp"
#include "realpaver/IntervalVector.hpp"

namespace realpaver {

/**
 * @brief Interval Gauss Seidel method for solving interval linear systems.
 *
 *  This is an iterative method with two steps.
 *
 *  The inner step inverses each row of the linear system with respect to
 *  the diagonal. It is equivalent to HC4 contractors on linear equations.
 *
 *  The outer step iterates the inner step until a stopping criterion is
 *  verified. This criterion may defined as follows. Let Xk, xk-1 be two
 *  consecutive unknown vectors. The algorithm returns Xk if one of the
 *  following conditions is verified:
 *  - Xk is empty;
 *  - Xk is not improved enough with respect to Xk-1;
 *  - the limit on the number of iterations is reached.
 *
 * The improvement factor is given as a tolerance which represents a percentage
 * of reduction of the width of an interval.
 */
class IntervalGaussSeidel {
public:
   /// Constructor
   IntervalGaussSeidel();

   /// Default copy constructor
   IntervalGaussSeidel(const IntervalGaussSeidel&) = default;

   /// No assignment
   IntervalGaussSeidel& operator=(const IntervalGaussSeidel&) = delete;

   /// Default destructor
   ~IntervalGaussSeidel() = default;

   /// Contracts x with respect to Ax = b and returns a certificate of proof
   Proof contract(const IntervalMatrix& A, IntervalVector& x,
                  const IntervalVector& b);

   /**
    * @brief Contraction method with preconditionning.
    *
    * Contracts x with respect to PAx = Pb and where P is the inverse of the
    * midpoint of A returns a certificate of proof
    */
   Proof contractPrecond(const IntervalMatrix& A, IntervalVector& x,
                         const IntervalVector& b);

   /// Sets a limit of iterations of the iterative method
   void setMaxIter(size_t n);

   /// Returns the maximum number of iterations of the iterative method
   size_t getMaxIter() const;

   /// Returns the improvement factor
   double getTol() const;

   /// Sets the improvement factor
   void setTol(const double& tol);

private:
   size_t maxiter_;  // maximum number of iterations
   double tol_;      // improvement factor

   // inner step
   // returns 0 if the system is not satisfiable; 1 if the system is satisfiable
   // but x is not improved enough, 2 otherwise
   int innerStep(const IntervalMatrix& A, IntervalVector& x,
                 const IntervalVector& b);
};

} // namespace

#endif
