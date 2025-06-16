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
 * @date   11 Fev 2025
 */

#ifndef REALPAVER_LINEARIZER_HPP
#define REALPAVER_LINEARIZER_HPP

#include "realpaver/Dag.hpp"
#include "realpaver/LPModel.hpp"

namespace realpaver {

/// Base class of linearizers of nonlinear problems
class Linearizer {
public:
   /// Type of lists of indexes
   using IndexList = std::vector<size_t>;

   /// Creates a linearizer for a DAG
   Linearizer(SharedDag dag);

   /// Creates a linearizer for a subset of a DAG given by a list of indexes
   Linearizer(SharedDag dag, const IndexList &lfun);

   /// Destructor
   virtual ~Linearizer();

   /// No copy
   Linearizer(const Linearizer &) = delete;

   /// No assignment
   Linearizer &operator=(const Linearizer &) = delete;

   /// Returns the dag of this
   SharedDag dag() const;

   /// Returns the scope of this
   Scope scope() const;

   /**
    * @brief Makes the linear relaxation.
    *
    * It makes a linear relaxation of the DAG (or a subset) on
    * the given box B. The new linear program is assigned to lpm
    * and it returns true in case of success, false otherwise.
    */
   virtual bool make(LPModel &lpm, const IntervalBox &B) = 0;

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
   SharedDag dag_;  // DAG
   Scope scop_;     // scope
   IndexList lfun_; // list of indexes of functions
   double tol_;     // relaxation tol for the equations

private:
   using MapType = std::unordered_map<size_t, size_t>;
   MapType mvv_; // map var index -> lin var index
};

} // namespace realpaver

#endif
