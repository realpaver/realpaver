///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_REGION_HPP
#define REALPAVER_INTERVAL_REGION_HPP

#include <memory>
#include "realpaver/IntervalVector.hpp"
#include "realpaver/RealPoint.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a scoped interval vector.
///////////////////////////////////////////////////////////////////////////////
class IntervalRegion : public IntervalVector {
public:
   /// Creates an interval region
   /// s scope of this
   /// x interval assigned to each element of this
   IntervalRegion(Scope s, const Interval& x = Interval::universe());

   /// Creates an interval region
   /// s scope of this
   /// X interval vector having the same size than s
   ///
   /// The i-th variable in s is assigned to X[i] for each i.
   IntervalRegion(Scope s, const IntervalVector& X);

   /// Default copy constructor
   IntervalRegion(const IntervalRegion&) = default;

   /// Default assignment operator
   IntervalRegion& operator=(const IntervalRegion&) = default;

   /// Default destructor
   ~IntervalRegion() = default;

   /// @return the scope of this (sorted set of variables)
   Scope scope() const;

   /// Gets an element in this
   /// @param v a variable that belongs to the scope of this
   /// @return the interval value of v in this
   ///
   /// This masks the access by index.
   Interval get(Variable v) const;

   /// Sets an element of this
   /// @param v a variable that belongs to the scope of this
   /// @param x interval assigned to v ion this
   ///
   /// This masks the access by index.
   void set(Variable v, const Interval& x);

   /// @return the midpoint of this
   RealPoint midpoint() const;

   /// @return the corner of this made from all the left bounds
   RealPoint lCorner() const;

   /// @return the corner of this made from all the right bounds
   RealPoint rCorner() const;

  /// Gets a corner of this
   /// @param bs a bitset having the same size than this
   /// @return a corner of this defined by 'bs'
   ///
   /// Let res be the result. For each i, res[i] is equal to the left bound of
   /// the i-th component of this if bs[i] is true, the right bound otherwise.
   RealPoint corner(const Bitset& bs) const;

   /// Gets a corner of this
   /// @param bs a bitset having the same size than this
   /// @return a corner of this defined by 'bs'
   ///
   /// Let res be the result. For each i, res[i] is equal to the right bound of
   /// the i-th component of this if bs[i] is true, the left bound otherwise.
   RealPoint oppositeCorner(const Bitset& bs) const;

   /// Set containment test
   /// @param reg a region whose scope is contained in the scope of this
   /// @return true if reg[v] is included in this[v] for each variable v
   bool contains(const IntervalRegion& reg) const;

   /// Set containment test
   /// @param reg a region whose scope is contained in the scope of this
   /// @return true if reg[v] is strictly included in this[v] for each
   ///         variable v
   bool strictlyContains(const IntervalRegion& reg) const;

   /// Set containment test
   /// @param pt a point whose scope is contained in the scope of this
   /// @return true if pt[v] belongs to this[v] for each variable v
   bool contains(const RealPoint& pt) const;

   /// Set containment test
   /// @param pt a point whose scope is contained in the scope of this
   /// @return true if pt[v] strictly belongs to this[v] for each variable v
   bool strictlyContains(const RealPoint& pt) const;

      /// Tests if two regions overlap
   /// @param reg a region having the same scope than this
   /// @return true if this and reg overlap
   bool overlaps(const IntervalRegion& reg) const;

   /// Hull with assignment on a scope
   /// @param reg an interval region
   /// @param s a scope included in the scope of this and reg
   ///
   /// this[s] is assigned to the hull of this[s] and reg[s]
   void hullAssignOnScope(const IntervalRegion& reg, Scope s);

   /// Assignment on a scope
   /// @param reg an interval region
   /// @param s a scope included in the scope of this and reg
   ///
   /// this[s] is assigned to reg[s]
   void setOnScope(const IntervalRegion& reg, Scope s);

   /// Midpoint of this on a scope
   /// @param s a scope included in the scope of this
   /// @return midpoint of this restricted to s
   RealPoint midpointOnScope(Scope s) const;

   /// Gets a sub-region
   /// @param s a scope included in the scope of this
   /// @return this restricted to s
   IntervalRegion subRegion(Scope s) const;

   /// Overrides (IntervalVector)
   /// @return a clone of this
   IntervalRegion* clone() const;

private:
   Scope scope_;
};

/// This is a shared interval region.
typedef std::shared_ptr<IntervalRegion> SharedIntervalRegion;

} // namespace

#endif
