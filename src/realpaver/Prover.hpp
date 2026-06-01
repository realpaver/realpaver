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
 * @file   Prover.hpp
 * @brief  Prover based on interval Newton
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_PROVER_HPP
#define REALPAVER_PROVER_HPP

#include "realpaver/DagExtended.hpp"
#include <memory>

namespace realpaver {

/**
 * @brief Prover based on interval Newton.
 *
 * It implements an epsilon-inflation algorithm based on an interval Newton
 * operator applied to square system of equations.
 *
 * The epsilon-inflation algorithm has two parameters, delta and chi.
 *
 * @see Inflator
 * @see IntervalNewton
 */
class Prover {
public:
   /// Constructor
   Prover(SharedDagExtended edag);

   /// Default destructor
   ~Prover() = default;

   /// No copy
   Prover(const Prover &) = delete;

   /// No asignment
   Prover &operator=(const Prover &) = delete;

   /// Certification method
   Proof certify(IntervalBox &B);

   /// Certification method
   Proof certify(IntervalBox &B, const Bitset &active);

   /// Assigns the initial box
   void setInitBox(const IntervalBox &B);

   /// Sets a limit of iterations of the Newton operator
   void setMaxIter(size_t n);

   /// Returns the maximum number of iterations of the Newton operator
   size_t getMaxIter() const;

   /// Returns the value of delta
   double getInflationDelta() const;

   /// Assigns delta
   void setInflationDelta(const double &val);

   /// Returns the value of chi
   double getInflationChi() const;

   /// Assigns chi
   void setInflationChi(const double &val);

private:
   SharedDagExtended edag_;               // extended dag
   std::unique_ptr<IntervalBox> initBox_; // initial hull
   double delta_;                         // parameter delta of inflation
   double chi_;                           // parameter chi of inflation
   size_t maxiter_;                       // maximum number of iterations
};

} // namespace realpaver

#endif
