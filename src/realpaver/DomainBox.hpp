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
 * @file   DomainBox.hpp
 * @brief  Box of domains
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_DOMAIN_BOX_HPP
#define REALPAVER_DOMAIN_BOX_HPP

#include <memory>
#include "realpaver/Domain.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

class IntervalBox;

/**
 * @brief Box of domains.
 *
 * A box associates a scope and a vector of variable domains indexed by the
 * local indexes of the variables from the scope.
 *
 * The perimeter of a box is defined from its hull as the sum of the widths
 * of its components.
 *
 * The grid perimeter of a box is defined from its hull as the sum of the
 * following quantities for each xi in Di with tolerance Ei:
 * - 0.0 if Di has tolerance Ei
 * - width(Di) / Ei (where Ei is the absolute tolerance of xi) otherwise
 *
 * The gap between two boxes is defined as the gap between their interval hulls.
 *
 * The glue method considers this and another box. For each variable v in some
 * scope, the domain of v in this is assigned to an interval domain defined as
 * the hull of this(v) and box(v).
 */
class DomainBox {
public:
   /// Constructor from a scope
   DomainBox(Scope scop);

   /// Copy constructor
   DomainBox(const DomainBox& box);

   /// Constructor from an interval box
   DomainBox(const IntervalBox& B);

   /// No asignment
   DomainBox& operator=(const DomainBox&) = delete;

   /// Destructor
   ~DomainBox();

   /// Returns the scope of this (sorted set of variables)
   Scope scope() const;

   /// Returns the number of couples (variable, domain)
   size_t size() const;

   /// Returns true if this is empty
   bool isEmpty() const;

   /// Gets the domain of v in this
   Domain* get(const Variable& v) const;

   /// Gets the i-th domain in this with 0 <= i < size()
   Domain* get(size_t i) const;

   /// Gets the i-th variable in this with 0 <= i < size()
   Variable var(size_t i) const;

   /**
    * @brief Domain assignment.
    *
    * Assigns the domain of v in this. The ownership of the domain pointer is
    * moved from p to this.
    */
   void set(const Variable& v, std::unique_ptr<Domain> p);

   /// Returns a clone of this
   DomainBox* clone() const;

   /// Tests if the domain of a variable is splitable
   bool isSplitable(const Variable& v) const;

   /// Output on a stream using a vector notation
   void print(std::ostream& os) const;

   /// Returns the width of the interval box corresponding to the hull of this
   double width() const;

   /// Returns the gap between this and box which must have the same scope
   double gap(const DomainBox& box) const;

   /// Returns the gap between this and box restricted to scop
   double gapOnScope(const DomainBox& box, const Scope& scop) const;

   /// Glues another box in this
   void glue(const DomainBox& box);

   /// Glues another box in this restricted to scop
   void glueOnScope(const DomainBox& box, const Scope& scop);

   /// Returns the perimeter of this
   double perimeter() const;

   /// Returns the perimeter of this restricted to scop
   double perimeterOnScope(const Scope& scop) const;

   /// Returns the grid perimeter of this
   double gridPerimeter() const;

   /// Returns the grid perimeter of this restricted to scop
   double gridPerimeterOnScope(const Scope& scop) const;

   /// Equality test
   bool equals(const DomainBox& box) const;

private:
   Scope scop_;                  // ordered set of variables
   std::vector<Domain*> doms_;   // domains
};

/// Output on a stream that calls box.vecPrint()
std::ostream& operator<<(std::ostream& os, const DomainBox& box);

} // namespace

#endif
