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
 * @file   NodeOptim.cpp
 * @brief  Node of search tree of branch-and-bound algorithms
 * @author Laurent Granvilliers
 * @date   1 July 2025
 */

#include "realpaver/Double.hpp"
#include "realpaver/NodeOptim.hpp"

namespace realpaver {

NodeOptim::NodeOptim(Scope scop, int nbc, int depth)
    : Node(scop, nbc, depth)
    , lo_(Double::neginf())
    , up_(Double::inf())
{
}

NodeOptim::NodeOptim(std::unique_ptr<DomainBox> box, int nbc, int depth)
    : Node(std::move(box), nbc, depth)
    , lo_(Double::neginf())
    , up_(Double::inf())
{
}

double NodeOptim::getLower() const
{
   return lo_;
}

NodeOptim &NodeOptim::setLower(double val)
{
   lo_ = val;
   return *this;
}

double NodeOptim::getUpper() const
{
   return up_;
}

NodeOptim &NodeOptim::setUpper(double val)
{
   up_ = val;
   return *this;
}

void NodeOptim::print(std::ostream &os) const
{
   os << index() << ": " << (*box()) << " " << Interval(lo_, up_);
}

NodeOptim *NodeOptim::clone() const
{
   return new NodeOptim(*this);
}

} // namespace realpaver
