///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_HC4_CONTRACTOR_HPP
#define REALPAVER_HC4_CONTRACTOR_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/Dag.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This implements the HC4Revise operator.
///
/// This contractor traverses the tree-representation of a constraints.
/// The first phase is an interval evaluation from the leaves to the root.
/// The second phase calculates the projections from the root to the leaves.
///
/// The expression of the constraint comes from a DAG.
///////////////////////////////////////////////////////////////////////////////
class HC4Contractor : public Contractor {
public:
   /// Creates a contractor
   /// @param dag a DAG
   /// @param i index of a function / constraint in the dag
   ///
   /// We have 0 <= i < dag->nbFuns().
   /// This does not own the dag.
   HC4Contractor(Dag* dag, size_t i);

   /// Default copy constructor
   HC4Contractor(const HC4Contractor&) = default;

   /// Default assignment operator
   HC4Contractor& operator=(const HC4Contractor&) = default;

   /// Default destructor
   ~HC4Contractor() = default;

   ///@{
   /// Overrides (Contractor)
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalRegion& reg);
   void print(std::ostream& os) const;
   ///@}

   /// @return the function enclosed
   DagFun* getFun() const;

   /// @return the dag
   Dag* getDag() const;

private:
   DagFun* f_;
};

} // namespace

#endif
