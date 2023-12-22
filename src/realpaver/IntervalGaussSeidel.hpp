///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_GAUSS_SEIDEL_HPP
#define REALPAVER_INTERVAL_GAUSS_SEIDEL_HPP

#include "realpaver/IntervalMatrix.hpp"
#include "realpaver/IntervalVector.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// Interval Gauss Seidel method for solving interval linear systems.
///
/// This is an iterative method with two steps.
///
/// The inner step inverses each row of the linear system with respect to
/// the diagonal. It is equivalent to HC4 contractors on linear equations.
///
/// The outer step iterates the inner step until a stopping criterion is
/// verified. This criterion may defined as follows. Let Xk, xk-1 be two
/// consecutive unknown vectors. The algorithm returns Xk if one of the
/// following conditions is verified:
/// - Xk is empty;
/// - the width of Xk is smaller than a given tolerance;
/// - the distance between Xk and Xk-1 is smaller than a given tolerance;
/// - the limit on the number of iterations is reached.
///////////////////////////////////////////////////////////////////////////////
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

   /// Contraction method
   /// @param A a matrix
   /// @param x an unknown vector
   /// @param b a vector
   /// @return the proof certificate resulting from the solving of Ax = b
   ///
   /// x is contracted by this operation
   Proof contract(const IntervalMatrix& A, IntervalVector& x,
                  const IntervalVector& b);

   /// Contraction method with preconditionning
   /// @param A a matrix
   /// @param x an unknown vector
   /// @param b a vector
   /// @return the proof certificate resulting from the solving of PAx = Pb
   ///         where P is the inverse of the midpoint of A
   ///
   /// x is contracted by this operation
   Proof contractPrecond(const IntervalMatrix& A, IntervalVector& x,
                         const IntervalVector& b);

   /// Sets a limit of iterations of the iterative method
   /// @param n new value of the limit
   void setMaxIter(size_t n);

   /// @return the maximum number of iterations of the iterative method
   size_t getMaxIter() const;

   /// @return the tolerance on the distance between two consecutive intervals
   ///         in the contraction method
   Tolerance getTol() const;

   /// Sets the tolerance on the distance between two consecutive intervals
   /// in the contraction method
   /// @param tol a tolerance
   void setTol(const Tolerance& tol);

private:
   size_t maxiter_;  // maximum number of iterations
   Tolerance tol_;   // tolerance on the distance between two intervals

   // inner step
   // returns 0 if the system is not satisfiable; 1 if the system is satisfiable
   // but the distance between the input vector x and the output x is smaller
   // than dtol_, 2 otherwise
   int innerStep(const IntervalMatrix& A, IntervalVector& x,
                 const IntervalVector& b);
};

} // namespace

#endif
