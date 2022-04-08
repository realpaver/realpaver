///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_POLYTOPE_HULL_CONTRACTOR_HPP
#define REALPAVER_POLYTOPE_HULL_CONTRACTOR_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/LPSolver.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor based on linear relaxations of constraint systems.
///
/// Given a constraint system S, and a region R, it generates an outer
/// approximation A of the set of solutions to S in R defined by a polytope.
/// Now, for each variable x, two LPs are solved: min or max x s.t. A in
/// order to contract of the domain of x in R.
///////////////////////////////////////////////////////////////////////////////
class PolytopeHullContractor : public Contractor {
public:
   /// Constructor
   /// @param dag a DAG representing a set of constraints
   /// @param sco set of variables to be contracted by this
   PolytopeHullContractor(Dag* dag, const Scope& sco);

   ///@{
   /// Overrides
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalRegion& reg);
   void print(std::ostream& os) const;
   ///@}

private:
   Dag* dag_;
   Scope sco_;    // set of variables to be contracted
   Bitset bs_;    // bitset view of the scope
};

} // namespace

#endif
