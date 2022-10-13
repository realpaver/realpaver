///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BC4_CONTRACTOR_HPP
#define REALPAVER_BC4_CONTRACTOR_HPP

#include <vector>
#include "realpaver/BC3Contractor.hpp"
#include "realpaver/HC4Contractor.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the BC4Revise contractor.
///
/// Given a <= f(x1, ..., xn) <= n, it applied first an HC4 contractor. Then
/// it applies a BC3 contractor for each variable with multiple occurrences
/// in f, this contractor processing the corresponding thick interval function.
///////////////////////////////////////////////////////////////////////////////
class BC4Contractor : public Contractor {
public:
   /// Creates a contractor
   /// @param dag a DAG
   /// @param i index of a function in the DAG
   ///
   /// This contractor does not own the dag.
   BC4Contractor(SharedDag dag, size_t i);

   /// Destructor
   ~BC4Contractor();

   /// No copy
   BC4Contractor(const BC4Contractor&) = delete;

   /// No assignment
   BC4Contractor& operator=(const BC4Contractor&) = delete;

   ///@{
   /// Overrides (Contractor)
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalRegion& reg);
   void print(std::ostream& os) const;
   ///@}

private:
   SharedDag dag_;                     // a dag
   size_t if_;                         // index of a function f in the dag
   HC4Contractor* hc4_;                // hc4 contractor associated with f
   std::vector<BC3Contractor*> bc3_;   // bc3 contractors associated with
                                       // the variables having multiple
                                       // occurrences in f
};

} // namespace

#endif
