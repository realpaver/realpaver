///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_BC4_REVISE_HPP
#define REALPAVER_CONTRACTOR_BC4_REVISE_HPP

#include <vector>
#include "realpaver/ContractorBC3Revise.hpp"
#include "realpaver/ContractorHC4Revise.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the BC4Revise contractor.
///
/// Given a constraint a <= f(x1, ..., xn) <= b, it applies first an HC4
/// contractor. Then it applies a BC3 contractor for each variable with multiple
/// occurrences in f.
///////////////////////////////////////////////////////////////////////////////
class ContractorBC4Revise : public Contractor {
public:
   /// Creates a contractor
   /// @param dag a DAG
   /// @param i index of a function in the DAG
   ContractorBC4Revise(SharedDag dag, size_t i);

   /// Destructor
   ~ContractorBC4Revise();

   /// No copy
   ContractorBC4Revise(const ContractorBC4Revise&) = delete;

   /// No assignment
   ContractorBC4Revise& operator=(const ContractorBC4Revise&) = delete;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   SharedDag dag_;                     // a dag
   size_t if_;                         // index of a function f in the dag
   ContractorHC4Revise* hc4_;                // hc4 contractor associated with f
   std::vector<ContractorBC3Revise*> bc3_;   // bc3 contractors associated with
                                             // the variables having multiple
                                       // occurrences in f
};

} // namespace

#endif
