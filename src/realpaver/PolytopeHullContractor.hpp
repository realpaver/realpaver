///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_POLYTOPE_HULL_CONTRACTOR_HPP
#define REALPAVER_POLYTOPE_HULL_CONTRACTOR_HPP

#include <vector>
#include <unordered_map>
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
   typedef std::vector<size_t> IndexList;

   /// Creates a creator for a whole DAG
   /// @param dag dag-representation of a nonlinear system
   PolytopeCreator(SharedDag dag);

   /// Creates a creator for a subset of the  DAG
   /// @param dag dag-representation of a nonlinear system
   /// @param lfun list of indexes of the DAG functions to be relaxed
   PolytopeCreator(SharedDag dag, const IndexList& lfun);

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
   /// @return true in case of sucess, false otherwise
   virtual bool make(LPModel& lpm, const IntervalRegion& reg) = 0;

   /// @param v a variable
   /// @return the index of the linear variable associated with v
   size_t linVarIndex(Variable v) const;

   /// @return the relaxation tolerance for the equations
   double getRelaxEqTol() const;

   /// Assigns the relaxation tolerance for the equations
   /// @param tol a positive real number
   void setRelaxEqTol(double tol);

protected:
   SharedDag dag_;                           // DAG
   Scope scope_;                             // scope
   Bitset bs_;                               // bitset view of the scope
   std::unordered_map<size_t, size_t> mvv_;  // map var index ->lin var index
   IndexList lfun_;                          // list of indexes of functions
   double eqtol_;                            // relaxation tol for the equations
};

///////////////////////////////////////////////////////////////////////////////
/// This is a polytope maker implementing the Reformulation-Linearization-
/// Technique (RLT).
///////////////////////////////////////////////////////////////////////////////
class PolytopeRLTCreator : public PolytopeCreator {
public:
   /// Creates a creator for a whole DAG
   /// @param dag dag-representation of a nonlinear system
   PolytopeRLTCreator(SharedDag dag);

   /// Creates a creator for a part of a DAG
   /// @param dag dag-representation of a nonlinear system
   /// @param lfun list of indexes of the DAG functions to be relaxed
   PolytopeRLTCreator(SharedDag dag, const IndexList& lfun);

   bool make(LPModel& lpm, const IntervalRegion& reg) override;

private:
   size_t nodeToLinVar(DagNode* node) const;
   void createLinVar(LPModel& lpm, DagNode* node);

   std::unordered_map<size_t, size_t > mnv_; // node index -> lin var index
};

///////////////////////////////////////////////////////////////////////////////
/// This is a polytope maker that generates Taylor forms.
///////////////////////////////////////////////////////////////////////////////
class PolytopeTaylorCreator : public PolytopeCreator {
public:
   /// Creates a creator for a whole DAG
   /// @param dag dag-representation of a nonlinear system
   PolytopeTaylorCreator(SharedDag dag);

   /// Creates a creator for a part of a DAG
   /// @param dag dag-representation of a nonlinear system
   /// @param lfun list of indexes of the DAG functions to be relaxed
   /// @param corner 
   PolytopeTaylorCreator(SharedDag dag, const IndexList& lfun);

   bool make(LPModel& lpm, const IntervalRegion& reg) override;

private:
   Bitset corner_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor based on linear relaxations of constraint systems.
///
/// Given a constraint system S, and a region R, it generates an outer
/// approximation A of the set of solutions to S in R defined by a polytope.
/// For each variable x, two LPs are solved: min or max x s.t. A in
/// order to contract the domain of x in R.
///
/// This contractor is parameterized by the relaxation method.
///////////////////////////////////////////////////////////////////////////////
class PolytopeHullContractor : public Contractor {
public:
   /// Constructor
   /// @param dag a DAG representing a set of constraints
   /// @param style kind of relaxation method
   PolytopeHullContractor(SharedDag dag, PolytopeCreatorStyle style);

   /// Constructor
   /// @param dag a DAG representing a set of constraints
   /// @param lfun list of indexes of the DAG functions to be relaxed
   /// @param style kind of relaxation method
   PolytopeHullContractor(SharedDag dag, const IndexList& lfun,
                          PolytopeCreatorStyle style);

   /// Destructor
   ~PolytopeHullContractor();

   /// No copy
   PolytopeHullContractor(const PolytopeHullContractor&) = delete;

   /// No assignment
   PolytopeHullContractor& operator=(const PolytopeHullContractor&) = delete;

   /// @return the relaxation tolerance for the equations
   double getRelaxEqTol() const;

   /// Assigns the relaxation tolerance for the equations
   /// @param tol a positive real number
   void setRelaxEqTol(double tol);

   bool dependsOn(const Bitset& bs) const override;
   Scope scope() const override;
   Proof contract(IntervalRegion& reg) override;
   void print(std::ostream& os) const override;

private:
   PolytopeCreator* creator_;    // creator of the relaxation

   Proof contractImpl(IntervalRegion& reg);
};

} // namespace

#endif
