///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_HC4_HPP
#define REALPAVER_CONTRACTOR_HC4_HPP

#include "realpaver/ContractorHC4Revise.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/PropagationAlg.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a constraint propagation algorithm applying HC4Revise contractors
/// on a DAG.
///////////////////////////////////////////////////////////////////////////////
class ContractorHC4 : public Contractor {
public:
   /// Constructor
   /// @param dag a shared dag
   ContractorHC4(SharedDag dag);

   /// Destructor
   ~ContractorHC4();

   /// No copy
   ContractorHC4(const ContractorHC4&) = delete;

   /// No asignment
   ContractorHC4& operator=(const ContractorHC4&) = delete;

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

private:
   SharedDag dag_;
   DagContext* context_;
   PropagationAlg* propag_;

   //////////
   class SharedHC4Revise : public Contractor {
   public:
      SharedHC4Revise(SharedDag dag, size_t i);

      ///@{
      Scope scope() const override;
      Proof contract(IntervalBox& B) override;
      void print(std::ostream& os) const override;
      ///@}

   private:
      SharedDag dag_;
      size_t idx_;
   };
};

/// Type of shared pointers of HC4 contractors
typedef std::shared_ptr<ContractorHC4> SharedContractorHC4;

} // namespace

#endif
