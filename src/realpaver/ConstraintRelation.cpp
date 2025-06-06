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
 * @file   ConstraintRelation.cpp
 * @brief  Constraint relations
 * @author Laurent Granvilliers
 * @date   28 may 2025
 */

#include "realpaver/ConstraintRelation.hpp"

namespace realpaver {

std::ostream &operator<<(std::ostream &os, const ConstraintRelation &rel)
{
   switch (rel)
   {
   case ConstraintRelation::Eq:
      return os << "==";
   case ConstraintRelation::Le:
      return os << "<=";
   case ConstraintRelation::Lt:
      return os << "<";
   case ConstraintRelation::Ge:
      return os << ">=";
   case ConstraintRelation::Gt:
      return os << ">";
   case ConstraintRelation::In:
      return os << "in";
   case ConstraintRelation::Cond:
      return os << "cond";
   case ConstraintRelation::Piecewise:
      return os << "piecewise";
   case ConstraintRelation::Table:
      return os << "table";
   default:
      os.setstate(std::ios::failbit);
   }
   return os;
}

} // namespace realpaver
