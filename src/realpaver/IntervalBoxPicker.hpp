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
 * @file   IntervalBoxPicker.hpp
 * @brief  Base class of pickers on interval boxes
 * @author Laurent Granvilliers
 * @date   13 May 2026
 */

#ifndef REALPAVER_INTERVAL_BOX_PICKER_HPP
#define REALPAVER_INTERVAL_BOX_PICKER_HPP

#include "realpaver/IntervalBox.hpp"
#include "realpaver/RealPoint.hpp"

namespace realpaver {

/// A picker chooses a point in a box.
class IntervalBoxPicker {
public:
   /// Default constructor
   IntervalBoxPicker() = default;

   /// Virtual destructor
   virtual ~IntervalBoxPicker();

   // Default copy constructor
   IntervalBoxPicker(const IntervalBoxPicker &) = default;

   /// No assignment
   IntervalBoxPicker &operator=(const IntervalBoxPicker &) = delete;

   /**
    * @brief Selection method.
    *
    * It selects the point pt within the box. The scope of the box must
    * contain the scope of the point.
    *
    * The default implementation calculates the midpoint.
    */
   virtual void apply(const IntervalBox &B, RealPoint &pt);

   /**
    * @brief Selection method on a scope.
    *
    * It selects the point pt within the box on the given scope.
    *
    * The default implementation calculates the midpoint.
    */
   virtual void applyOnScope(const IntervalBox &B, RealPoint &pt, const Scope &scop);
};

} // namespace realpaver

#endif
