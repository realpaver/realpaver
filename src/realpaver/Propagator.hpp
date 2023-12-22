///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_PROPAGATOR_HPP
#define REALPAVER_PROPAGATOR_HPP

#include "realpaver/ContractorPool.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a constraint propagation algorithm.
///
/// A propagator implements an AC3-like  constraint propagation algorithm
/// over a set of contractors. It is itself a contractor, hence it can be
/// combined with other contractors in new propagators. This is a composite.
///
/// A propagator stops in three situations:
/// - a doman is empty;
/// - a maximum number of iterations is reached;
/// - the domains are not reduced enough with respect to a given tolerance,
///   i.e. for every variable the distance between two consecutive domains
///   is smaller than the tolerance.
///////////////////////////////////////////////////////////////////////////////
class Propagator : public Contractor {
public:
   /// Creates a propagator over a pool of contractors
   /// @param pool a pool of contractors
   Propagator(SharedContractorPool pool = nullptr);

   /// Default copy constructor
   Propagator(const Propagator&) = default;

   /// No assignment
   Propagator& operator=(const Propagator&) = delete;

   /// Default destructor
   ~Propagator() = default;

   /// @return the number of contractors
   size_t poolSize() const;

   /// @return the pool
   SharedContractorPool getPool() const;

   /// Sets the pool
   /// @param pool new pool of contractors   
   void setPool(SharedContractorPool pool);

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& box) override;
   void print(std::ostream& os) const override;
   ///@}

   /// @return the tolerance used as stopping criterion
   Tolerance getTol() const;

   /// Sets the tolerance used as stopping criterion
   /// @param tol new value of the tolerance
   void setTol(Tolerance tol);

   /// @return the maximum number of propagation steps
   size_t getMaxIter() const;

   /// Sets the maximum number of propagation steps
   /// @param n new value
   void setMaxIter(size_t n);

   /// Gets a proof certificate after a contraction
   /// @param i an index of a contractor between 0 and getNbContractors()
   /// @return the proof returned by the i-th contractor of this
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
};

/// Type of shared pointers on propagators
typedef std::shared_ptr<Propagator> SharedPropagator;

} // namespace

#endif
