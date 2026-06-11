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
 * @file   LinearizerInner.cpp
 * @brief  Inner linearization of nonlinear problems
 * @author Raphaël Chenouard
 * @date   11 Jun 2026
 */

#include "realpaver/LinearizerInner.hpp"

namespace realpaver {

LinearizerInner::LinearizerInner(SharedDagExtended dag)
    : Linearizer(dag)
{
}

} // namespace realpaver
