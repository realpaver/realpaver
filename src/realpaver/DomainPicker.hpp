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
 * @file   DomainPicker.hpp
 * @brief  A picker picks a point in a box
 * @author Laurent Granvilliers
 * @date   14 Oct 2025
 */

#ifndef REALPAVER_DOMAIN_PICKER_HPP
#define REALPAVER_DOMAIN_PICKER_HPP

#include "Domain.hpp"

namespace realpaver {

/**
 * @brief A picker picks a point in a box.
 *
 * Given a domain d, it select a point in d as follows:
 * - the midpoint if d is an interval;
 * - the midpoint of the first element if d is an interval union;
 * - the left bound if d is a range;
 * - the left bound of the first element if d is a range union;
 * - 0 or 1 if d is a binary domain, precisely 0 if it contains 0, 1 otherwise.
 */
class DomainPicker : public DomainVisitor {
public:
   /// Constructor
   DomainPicker();

   /// Returns the value picked after a visit
   double getValue() const;

   void apply(const IntervalDomain *d) override;
   void apply(const IntervalUnionDomain *d) override;
   void apply(const RangeDomain *d) override;
   void apply(const RangeUnionDomain *d) override;
   void apply(const BinaryDomain *d) override;

private:
   double val_;
};

} // namespace realpaver

#endif
