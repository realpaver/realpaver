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
 * @file   Env.hpp
 * @brief  Base class of environments for solvers
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_ENV_HPP
#define REALPAVER_ENV_HPP

#include "realpaver/Param.hpp"

namespace realpaver {

/**
 * @brief Base class of environments for solvers.
 *
 * An environment manages a set of parameters, limits used to stop the solving,
 * and data from the solving process.
 */
class Env {
public:
   /// Constructor
   Env();

   /// Destructor
   virtual ~Env();

   /// Copy constructor
   Env(const Env &e);

   /// No assignment
   Env &operator=(const Env &) = delete;

   /// Sets the object that manages the parameter settings
   void setParams(const Params &params);

   /// Returns the object that manages the parameter settings
   Params *getParams() const;

   /// Sets a flag that informs on the termination of a solving process
   void setTimeLimit(bool b = true);

   /// Returns true if a solving process is aborted due a time limit
   bool usedTimeLimit() const;

   /// Sets a flag that informs on the termination of a solving process
   void setNodeLimit(bool b = true);

   /// Returns true if a solving process is aborted due a node limit
   bool usedNodeLimit() const;

private:
   Params *params_; // management of parameters
   bool tlim_;      // time limit reached
   bool nlim_;      // node limit reached
};

} // namespace realpaver

#endif
