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
 * @file   NodeOptim.hpp
 * @brief  Node of search tree of branch-and-bound algorithms
 * @author Laurent Granvilliers
 * @date   1 July 2025
 */

#ifndef REALPAVER_NODE_OPTIM_HPP
#define REALPAVER_NODE_OPTIM_HPP

#include "realpaver/Node.hpp"

namespace realpaver {

class NodeOptim : public Node {
public:
   /**
    * @brief Creates a node from a scope.
    *
    * The variable domains are extracted from the variables in order to create a box.
    * The number of constraints nbc is used to create a bitset that stores the
    * activity status of the constraints. The depth in the search tree is also given.
    */
   NodeOptim(Scope scop, int nbc, int depth = 0);

   /**
    * @brief Creates a node from a box.
    *
    * The given box is cloned. The number of constraints nbc is used to create a bitset
    * that stores the activity status of the constraints. The depth in the search tree is
    * also given.
    */
   NodeOptim(std::unique_ptr<DomainBox> box, int nbc, int depth = 0);

   /// Default destructor
   ~NodeOptim() = default;

   /// Default copy constructor
   NodeOptim(const NodeOptim &node) = default;

   /// No assignment
   NodeOptim &operator=(const NodeOptim &) = delete;

   /// Returns the lower bound of the objective function
   double getLower() const;

   /// Assigns the lower bound of the objective function and returns *this
   NodeOptim &setLower(double val);

   /// Returns the upper bound of the objective function
   double getUpper() const;

   /// Assigns the upper bound of the objective function and returns *this
   NodeOptim &setUpper(double val);

   void print(std::ostream &os) const override;
   NodeOptim *clone() const override;

private:
   double lo_, up_; // lower and upper bound of the objective function
};

/// Type of shared pointers on nodes
using SharedNodeOptim = std::shared_ptr<NodeOptim>;

} // namespace realpaver

#endif
