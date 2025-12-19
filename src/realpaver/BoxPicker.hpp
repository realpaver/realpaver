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
 * @file   BoxPicker.hpp
 * @brief  Base class of pickers
 * @author Laurent Granvilliers
 * @date   14 Oct 2025
 */

#ifndef REALPAVER_BOX_PICKER_HPP
#define REALPAVER_BOX_PICKER_HPP

#include "realpaver/DomainBox.hpp"
#include "realpaver/RealPoint.hpp"

namespace realpaver {

/// A picker chooses a point in a box.
class BoxPicker {
public:
   /// Default constructor
   BoxPicker() = default;

   /// Virtual destructor
   virtual ~BoxPicker();

   // Default copy constructor
   BoxPicker(const BoxPicker &) = default;

   /// No assignment
   BoxPicker &operator=(const BoxPicker &) = delete;

   /**
    * @brief Selection method.
    *
    * It selects the point pt within the box, both having the same scope.
    */
   virtual void apply(const DomainBox &box, RealPoint &pt);
};

} // namespace realpaver

#endif
