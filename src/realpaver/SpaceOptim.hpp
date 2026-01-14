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
 * @file   SpaceOptim.hpp
 * @brief  Base class of search spaces of branch-and-bound algorithms
 * @author Laurent Granvilliers
 * @date   1 July 2025
 */

#ifndef REALPAVER_SPACE_OPTIM_HPP
#define REALPAVER_SPACE_OPTIM_HPP

#include "realpaver/NodeOptim.hpp"
#include "realpaver/Space.hpp"
#include "realpaver/Tolerance.hpp"
#include <set>

namespace realpaver {

/**
 * @brief Base class of search spaces for branch-and-bound algorithms
 *
 * Solution nodes and a tolerance on the optimum are managed in this class. The solution
 * nodes are sorted according to an increasing ordering of the lower bounds oh the
 * optimum.
 *
 * It is assumed that the problem is a minimization problem. This assumption is used
 * when a space is simplified given an upper bound of the optimal value,
 * i.e. a minimum.
 */
class SpaceOptim : public Space<NodeOptim> {
public:
   /// Creates an empty space
   SpaceOptim();

   /// No copy
   SpaceOptim(const SpaceOptim &) = delete;

   /// No assignment
   SpaceOptim &operator=(const SpaceOptim &) = delete;

   /// Virtual destructor
   virtual ~SpaceOptim();

   /// Simplifies this given an upper bound of the minimum
   void simplify(double u);

   /// Returns the tolerance on the enclosure of the optimum
   Tolerance getTol() const;

   /// Assigns the tolerance on the enclosure of the optimum
   void setTol(const Tolerance &tol);

   size_t nbSolNodes() const override;
   SharedNodeOptim getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;

   virtual void pushSolNode(const SharedNodeOptim &node) override;
   virtual SharedNodeOptim popSolNode() override;

   /**
    * @brief Access to the lowest lower bound.
    *
    * Returns the lowest lower bound among the pending nodes and the solution nodes,
    * +oo if this is empty
    */
   virtual double getLowestLowerBound() const = 0;

   /// Output on a stream
   virtual void print(std::ostream &os) const = 0;

protected:
   // Simplifies this given an upper bound of the minimum
   virtual void simplifyImpl(double u) = 0;

   // Comparator of nodes by decreasing ordering of the lower bounds
   struct SolCmp {
      bool operator()(const SharedNodeOptim &x, const SharedNodeOptim &y) const
      {
         return (x->getLower() < y->getLower());
      }
   };

   std::multiset<SharedNodeOptim, SolCmp> sol_; // set of solution nodes
   Tolerance tol_;                              // tolerance on the optimum
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const SpaceOptim &space);

} // namespace realpaver

#endif
