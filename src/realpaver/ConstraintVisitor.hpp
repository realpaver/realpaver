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
 * @file   ConstraintVisitor.hpp
 * @brief  Base class of constraint visitors
 * @author Laurent Granvilliers
 * @date   28 May 2025
 */

#ifndef REALPAVER_CONSTRAINT_VISITOR_HPP
#define REALPAVER_CONSTRAINT_VISITOR_HPP

namespace realpaver {

class ConstraintCond;
class ConstraintTable;
class ConstraintPiecewise;
class ConstraintEq;
class ConstraintLe;
class ConstraintLt;
class ConstraintGe;
class ConstraintGt;
class ConstraintIn;

/// Base class of visitors of constraint representations
class ConstraintVisitor {
public:
   /// Virtual destructor
   virtual ~ConstraintVisitor();

   /// @name Visit methods
   ///@{
   virtual void apply(const ConstraintCond *c);
   virtual void apply(const ConstraintTable *c);
   virtual void apply(const ConstraintPiecewise *c);
   virtual void apply(const ConstraintEq *c);
   virtual void apply(const ConstraintLe *c);
   virtual void apply(const ConstraintLt *c);
   virtual void apply(const ConstraintGe *c);
   virtual void apply(const ConstraintGt *c);
   virtual void apply(const ConstraintIn *c);
   ///@}
};

} // namespace realpaver

#endif
