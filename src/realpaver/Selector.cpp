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
 * @file   Selector.cpp
 * @brief  Selector of variables
 * @author Laurent Granvilliers
 * @date   25 Apr 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Selector.hpp"

namespace realpaver {

Selector::Selector(Scope scop)
    : scop_(scop)
    , v_()
{
   ASSERT(!scop.isEmpty(), "Empty scope");
}

Selector::~Selector()
{
}

void Selector::setSelectedVar(Variable v)
{
   v_ = v;
}

Variable Selector::getSelectedVar() const
{
   return v_;
}

Scope Selector::scope() const
{
   return scop_;
}

} // namespace realpaver
