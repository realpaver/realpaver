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
 * @file   DomainBoxPicker.hpp
 * @brief  Base class of pickers on domain boxes
 * @author Laurent Granvilliers
 * @date   14 Oct 2025
 */

#ifndef REALPAVER_DOMAIN_BOX_PICKER_HPP
#define REALPAVER_DOMAIN_BOX_PICKER_HPP

#include "realpaver/DomainBox.hpp"
#include "realpaver/RealPoint.hpp"

namespace realpaver {

/// A picker chooses a point in a box.
class DomainBoxPicker {
public:
   /// Default constructor
   DomainBoxPicker() = default;

   /// Virtual destructor
   virtual ~DomainBoxPicker();

   // Default copy constructor
   DomainBoxPicker(const DomainBoxPicker &) = default;

   /// No assignment
   DomainBoxPicker &operator=(const DomainBoxPicker &) = delete;

   /**
    * @brief Selection method.
    *
    * It selects the point pt within the box. The scope of the box must
    * contain the scope of the point.
    */
   virtual void apply(const DomainBox &box, RealPoint &pt);
};

} // namespace realpaver

#endif
