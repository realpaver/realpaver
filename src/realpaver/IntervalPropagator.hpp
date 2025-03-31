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
 * @file   IntervalPropagator.hpp
 * @brief  Constraint propagation over interval contractors
 * @author Laurent Granvilliers
 * @date   2024-7-1
 */

#ifndef REALPAVER_INTERVAL_PROPAGATOR_HPP
#define REALPAVER_INTERVAL_PROPAGATOR_HPP

#include "realpaver/ContractorPool.hpp"
#include <vector>

namespace realpaver {

class IntervalPropagator : public Contractor {
public:
   /// Constructor given a pool of contractors
   IntervalPropagator(SharedContractorPool pool = nullptr);

   /// Default copy constructor
   IntervalPropagator(const IntervalPropagator &) = default;

   /// No assignment
   IntervalPropagator &operator=(const IntervalPropagator &) = delete;

   /// Default destructor
   ~IntervalPropagator() = default;

   /// Returns the number of contractors
   size_t poolSize() const;

   /// Returns the pool
   SharedContractorPool getPool() const;

   /// Sets the pool
   void setPool(SharedContractorPool pool);

   /// Inserts a contractor in this
   void push(SharedContractor op);

   Scope scope() const override;
   Proof contract(IntervalBox &B) override;
   void print(std::ostream &os) const override;

   /// Returns the tolerance used as stopping criterion
   double getTol() const;

   /// Sets the tolerance used as stopping criterion
   void setTol(double tol);

   /// Returns the maximum number of propagation steps
   size_t getMaxIter() const;

   /// Sets the maximum number of propagation steps
   void setMaxIter(size_t n);

   /// Gets the proof returned by the i-th contractor of this
   Proof proofAt(size_t i) const;

private:
   SharedContractorPool pool_; // pool of contractors
   double tol_;                // tolerance used for propagation
   std::vector<Proof> certif_; // proof certificates of contractors
};

} // namespace realpaver

#endif
