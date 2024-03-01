///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
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
   /// @param scop scope of this
   /// @param a value assigned to each element of this
   RealPoint(Scope scop, double a = 0.0);

   /// Creates a real point
   /// @param scop scope of this
   /// @param X real vector having the same size than s
   ///
   /// The i-th variable in scop is assigned to X[i] for each i.
   RealPoint(Scope scop, const RealVector& X);

   /// Default copy constructor
   RealPoint(const RealPoint&) = default;

   /// Creates a point from another point projected on a scope
   /// @param pt a point
   /// @param scop a scope that is included in the scope of pt
   ///
   /// this is equal to pt restricted to scop
   RealPoint(const RealPoint& pt, Scope scop);

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
   double get(const Variable& v) const;

   /// Sets an element of this
   /// @param v a variable that belongs to the scope of this
   /// @param a value assigned to v ion this
   ///
   /// This masks the access by index.
   void set(const Variable& v, double a);

   /// Assignment on a scope
   /// @param pt a real point
   /// @param scop a scope included in the scope of this and pt
   ///
   /// this[scop] is assigned to pt[scop]
   void setOnScope(const RealPoint& pt, const Scope& scop);

   /// Gets a sub-point
   /// @param scop a scope included in the scope of this
   /// @return this restricted to scop
   RealPoint subPoint(const Scope& scop) const;

   ///@{
   RealPoint* clone() const override;
   ///@}

private:
   Scope scop_;
};

} // namespace

#endif
