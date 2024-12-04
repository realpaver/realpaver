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
 * @file   IntervalBox.hpp
 * @brief  Class of interval boxes
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_INTERVAL_BOX_HPP
#define REALPAVER_INTERVAL_BOX_HPP

#include <memory>
#include "realpaver/IntervalVector.hpp"
#include "realpaver/RealPoint.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

class DomainBox;

/// Scoped interval vector
class IntervalBox : public IntervalVector {
public:
   /// @name Constructors
   ///@{

   /**
    * @brief Constructor from a scope.
    * 
    * Each variable from scop is assigned to the hull of its domain.
    */
   IntervalBox(Scope scop);

   /**
    * @brief Constructor from a scope and an interval.
    * 
    * Each variable from scop is assigned to x.
    */
   IntervalBox(Scope scop, const Interval& x);

   /**
    * @brief Constructor from a scope and an interval vector.
    * 
    * For each i, the i-th variable from scop is assigned to X[i].
    */
   IntervalBox(Scope scop, const IntervalVector& X);

   /**
    * @brief Constructor from a scope and a real vector.
    * 
    * For each i, the i-th variable from scop is assigned to X[i].
    */
   IntervalBox(Scope scop, const RealVector& X);

   /// Constructor from a point
   IntervalBox(const RealPoint& pt);

   /// Constructor of the hull of a domain box
   IntervalBox(const DomainBox& box);

   /// Default copy constructor
   IntervalBox(const IntervalBox&) = default;

   /**
    * @brief Constructor of a sub-box.
    * 
    * Assigns this to B restricted to scop
    */
   IntervalBox(const IntervalBox& B, Scope scop);
   ///@}

   /// Default assignment operator
   IntervalBox& operator=(const IntervalBox&) = default;

   /// Default destructor
   ~IntervalBox() = default;

   /// Returns the scope of this (sorted set of variables)
   Scope scope() const;

   /// Gets the domain of v
   Interval get(const Variable& v) const;

   /// Sets the domain of v to x
   void set(const Variable& v, const Interval& x);

   /// Returns the midpoint of this
   RealPoint midpoint() const;

   /// Returns the corner of this made from all the left bounds
   RealPoint lCorner() const;

   /// Returns the corner of this made from all the right bounds
   RealPoint rCorner() const;

   /**
    * @brief Gets a corner of this.
    * 
    * The bitset bs defines the corner as follows. For each i, the i-th variable
    * is assigned to the left bound in the resulting point if bs[i] is true, to
    * the right bound otherwise.
    */
   RealPoint corner(const Bitset& bs) const;

   /**
    * @brief Gets a corner of this.
    * 
    * The bitset bs defines the corner as follows. For each i, the i-th variable
    * is assigned to the left bound in the resulting point if bs[i] is false, to
    * the right bound otherwise.
    */
   RealPoint oppositeCorner(const Bitset& bs) const;

   /// @name Set containment tests
   ///@{

   /// Returns true if this contains B
   bool contains(const IntervalBox& B) const;

   /// Returns true if this strictly contains B
   bool strictlyContains(const IntervalBox& B) const;

   /// Returns true if this contains pt
   bool contains(const RealPoint& pt) const;

   /// Returns true if this strictly contains pt
   bool strictlyContains(const RealPoint& pt) const;
   ///@}

   /// Tests if two boxes overlap
   bool overlaps(const IntervalBox& B) const;

   /**
    * @brief Glues a box in this.
    * 
    * For each variable v in the scope of this, its domain in this is assigned
    * to the hull of its domain in this and its domain in B.
    */
   void glue(const IntervalBox& B);

   /**
    * @brief Glues a box in this.
    * 
    * For each variable v in scop, its domain in this is assigned to the hull
    * of its domain in this and its domain in B.
    */
   void glueOnScope(const IntervalBox& B, const Scope& scop);

   /**
    * @brief Assignment on a scope.
    * 
    * For each v in scope, this[v] is assigned to B[v].
    */
   void setOnScope(const IntervalBox& B, const Scope& scop);

   /**
    * @brief Midpoint on a scope.
    * 
    * For each v in scop, the value of v in the resulting point is assigned
    * to the midpoint of the domain of v in this.
    */
   RealPoint midpointOnScope(const Scope& scop) const;

   /**
    * @brief Midpoint on a scope.
    * 
    * For each v in scop, the value of v in pt is assigned to the midpoint of
    * the domain of v in this.
    */
   void midpointOnScope(const Scope& scop, RealPoint& pt) const;

   /// Gets a sub-box of this restricted to scop
   IntervalBox subRegion(const Scope& scop) const;

   /// Returns Hausdorff distance between this and B
   double distance(const IntervalBox& B) const;

   /// Returns the Hausdorff distance between this and B restricted to scop
   double distanceOnScope(const IntervalBox& B, const Scope& scop) const;

   /// Returns the gap componentwise between this and B
   double gap(const IntervalBox& B) const;

   /// Returns the gap componentwise between this and B restricted to scop
   double gapOnScope(const IntervalBox& B, const Scope& scop) const;

   /// Inflation method restricted to scop
   void inflateOnScope(const Scope& scop, double delta, double chi);

   /// Returns the sum of the widths of the components of this
   double perimeter() const;

   /// Returns the sum of the widths of the components of this from scop
   double perimeterOnScope(const Scope& scop) const;

   /**
    * @brief Returns the grid perimeter of this.
    * 
    *
    * Given xi in Di with tolerance Ei for each i, the grid perimeter is the
    * sum for each i of the following quantity:
    * - 0.0 if Di has tolerance Ei
    * - width(Di) / Ei (where Ei is the absolute tolerance) otherwise
    */
   double gridPerimeter() const;

   /// Returns the grid perimeter of this restricted to scop
   double gridPerimeterOnScope(const Scope& scop) const;

   IntervalBox* clone() const override;
   void print(std::ostream& os) const override;

   /// Outout on a stream with one variable per line
   void listPrint(std::ostream& os) const;

   /// Output on a stream using a vector notation
   void vecPrint(std::ostream& os) const;

   /// Tests if the domain of a variable is splitable
   bool isSplitable(const Variable& v) const;

   /**
    * @brief Tests if this behaves like a vector.
    * 
    * Returns true if the scope of this contains the variables whose
    * identifiers are 0, 1, 2, ..., which permits to consider this as a vector.
    * In other words, it is possible to cast this to an IntervalVector and
    * to access the domains using the variable ids.
    */
   bool isVectorizable() const;

   /// Equality test
   bool equals(const IntervalBox& B) const;

   /// Equality test on all dimensions but one variable
   bool equals(const IntervalBox& B, const Variable& v) const;

private:
   Scope scop_;
};

/// This is a shared interval box.
using SharedIntervalBox = std::shared_ptr<IntervalBox>;

} // namespace

#endif
