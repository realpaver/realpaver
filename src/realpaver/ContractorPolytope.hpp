/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   ContractorPolytope.hpp
 * @brief  Polytope Hull contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CONTRACTOR_POLYTOPE_HPP
#define REALPAVER_CONTRACTOR_POLYTOPE_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include "realpaver/Contractor.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/LPSolver.hpp"

namespace realpaver {

/// Types of linearizations
enum class PolytopeStyle { RLT, Affine, Taylor };

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const PolytopeStyle& style);

/// Base class of polytope makers used to linearize nonlinear systems
class PolytopeCreator {
public:
   /// Type of lists of indexes
   using IndexList = std::vector<size_t>;

   /// Creates a creator for a DAG
   PolytopeCreator(SharedDag dag);

   /// Creates a creator for a subset of a DAG given by a list of indexes
   PolytopeCreator(SharedDag dag, const IndexList& lfun);

   /// Destructor
   virtual ~PolytopeCreator();

   /// @return the dag of this
   SharedDag dag() const;

   /// @return the scope of this
   Scope scope() const;

   /**
    * @brief Makes the linear relaxation.
    * 
    * @param lpm resulting linear model
    * @param B input box
    * @return true in case of sucess, false otherwise
    */
   virtual bool make(LPModel& lpm, const IntervalBox& B) = 0;

   /// Returns the index of the linear variable associated with v
   size_t linVarIndex(Variable v) const;

   /// Returns the relaxation tolerance for the equations
   double getRelaxEqTol() const;

   /// Assigns the relaxation tolerance for the equations
   void setRelaxEqTol(double tol);

protected:
   SharedDag dag_;                           // DAG
   Scope scop_;                              // scope
   std::unordered_map<size_t, size_t> mvv_;  // map var index ->lin var index
   IndexList lfun_;                          // list of indexes of functions
   double eqtol_;                            // relaxation tol for the equations
};

/*----------------------------------------------------------------------------*/

/// Polytope maker implementing the Reformulation-Linearization-Technique (RLT)
class PolytopeRLTCreator : public PolytopeCreator {
public:
   /// Creates a creator for a DAG
   PolytopeRLTCreator(SharedDag dag);

   /// Creates a creator for a subset of a DAG given by a list of indexes
   PolytopeRLTCreator(SharedDag dag, const IndexList& lfun);

   bool make(LPModel& lpm, const IntervalBox& B) override;

private:
   size_t nodeToLinVar(DagNode* node) const;
   void createLinVar(LPModel& lpm, DagNode* node);

   std::unordered_map<size_t, size_t > mnv_; // node index -> lin var index
};

/*----------------------------------------------------------------------------*/

///Polytope maker that generates Taylor forms
class PolytopeTaylorCreator : public PolytopeCreator {
public:
   /// Creates a creator for a DAG
   PolytopeTaylorCreator(SharedDag dag);

   /// Creates a creator for a subset of the  DAG given by a list of indexes
   PolytopeTaylorCreator(SharedDag dag, const IndexList& lfun);

   bool make(LPModel& lpm, const IntervalBox& B) override;

private:
   Bitset corner_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Polytope Hull contractor.
 * 
 * This is a contractor based on linear relaxations of constraint systems,
 * which is parameterized by the relaxation method.
 * 
 * Given a constraint system S, and a region R, it generates an outer
 * approximation A of the set of solutions to S in R defined by a polytope.
 * For each variable x, two LPs are solved: min or max x s.t. A in
 * order to contract the domain of x in R.
 */
class ContractorPolytope : public Contractor {
public:
   /// Creates a contractor for a DAG
   ContractorPolytope(SharedDag dag, PolytopeStyle style);

   /// Creates a creator for a subset of a  DAG
   ContractorPolytope(SharedDag dag, const IndexList& lfun,
                      PolytopeStyle style);

   /// Destructor
   ~ContractorPolytope();

   /// No copy
   ContractorPolytope(const ContractorPolytope&) = delete;

   /// No assignment
   ContractorPolytope& operator=(const ContractorPolytope&) = delete;

   /// Returns the relaxation tolerance for the equations
   double getRelaxEqTol() const;

   /// Assigns the relaxation tolerance for the equations
   void setRelaxEqTol(double tol);

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

private:
   PolytopeCreator* creator_;    // creator of the relaxation

   Proof contractImpl(IntervalBox& B);
};

/// type of shared contractors
using SharedContractorPolytope = std::shared_ptr<ContractorPolytope>;

} // namespace

#endif
