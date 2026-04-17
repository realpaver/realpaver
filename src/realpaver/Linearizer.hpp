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
 * @file   Linearizer.hpp
 * @brief  Linearization of nonlinear problems
 * @author Laurent Granvilliers
 * @date   11 Feb 2025
 */

#ifndef REALPAVER_LINEARIZER_HPP
#define REALPAVER_LINEARIZER_HPP

#include "realpaver/DagExtended.hpp"
#include "realpaver/LPModel.hpp"

namespace realpaver {

/**
 * @brief Base class of linearizers of nonlinear problems.
 *
 * A linearizer is used to linearize constraints from a DAG. Its scope corresponds
 * to the DAG's scope. A resulting linear model must involve all the variables from
 * the scope, even if some variable does not occur in active constraints.
 */
class Linearizer {
public:
   /// Type of lists of indexes
   using IndexList = std::vector<size_t>;

   /// Creates a linearizer for a DAG
   Linearizer(SharedDagExtended edag);

   /// Destructor
   virtual ~Linearizer();

   /// No copy
   Linearizer(const Linearizer &) = delete;

   /// No assignment
   Linearizer &operator=(const Linearizer &) = delete;

   /// Returns the dag of this
   SharedDagExtended edag() const;

   /// Returns the scope of this
   Scope scope() const;

   /**
    * @brief Makes the linear relaxation.
    *
    * It makes a linear relaxation of a subset of the DAG on the given box B. The subset
    * is determined by the bitset. The new linear program is assigned to lpm and it
    * returns true in case of success, false otherwise.
    *
    * Calls makeLPImpl(lpm, B, active).
    */
   bool makeLP(LPModel &lpm, const IntervalBox &B, const Bitset &active = Bitset());

   /**
    * @brief Implementation of the linear relaxation in sub-classes.
    *
    * It makes a linear relaxation of a subset of the DAG on the given box B. The subset
    * is determined by the bitset. The new linear program is assigned to lpm and it
    * returns true in case of success, false otherwise.
    *
    * There are two cases for the bitset active. If its size is empty, then all the
    * constraints are considered as active. Otherwise, it corresponds to the constraint
    * activities.
    *
    * All the variables of the scope must be involved in the linear model, even if
    * they do not occur in active constraints.
    */
   virtual bool makeLPImpl(LPModel &lpm, const IntervalBox &B, const Bitset &active) = 0;

   /// Returns the index of the linear variable associated with v
   size_t linVarIndex(Variable v) const;

   /// Returns the index of the linear variable associated with the variable id
   size_t linVarIndex(size_t id) const;

   /// Associates the linear variable of index k to the variable id
   void setLinVarIndex(size_t id, size_t k);

   /// Returns the relaxation tolerance for the equations
   double getRelaxTol() const;

   /// Assigns the relaxation tolerance for the equations
   void setRelaxTol(double tol);

protected:
   SharedDagExtended edag_; // extended DAG
   Scope scop_;             // scope
   double tol_;             // relaxation tol for the equations

private:
   using MapType = std::unordered_map<size_t, size_t>;
   MapType mvv_; // map var index -> lin var index
};

} // namespace realpaver

#endif
