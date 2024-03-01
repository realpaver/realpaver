///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_BC4_HPP
#define REALPAVER_CONTRACTOR_BC4_HPP

#include "realpaver/ContractorBC4Revise.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/PropagationAlg.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a constraint propagation algorithm applying BC4Revise contractors
/// on a DAG.
///////////////////////////////////////////////////////////////////////////////
class ContractorBC4 : public Contractor {
public:
   /// Constructor
   /// @param dag a shared dag
   ContractorBC4(SharedDag dag);

   /// Destructor
   ~ContractorBC4();

   /// No copy
   ContractorBC4(const ContractorBC4&) = delete;

   /// No asignment
   ContractorBC4& operator=(const ContractorBC4&) = delete;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;
   ///@}

   /// Inserts a contractor in this
   /// @param op a contractor
   ///
   /// This is typically used to call new contractors in the propagation
   /// loop, these ones being independent from the DAG
   void push(SharedContractor op);

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

   /// Sets the peel factor of the BC4Revise operators
   /// @param f f/100 is a percentage with f >= 0.0 and f <= 100.0
   void setBC4RevisePeelFactor(double f);

   /// Sets the maximum number of steps in the BC4Revise operators
   /// @param val new value
   void setBC4ReviseMaxIter(size_t val);

private:
   SharedDag dag_;
   PropagationAlg* propag_;
   std::vector<std::shared_ptr<ContractorBC4Revise>> vop_;
};

/// Type of shared pointers of BC4 contractors
typedef std::shared_ptr<ContractorBC4> SharedContractorBC4;

} // namespace

#endif
