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
 * @file   ConstraintVisitor.cpp
 * @brief  Base class of constraint visitors
 * @author Laurent Granvilliers
 * @date   28 May 2025
 */

#include "realpaver/ConstraintVisitor.hpp"
#include "realpaver/Exception.hpp"

namespace realpaver {

ConstraintVisitor::~ConstraintVisitor()
{
}

void ConstraintVisitor::apply(const ConstraintCond *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintTable *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintPiecewise *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintEq *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintLe *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintLt *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintGe *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintGt *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintIn *c)
{
   THROW("Visit method not implemented");
}

} // namespace realpaver
