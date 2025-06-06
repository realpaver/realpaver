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
 * @file   ConstraintRelation.hpp
 * @brief  Constraint relations
 * @author Laurent Granvilliers
 * @date   28 may 2025
 */

#ifndef REALPAVER_CONSTRAINT_RELATION_HPP
#define REALPAVER_CONSTRAINT_RELATION_HPP

#include <iostream>

namespace realpaver {

/// Enumeration of relation symbols
enum class ConstraintRelation {
   Eq,       ///< equation
   Le,       ///< less than
   Lt,       ///< strictly less than
   Ge,       ///< greater than
   Gt,       ///< stricly greater than
   In,       ///< doubly bounded inequality constraint
   Table,    ///< table constraint
   Cond,     ///< conditional constraint
   Piecewise ///< piecewise constraint
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const ConstraintRelation &rel);

} // namespace realpaver

#endif
