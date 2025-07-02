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
 * @file   EnvSolver.cpp
 * @brief  Environment for solvers
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/EnvSolver.hpp"

namespace realpaver {

EnvSolver::EnvSolver()
    : Env()
    , slim_(false)
    , dlim_(false)
{
}

void EnvSolver::setSolutionLimit(bool b)
{
   slim_ = b;
}

bool EnvSolver::usedSolutionLimit() const
{
   return slim_;
}

void EnvSolver::setDepthLimit(bool b)
{
   dlim_ = b;
}

bool EnvSolver::usedDepthLimit() const
{
   return dlim_;
}

bool EnvSolver::usedNoLimit() const
{
   return !(usedTimeLimit() || usedNodeLimit() || usedSolutionLimit() ||
            usedDepthLimit());
}

} // namespace realpaver
