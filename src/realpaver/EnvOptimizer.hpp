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
 * @file   EnvOptimizer.hpp
 * @brief  Environment for optimizers
 * @author Laurent Granvilliers
 * @date   6 oct 2025
 */

#ifndef REALPAVER_ENV_OPTIMIZER_HPP
#define REALPAVER_ENV_OPTIMIZER_HPP

#include "realpaver/EnvSolver.hpp"

namespace realpaver {

class EnvOptimizer : public EnvSolver {
public:
   /// Constructor
   EnvOptimizer();

   /// Default destructor
   ~EnvOptimizer() = default;

   /// Default copy constructor
   EnvOptimizer(const EnvOptimizer &) = default;

   /// No assignment
   EnvOptimizer &operator=(const EnvOptimizer &) = delete;
};

} // namespace realpaver

#endif
