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
 * @date   2024-4-11
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
   void setParam(const Param &prm);

   /// Returns the object that manages the parameter settings
   Param *getParam() const;

   /// Sets a flag that informs on the termination of a solving process
   void setTimeLimit(bool b = true);

   /// Returns true if a solving process is aborted due a time limit
   bool usedTimeLimit() const;

   /// Sets a flag that informs on the termination of a solving process
   void setNodeLimit(bool b = true);

   /// Returns true if a solving process is aborted due a node limit
   bool usedNodeLimit() const;

   /// Sets a flag related to the preprocessing
   void setPresolved(bool b = false);

   /// Returns true if a problem is solved at preprocessing
   bool isPresolved() const;

   /// Sets a flag related to the preprocessing
   void setConstraintViolated(bool b = false);

   /// Returns true if a constraint is violated at preprocessing
   bool hasConstraintViolated() const;

private:
   Param *param_;   // management of parameters
   bool tlim_;      // time limit reached
   bool nlim_;      // node limit reached
   bool presolved_; // true if a problem is solved at preprocessing
   bool cviol_;     // true if a constraint is violated at preprocessing
};

} // namespace realpaver

#endif
