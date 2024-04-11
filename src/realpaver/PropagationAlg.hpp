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
 * @file   PropagationAlg.hpp
 * @brief  Constraint propagation algorithm
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_PROPAGATION_ALG_HPP
#define REALPAVER_PROPAGATION_ALG_HPP

#include "realpaver/ContractorPool.hpp"

namespace realpaver {

/**
 * @brief Constraint propagation algorithm.
 *
 * A propagator implements an AC3-like  constraint propagation algorithm
 * over a set of contractors. It is itself a contractor, hence it can be
 * combined with other contractors in new propagators. This is a composite.
 *
 * A propagator stops in three situations:
 * - a doman is empty;
 * - a maximum number of iterations is reached;
 * - the domains are not reduced enough with respect to a given tolerance.
 * 
 * The tolerance represents an improvement factor, which is a percentage of
 * reduction of the width of a box.
 */
class PropagationAlg : public Contractor {
public:
   /// Constructor given a pool of contractors
   PropagationAlg(SharedContractorPool pool = nullptr);

   /// Default copy constructor
   PropagationAlg(const PropagationAlg&) = default;

   /// No assignment
   PropagationAlg& operator=(const PropagationAlg&) = delete;

   /// Default destructor
   ~PropagationAlg() = default;

   /// Returns the number of contractors
   size_t poolSize() const;

   /// Returns the pool
   SharedContractorPool getPool() const;

   /// Sets the pool
   void setPool(SharedContractorPool pool);

   /// Inserts a contractor in this
   void push(SharedContractor op);

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

   /// Returns the tolerance used as stopping criterion
   Tolerance getTol() const;

   /// Sets the tolerance used as stopping criterion
   void setTol(Tolerance tol);

   /// Returns the maximum number of propagation steps
   size_t getMaxIter() const;

   /// Sets the maximum number of propagation steps
   void setMaxIter(size_t n);

   /// Gets the proof returned by the i-th contractor of this
   Proof proofAt(size_t i) const;

private:
   SharedContractorPool pool_;   // pool of contractors
   Tolerance tol_;               // tolerance used for propagation
   size_t maxiter_;              // maximum number of propagation steps
   std::vector<Proof> certif_;   // proof certificates of contractors

   // hash set of variables
   typedef std::unordered_set<Variable, VariableHasher, VariableEqual>
         ModifSetType;

   // returns true if the i-th contractor of this depends on a variable that
   // belongs to the set ms
   bool contractorDependsOn(size_t i, const ModifSetType& ms);

   // algorithm that propagates after each application of one contractor
   Proof contractBis(IntervalBox& B);
};

/// Type of shared pointers on propagators
using SharedPropagationAlg = std::shared_ptr<PropagationAlg>;

} // namespace

#endif
