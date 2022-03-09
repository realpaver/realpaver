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
   IntervalRegion(const Scope& s, const Interval& x = Interval::universe());

   /// Default copy constructor
   IntervalRegion(const IntervalRegion&) = default;

   /// No assignment
   IntervalRegion& operator=(const IntervalRegion&) = delete;

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

   /// Hull with assignment on a scope
   /// @param reg an interval region
   /// @param s a scope included in the scope of this and reg
   ///
   /// this[s] is assigned to the hull of this[s] and reg[s]
   void hullAssignOnScope(const IntervalRegion& reg, const Scope& s);

   /// Assignment on a scope
   /// @param reg an interval region
   /// @param s a scope included in the scope of this and reg
   ///
   /// this[s] is assigned to reg[s]
   void setOnScope(const IntervalRegion& reg, const Scope& s);

   /// Midpoint of this on a scope
   /// @param s a scope included in the scope of this
   /// @return midpoint of this restricted to s
   RealVector midpointOnScope(const Scope& s) const;

   /// Calculates the midpoint of this on a scope
   /// @param s a scope included in the scope of this
   /// @param mid vector assigned to the midpoint of this[s]
   void toMidpointOnScope(const Scope& s, RealVector& mid) const;

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
