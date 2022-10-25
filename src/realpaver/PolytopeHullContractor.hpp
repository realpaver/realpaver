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

/// Types of linearizations
enum class PolytopeCreatorStyle { RLT, Affine, Taylor };

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const PolytopeCreatorStyle& style);

///////////////////////////////////////////////////////////////////////////////
/// This the abstract class of polytope makers used to linearize nonlinear
/// systems.
///////////////////////////////////////////////////////////////////////////////
class PolytopeCreator {
public:
   /// Constructor
   /// @param dag dag-representation of a nonlinear system
   /// @param scope set of variables of the system
   PolytopeCreator(SharedDag dag, Scope scope);

   /// Destructor
   virtual ~PolytopeCreator();

   /// @return the dag of this
   SharedDag dag() const;

   /// @return the scope of this
   Scope scope() const;

   /// Tests if this depends on a variable referenced in a bitset
   /// @param bs a bitset
   /// @return the result of the test
   bool dependsOn(const Bitset& bs) const;

   /// Makes the linear relaxation
   /// @param lpm resulting linear model
   /// @param reg input region
   virtual void make(LPModel& lpm, const IntervalRegion& reg) = 0;

protected:
   SharedDag dag_;
   Scope scope_;
   Bitset bs_;     // bitset view of the scope
};

///////////////////////////////////////////////////////////////////////////////
/// This is a polytope maker implementing the Reformulation-Linearization-
/// Technique (RLT).
///////////////////////////////////////////////////////////////////////////////
class PolytopeRLTCreator : public PolytopeCreator {
public:
   /// Constructor
   /// @param dag dag-representation of a nonlinear system
   /// @param scope set of variables of the system
   PolytopeRLTCreator(SharedDag dag, Scope scope);

   /// Overrides
   void make(LPModel& lpm, const IntervalRegion& reg);
};

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor based on linear relaxations of constraint systems.
///
/// Given a constraint system S, and a region R, it generates an outer
/// approximation A of the set of solutions to S in R defined by a polytope.
/// Now, for each variable x, two LPs are solved: min or max x s.t. A in
/// order to contract of the domain of x in R.
///
/// This contractor is parameterized by the relaxation method.
///////////////////////////////////////////////////////////////////////////////
class PolytopeHullContractor : public Contractor {
public:
   /// Constructor
   /// @param dag a DAG representing a set of constraints
   /// @param sco set of variables to be contracted by this
   /// @param style kind of relaxation method
   PolytopeHullContractor(SharedDag dag, Scope sco, PolytopeCreatorStyle style);

   /// Destructor
   ~PolytopeHullContractor();

   ///@{
   /// Overrides
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalRegion& reg);
   void print(std::ostream& os) const;
   ///@}

private:
   PolytopeCreator* creator_;
};

} // namespace

#endif
