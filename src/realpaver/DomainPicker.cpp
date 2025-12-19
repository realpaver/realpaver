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
 * @file   DomainPicker.cpp
 * @brief  A picker picks a point in a box.
 * @author Laurent Granvilliers
 * @date   14 Oct 2025
 */

#include "realpaver/DomainPicker.hpp"
#include "realpaver/Double.hpp"

namespace realpaver {

DomainPicker::DomainPicker()
    : DomainVisitor()
    , val_(Double::inf())
{
}

double DomainPicker::getValue() const
{
   return val_;
}

void DomainPicker::apply(const IntervalDomain *d)
{
   val_ = d->getVal().midpoint();
}

void DomainPicker::apply(const IntervalUnionDomain *d)
{
   val_ = d->getVal()[0].midpoint();
}

void DomainPicker::apply(const RangeDomain *d)
{
   val_ = d->getVal().left().get();
}

void DomainPicker::apply(const RangeUnionDomain *d)
{
   val_ = d->getVal()[0].left().get();
}

void DomainPicker::apply(const BinaryDomain *d)
{
   val_ = d->getVal().hasZero() ? 0.0 : 1.0;
}

} // namespace realpaver
