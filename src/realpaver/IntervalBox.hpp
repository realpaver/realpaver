///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_BOX_HPP
#define REALPAVER_INTERVAL_BOX_HPP

#include <memory>
#include "realpaver/IntervalVector.hpp"
#include "realpaver/RealPoint.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

class DomainBox;

///////////////////////////////////////////////////////////////////////////////
/// This is a scoped interval vector.
///////////////////////////////////////////////////////////////////////////////
class IntervalBox : public IntervalVector {
public:
   /// Creates an interval box
   /// @param scop scope of this
   ///
   /// The domains in this are assigned to the hull of domains enclosed in the
   /// variables
   IntervalBox(Scope scop);

   /// Creates an interval box
   /// @param scop scope of this
   /// @param x interval assigned to each element of this
   IntervalBox(Scope scop, const Interval& x);

   /// Creates an interval box
   /// @param scop scope of this
   /// @param X interval vector having the same size than sco
   ///
   /// The i-th variable in scop is assigned to X[i] for each i.
   IntervalBox(Scope scop, const IntervalVector& X);

   /// Creates an interval box
   /// @param scop scope of this
   /// @param X real vector having the same size than scop
   ///
   /// The i-th variable in scop is assigned to X[i] for each i.
   IntervalBox(Scope scop, const RealVector& X);

   /// Creates an interval box
   /// @param pt a real point assigned to this
   IntervalBox(const RealPoint& pt);

   /// Creates an interval box
   /// @param box a domain box
   ///
   /// this is the interval hull of the given box
   IntervalBox(const DomainBox& box);

   /// Default copy constructor
   IntervalBox(const IntervalBox&) = default;

   /// Creates a box from another box projected on a scope
   /// @param B an interval box
   /// @param scop a scope that is included in the scope of B
   ///
   /// this is equal to B restricted to scop
   IntervalBox(const IntervalBox& B, Scope scop);

   /// Default assignment operator
   IntervalBox& operator=(const IntervalBox&) = default;

   /// Default destructor
   ~IntervalBox() = default;

   /// @return the scope of this (sorted set of variables)
   Scope scope() const;

   /// Gets an element in this
   /// @param v a variable that belongs to the scope of this
   /// @return the interval value of v in this
   ///
   /// This masks the access by index.
   Interval get(const Variable& v) const;

   /// Sets an element of this
   /// @param v a variable that belongs to the scope of this
   /// @param x interval assigned to v in this
   ///
   /// This masks the access by index.
   void set(const Variable& v, const Interval& x);

   /// @return the midpoint of this
   RealPoint midpoint() const;

   /// @return the corner of this made from all the left bounds
   RealPoint lCorner() const;

   /// @return the corner of this made from all the right bounds
   RealPoint rCorner() const;

   /// Gets a corner of this
   /// @param bs a bitset having the same size than the scope of this
   /// @return a corner of this defined by 'bs'
   ///
   /// Let res be the result. For each i, res[i] is equal to the left bound of
   /// the i-th component of this (the i-th variable in the scope) if bs[i] is
   /// true, the right bound otherwise.
   RealPoint corner(const Bitset& bs) const;

   /// Gets a corner of this
   /// @param bs a bitset having the same size than the scope of this
   /// @return a corner of this defined by 'bs'
   ///
   /// Let res be the result. For each i, res[i] is equal to the right bound of
   /// the i-th component of this (the i-th variable in the scope) if bs[i] is
   /// true, the left bound otherwise.
   RealPoint oppositeCorner(const Bitset& bs) const;

   /// Set containment test
   /// @param B a box whose scope is contained in the scope of this
   /// @return true if B[v] is included in this[v] for each variable v
   bool contains(const IntervalBox& B) const;

   /// Set containment test
   /// @param B a box whose scope is contained in the scope of this
   /// @return true if B[v] is strictly included in this[v] for each
   ///         variable v
   bool strictlyContains(const IntervalBox& B) const;

   /// Set containment test
   /// @param pt a point whose scope is contained in the scope of this
   /// @return true if pt[v] belongs to this[v] for each variable v
   bool contains(const RealPoint& pt) const;

   /// Set containment test
   /// @param pt a point whose scope is contained in the scope of this
   /// @return true if pt[v] strictly belongs to this[v] for each variable v
   bool strictlyContains(const RealPoint& pt) const;

   /// Tests if two boxes overlap
   /// @param B a box having the same scope than this
   /// @return true if this and B overlap
   bool overlaps(const IntervalBox& B) const;

   /// Glue a box in this
   /// @param B an interval box whose scope is included in the scope of this
   ///
   /// for each variable v in the scope of this, its domain in this is
   /// assigned to the hull of its domain in this and its domain in B
   void glue(const IntervalBox& B);

   /// Glue a box in this, restricted to some scope
   /// @param B an interval box
   /// @param scop a scope included in the scope of this and B
   ///
   /// for each variable v in the given scope, its domain in this is
   /// assigned to the hull of its domain in this and its domain in B
   void glueOnScope(const IntervalBox& B, const Scope& scop);

   /// Assignment on a scope
   /// @param B an interval box
   /// @param scop a scope included in the scope of this and B
   ///
   /// this[scop] is assigned to B[sco]
   void setOnScope(const IntervalBox& B, const Scope& scop);

   /// Midpoint of this on a scope
   /// @param sco a scope included in the scope of this
   /// @return midpoint of this restricted to sco
   RealPoint midpointOnScope(const Scope& sco) const;

   /// Midpoint of this on a scope
   /// @param scop a scope included in the scope of this
   /// @param pt midpoint of this restricted to scop
   void midpointOnScope(const Scope& scop, RealPoint& pt) const;

   /// Gets a sub-box
   /// @param scop a scope included in the scope of this
   /// @return this restricted to scop
   IntervalBox subRegion(const Scope& scop) const;

   /// Hausdorff distance between boxes
   /// @param B an interval box
   /// @return the maximum distance componentwise between this and B
   ///
   /// Assumption: this and B have the same scope
   double distance(const IntervalBox& B) const;

   /// Hausdorff distance between boxes on a scope
   /// @param B an interval box
   /// @param scop a scope
   /// @return the maximum distance componentwise between this and B
   ///         restricted to scop
   ///
   /// Assumption: scop is included in the scopes of this and B
   double distanceOnScope(const IntervalBox& B, const Scope& scop) const;

   /// Gap between boxes
   /// @param B an interval box
   /// @return the maximum gap componentwise between this and B
   ///
   /// Assumption: this and B have the same scope
   double gap(const IntervalBox& B) const;

   /// Gap between boxes on a scope
   /// @param B an interval box
   /// @param scop a scope
   /// @return the maximum gap componentwise between this and B
   ///         restricted to scop
   ///
   /// Assumption: scop is included in the scopes of this and B
   double gapOnScope(const IntervalBox& B, const Scope& scop) const;

   /// Inflation method
   /// @param scop a scope
   /// @param delta a real > 1.0
   /// @param chi a real > 0.0
   ///
   /// Let x be an interval of scop and let m(x) be its midpoint.
   /// Then x is replaced by m(x) + delta*(x - m(x)) + chi*[-1,1].
   void inflateOnScope(const Scope& scop, double delta, double chi);

   /// @return the sum of the widths of the components of this
   double perimeter() const;

   /// @return the sum of the widths of some components of this
   /// @param scop the components considered
   double perimeterOnScope(const Scope& scop) const;

   /// @return the grid perimeter of this
   ///
   /// Given xi in Di with tolerance Ei for each i, the grid perimeter is the
   /// sum for each i of the following quantity:
   /// - 0.0 if Di has tolerance Ei
   /// - width(Di) / Ei (where Ei is the absolute tolerance) otherwise
   double gridPerimeter() const;

   /// @return the grid perimeter for some components of this
   /// @param scop the components considered   
   double gridPerimeterOnScope(const Scope& scop) const;

   ///@{
   IntervalBox* clone() const override;
   void print(std::ostream& os) const override;
   ///@}

   /// Display on a stream with one variable per line
   /// @param os an output stream
   ///
   /// Example:
   /// x = [1.35, 1.36]
   /// y = [0, 0.01]
   void listPrint(std::ostream& os) const;

   /// Display on a stream using a vector notation
   /// @param os an output stream
   ///
   /// Example:
   /// (x = [1.35, 1.36], y = [Ø, 0.01])
   void vecPrint(std::ostream& os) const;

   /// Tests if the domain of a variable is splitable
   /// @param v a variable in this
   /// @return true if the domain of v in this has the desired tolerance
   bool isSplitable(const Variable& v) const;

private:
   Scope scop_;
};

/// This is a shared interval box.
typedef std::shared_ptr<IntervalBox> SharedIntervalBox;

} // namespace

#endif
