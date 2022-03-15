///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_REAL_POINT_HPP
#define REALPAVER_REAL_POINT_HPP

#include "realpaver/RealVector.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a scoped real vector.
///////////////////////////////////////////////////////////////////////////////
class RealPoint : public RealVector {
public:
   /// Creates a real pooint
   /// s scope of this
   /// a value assigned to each element of this
   RealPoint(Scope s, double a = 0.0);

   /// Creates a real point
   /// s scope of this
   /// X real vector having the same size than s
   ///
   /// The i-th variable in s is assigned to X[i] for each i.
   RealPoint(Scope s, const RealVector& X);

   /// Default copy constructor
   RealPoint(const RealPoint&) = default;

   /// Default assignment operator
   RealPoint& operator=(const RealPoint&) = default;

   /// Default destructor
   ~RealPoint() = default;

   /// @return the scope of this (sorted set of variables)
   Scope scope() const;

   /// Gets an element in this
   /// @param v a variable that belongs to the scope of this
   /// @return the value of v in this
   ///
   /// This masks the access by index.
   double get(Variable v) const;

   /// Sets an element of this
   /// @param v a variable that belongs to the scope of this
   /// @param a value assigned to v ion this
   ///
   /// This masks the access by index.
   void set(Variable v, double a);

   /// Assignment on a scope
   /// @param pt a real point
   /// @param s a scope included in the scope of this and pt
   ///
   /// this[s] is assigned to pt[s]
   void setOnScope(const RealPoint& pt, Scope s);

   /// Gets a sub-point
   /// @param s a scope included in the scope of this
   /// @return this restricted to s
   RealPoint subPoint(Scope s) const;

   /// Overrides (RealVector)
   /// @return a clone of this
   RealPoint* clone() const;

private:
   Scope scope_;
};

} // namespace

#endif
