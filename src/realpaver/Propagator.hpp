///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
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
   ///
   /// This propagator does not own the pool.
   Propagator(ContractorPool* pool = nullptr);

   /// Default copy constructor
   Propagator(const Propagator&) = default;

   /// No assignment
   Propagator& operator=(const Propagator&) = delete;

   /// Default destructor
   ~Propagator() = default;

   /// @return the number of contractors
   size_t poolSize() const;

   /// @return the pool
   ContractorPool* getPool() const;

   /// Sets the pool
   /// @param pool new pool of contractors   
   void setPool(ContractorPool* pool);

   ///@{
   /// Overrides the methods of Contractor
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalVector& X);
   void print(std::ostream& os) const;
   ///@}

   /// @return the tolerance used as stopping criterion
   Tolerance getDistTol() const;

   /// Sets the tolerance used as stopping criterion
   /// @param tol new value of the tolerance
   void setDistTol(Tolerance tol);

   /// @return the maximum number of propagation steps
   size_t getMaxIterations() const;

   /// Sets the maximum number of propagation steps
   /// @param n new value
   void setMaxIterations(size_t n);

   /// Gets a proof certificate after a contraction
   /// @param i an index of a contractor between 0 and getNbContractors()
   /// @return the proof returned by the i-th contractor of this
   Proof proofAt(size_t i) const;

private:
   ContractorPool* pool_;        // pool of contractors
   Tolerance dtol_;              // tolerance used for propagation
   size_t maxiter_;              // maximum number of propagation steps
   std::vector<Proof> certif_;   // proof certificates of contractors
};

} // namespace

#endif