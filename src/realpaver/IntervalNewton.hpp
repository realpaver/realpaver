///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_NEWTON_HPP
#define REALPAVER_INTERVAL_NEWTON_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/IntervalFunctionVector.hpp"
#include "realpaver/IntervalGaussSeidel.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the multivariate interval Newton method.
///////////////////////////////////////////////////////////////////////////////
class IntervalNewton : public Contractor {
public:
   /// Constructor
   /// @param F vector of functions
   IntervalNewton(IntervalFunctionVector F);

   /// Destructor
   ~IntervalNewton();

   /// No copy
   IntervalNewton(const IntervalNewton&) = delete;

   /// No assignment
   IntervalNewton& operator=(const IntervalNewton&) = delete;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalRegion& X) override;
   void print(std::ostream& os) const override;
   ///@}

   /// @return the interval GFauss Seidel operator of this
   IntervalGaussSeidel* getGaussSeidel() const;

   /// Sets a limit of iterations of the iterative method
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

private:
   IntervalFunctionVector F_;       // vector of interval functions
   IntervalMatrix jac_;             // Jacobian matrix
   IntervalVector val_, y_, b_;     // working vectors
   RealPoint c_;                    // point of expansion
   IntervalGaussSeidel* gs_;        // Gauss-Seidel operator
   size_t maxiter_;           // maximum number of iterations
   Tolerance xtol_;           // tolerance on the width of an interval
   Tolerance dtol_;           // tolerance on the distance between two intervals

   void makeY(IntervalRegion& X);
   Proof reduceX(IntervalRegion& X, bool& hasxtol, bool& hasdtol);
};

} // namespace

#endif
