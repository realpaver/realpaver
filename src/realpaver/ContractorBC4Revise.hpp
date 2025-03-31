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
 * @file   ContractorBC4Revise.hpp
 * @brief  BC4Revise contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CONTRACTOR_BC4_REVISE_HPP
#define REALPAVER_CONTRACTOR_BC4_REVISE_HPP

#include "realpaver/ContractorBC3Revise.hpp"
#include "realpaver/ContractorHC4Revise.hpp"
#include <vector>

namespace realpaver {

/**
 * @brief BC4Revise contractor implementing hull/box consistency.
 *
 * Given a constraint a <= f(x1, ..., xn) <= b, it applies first an HC4
 * contractor. Then it applies a BC3 contractor for each variable with multiple
 * occurrences in f.
 */
class ContractorBC4Revise : public Contractor {
public:
   /// Creates a contractor associated with the i-th constraint of a DAG
   ContractorBC4Revise(SharedDag dag, size_t i);

   /// Destructor
   ~ContractorBC4Revise();

   /// No copy
   ContractorBC4Revise(const ContractorBC4Revise &) = delete;

   /// No assignment
   ContractorBC4Revise &operator=(const ContractorBC4Revise &) = delete;

   /// Sets the peel factor of BC3Revise
   void setPeelFactor(double f);

   /// Sets the maximum number of steps of BC3Revise
   void setMaxIter(size_t val);

   Scope scope() const override;
   Proof contract(IntervalBox &B) override;
   void print(std::ostream &os) const override;

private:
   SharedDag dag_;                          // a dag
   size_t if_;                              // index of a function f in the dag
   ContractorHC4Revise *hc4_;               // hc4 contractor associated with f
   std::vector<ContractorBC3Revise *> bc3_; // bc3 contractors associated with
                                            // the variables having multiple
                                            // occurrences in f
};

} // namespace realpaver

#endif
