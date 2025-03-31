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
 * @file   CSPEnv.cpp
 * @brief  Environment for CSP solver
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/CSPEnv.hpp"

namespace realpaver {

CSPEnv::CSPEnv()
    : Env()
    , slim_(false)
    , dlim_(false)
{
}

void CSPEnv::setSolutionLimit(bool b)
{
   slim_ = b;
}

bool CSPEnv::usedSolutionLimit() const
{
   return slim_;
}

void CSPEnv::setDepthLimit(bool b)
{
   dlim_ = true;
}

bool CSPEnv::usedDepthLimit() const
{
   return dlim_;
}

bool CSPEnv::usedNoLimit() const
{
   return !(usedTimeLimit() || usedNodeLimit() || usedSolutionLimit() ||
            usedDepthLimit());
}

} // namespace realpaver
