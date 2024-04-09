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
 * @file   NcspEnv.cpp
 * @brief  Environment for NCSP solver
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspEnv.hpp"

namespace realpaver {

NcspEnv::NcspEnv() : Env(), slim_(false), dlim_(false)
{}

void NcspEnv::setSolutionLimit(bool b)
{
   slim_ = b;
}

bool NcspEnv::usedSolutionLimit() const
{
   return slim_;
}

void NcspEnv::setDepthLimit(bool b)
{
   dlim_ = true;
}

bool NcspEnv::usedDepthLimit() const
{
   return dlim_;
}

bool NcspEnv::usedNoLimit() const
{
   return !(usedTimeLimit() || usedNodeLimit() || usedSolutionLimit() ||
            usedDepthLimit());
}

} // namespace
