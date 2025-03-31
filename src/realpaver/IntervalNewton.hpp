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
 * @file   IntervalNewton.hpp
 * @brief  Interval Newton method
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_INTERVAL_NEWTON_HPP
#define REALPAVER_INTERVAL_NEWTON_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/IntervalFunctionVector.hpp"
#include "realpaver/IntervalGaussSeidel.hpp"

namespace realpaver {

/**
 * @brief Multivariate interval Newton method.
 *
 * Given a square system of equations F = 0 and a box, the contraction method
 * is a fixed-point loop applying an inner step until a stopping criterion is
 * verified. The inner step linearizes the system and applies the interval
 * Gauss-Seidel method. The stopping criterion is verified in the following
 * cases:
 * - the box is empty;
 * - the box is not improvement enough, the improvement factor being given by
 *   a tolerance;
 * - a maximum number of iterations is reached.
 * The contraction method is not applied if the width of the box is smaller than
 * a threshold.
 *
 * The certification method tries to prove the existence of a solution in a box
 * using an inflation-based algorithm.
 */
class IntervalNewton : public Contractor {
public:
   /// Constructor
   IntervalNewton(IntervalFunctionVector F);

   /// Destructor
   ~IntervalNewton();

   /// Copy constructor
   IntervalNewton(const IntervalNewton &);

   /// No assignment
   IntervalNewton &operator=(const IntervalNewton &) = delete;

   Scope scope() const override;
   Proof contract(IntervalBox &X) override;
   void print(std::ostream &os) const override;

   /// Returns the interval GFauss Seidel operator of this
   IntervalGaussSeidel *getGaussSeidel() const;

   /// Sets a limit of iterations of the contraction method
   void setMaxIter(size_t n);

   /// Returns the maximum number of iterations of the contraction method
   size_t getMaxIter() const;

   /// Returns the improvement factor
   double getTol() const;

   /// Sets the improvement factor
   void setTol(const double &tol);

   /// Assigns the threshold on the width of a box of the contraction method
   void setWidthLimit(double val);

   /// Returns the threshold on the width of a box of the contraction method
   double getWidthLimit() const;

   /**
    * @brief Certification method.
    *
    * Tries to derive a proof certificate for the existence of a solution in
    * a box using an inflation-based algorithm. The box can be modified.
    */
   Proof certify(IntervalBox &box);

   /// Returns the parameter delta of the inflation algorithm
   double getInflationDelta() const;

   /// Assigns the parameter delta of the inflation algorithm
   void setInflationDelta(const double &val);

   /// Returns the parameter chi of the inflation algorithm
   double getInflationChi() const;

   /// Assigns the parameter chi of the inflation algorithm
   void setInflationChi(const double &val);

   /// Sets a limit of iterations of the certification method
   void setCertifyMaxIter(size_t n);

   /// Returns the maximum number of iterations of the certification method
   size_t getCertifyMaxIter() const;

private:
   IntervalFunctionVector F_;   // vector of interval functions
   IntervalMatrix jac_;         // Jacobian matrix
   IntervalVector val_, y_, b_; // working vectors
   IntervalBox c_;              // point of expansion
   IntervalGaussSeidel *gs_;    // Gauss-Seidel operator

   size_t maxiter_; // maximum number of iterations (contraction)
   double tol_;     // tolerance on the distance between two intervals
   double wlim_;    // threshold on the width of a box: no application
                    // of the contraction method if the width of the
                    // input box is greater than this value

   double delta_;    // parameter delta of inflation
   double chi_;      // parameter chi of inflation
   size_t cmaxiter_; // maximum number of iterations

   void makeY(IntervalBox &X);
   void makeC(IntervalBox &X);
   Proof reduceX(IntervalBox &X, bool &improved);
   Proof certifyX(IntervalBox &X);
};

} // namespace realpaver

#endif
