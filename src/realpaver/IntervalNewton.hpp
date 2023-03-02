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

   /// Copy constructor
   IntervalNewton(const IntervalNewton&);

   /// No assignment
   IntervalNewton& operator=(const IntervalNewton&) = delete;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalRegion& X) override;
   void print(std::ostream& os) const override;
   ///@}

   /// @return the interval GFauss Seidel operator of this
   IntervalGaussSeidel* getGaussSeidel() const;

   /// Sets a limit of iterations of the contraction method
   /// @param n new value of the limit
   void setMaxIter(size_t n);

   /// @return the maximum number of iterations of the contraction method
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
   ///
   /// The iteration stops if two consecutive regions are close enough, i.e.
   /// their distance is smaller than tol
   void setDTol(const Tolerance& tol);

   /// Tries to derive a proof certificate for the existence of a solution
   /// in an interval region using an inflation-based algorithm
   /// @param reg a region
   /// @return proof certificate
   Proof certify(IntervalRegion& reg);

   /// @return parameter delta of inflation
   double getInflationDelta() const;

   /// Assigns the parameter delta of inflation
   /// @param val new value
   void setInflationDelta(const double& val);

   /// @return parameter chi of inflation
   double getInflationChi() const;

   /// Assigns the parameter chi of inflation
   /// @param val new value
   void setInflationChi(const double& val);

   /// Sets a limit of iterations of the certification method
   /// @param n new value of the limit
   void setCertifyMaxIter(size_t n);

   /// @return the maximum number of iterations of the certification method
   size_t getCertifyMaxIter() const;

   /// @return the tolerance on the distance between two consecutive intervals
   ///         in the certification method
   Tolerance getCertifyDTol() const;

   /// Sets the tolerance on the distance between two consecutive intervals
   /// in the certification method
   /// @param tol absolute or relative tolerance
   ///
   /// The iteration stops if two consecutive regions are far enough, i.e.
   /// their distance is greater than tol
   void setCertifyDTol(const Tolerance& tol);

private:
   IntervalFunctionVector F_;       // vector of interval functions
   IntervalMatrix jac_;             // Jacobian matrix
   IntervalVector val_, y_, b_;     // working vectors
   RealPoint c_;                    // point of expansion
   IntervalGaussSeidel* gs_;        // Gauss-Seidel operator
   size_t maxiter_;           // maximum number of iterations (contraction)
   Tolerance xtol_;           // tolerance on the width of an interval
   Tolerance dtol_;           // tolerance on the distance between two intervals
   double delta_;             // parameter delta of inflation
   double chi_;               // parameter chi of inflation
   size_t cmaxiter_;          // maximum number of iterations (certification)
   Tolerance cdtol_;          // tolerance on the distance between two intervals

   void makeY(IntervalRegion& X);
   Proof reduceX(IntervalRegion& X, bool& hasxtol, bool& hasdtol);
};

} // namespace

#endif
