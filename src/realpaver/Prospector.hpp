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
 * @file   Prospector.hpp
 * @brief  Base class of prospectors
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#ifndef REALPAVER_PROSPECTOR_HPP
#define REALPAVER_PROSPECTOR_HPP

#include "realpaver/Node.hpp"
#include "realpaver/NodeContext.hpp"
#include "realpaver/ProverOptim.hpp"
#include "realpaver/RealPoint.hpp"

namespace realpaver {

/**
 * @brief Base class of prospectors.
 *
 * A prospector is used to find feasible points, defined by a real point that certainly
 * satisfies all the active constraints in a node.
 */
class Prospector {
public:
   /// Constructor
   Prospector(std::unique_ptr<ProverOptim> prover);

   /// No copy constructor
   Prospector(const Prospector &) = delete;

   /// No assignment
   Prospector &operator=(const Prospector &) = delete;

   /// Virtual destructor
   virtual ~Prospector();

   /**
    * @brief Exploration method.
    *
    * Explores the node in order to find a feasible point. Returns true in case of
    * success, false otherwise.
    */
   bool explore(Node &node, NodeContext &ctx);

   /// Gets the feasible point after a successfull exploration
   const RealPoint &feasiblePoint() const;

   /**
    * @brief Exploration method called by explore, to be overriden in sub-classes.
    *
    * Returns true and assigns the point in case of success.
    */
   virtual bool exploreImpl(Node &node, RealPoint &pt) = 0;

private:
   std::unique_ptr<ProverOptim> prover_;
   RealPoint *pt_; // feasible point if found
};

} // namespace realpaver

#endif
