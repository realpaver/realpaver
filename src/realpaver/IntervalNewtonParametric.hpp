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
 * @file   IntervalNewtonParametric.hpp
 * @brief  Parametric interval Newton method
 * @author Laurent Granvilliers
 * @date   13 May 2026
 */

#ifndef REALPAVER_INTERVAL_NEWTON_PARAMETRIC_HPP
#define REALPAVER_INTERVAL_NEWTON_PARAMETRIC_HPP

#include "realpaver/IntervalFunctionVector.hpp"
#include "realpaver/IntervalGaussSeidel.hpp"

namespace realpaver {

class IntervalNewtonParametric {
public:
   /// Constructor
   IntervalNewtonParametric(IntervalFunctionVector F);

   /// Default destructor
   ~IntervalNewtonParametric() = default;

   /// No copy constructor
   IntervalNewtonParametric(const IntervalNewtonParametric &) = delete;

   /// No assignment
   IntervalNewtonParametric &operator=(const IntervalNewtonParametric &) = delete;

   /**
    * @brief Certification method.
    *
    * Tries to derive a proof certificate for the existence of a solution in
    * a box using an inflation-based algorithm. The box can be modified.
    */
   Proof certify(IntervalBox &B);

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
   IntervalFunctionVector F_;                // vector of interval functions
   std::unique_ptr<IntervalGaussSeidel> gs_; // Gauss-Seidel operator

   double delta_;    // parameter delta of inflation
   double chi_;      // parameter chi of inflation
   size_t cmaxiter_; // maximum number of iterations

   bool makeScopes(IntervalBox &B, Scope &mainScop, Scope &paramScop);
   void makeC(const IntervalBox &X, const Scope &mainScop, IntervalBox &c);
   void makeY(const IntervalBox &X, const IntervalBox &c, const Scope &mainScop,
              IntervalVector &y);
   Proof certifyX(const IntervalBox &c, const IntervalVector &y, const Scope &mainScope,
                  IntervalBox &X);
};

} // namespace realpaver

#endif
