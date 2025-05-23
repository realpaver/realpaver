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
 * @file   RealPoint.hpp
 * @brief  Classes of real points
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_REAL_POINT_HPP
#define REALPAVER_REAL_POINT_HPP

#include "realpaver/RealVector.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

/// Scoped real vector
class RealPoint : public RealVector {
public:
   /**
    * @brief Constructor.
    *
    * Each variable from scop is assigned to a.
    */
   RealPoint(Scope scop, double a = 0.0);

   /**
    * @brief Constructor.
    *
    * The i-th variable of scop is assigned to X[i] for each i.
    */
   RealPoint(Scope scop, const RealVector &X);

   /// Default copy constructor
   RealPoint(const RealPoint &) = default;

   /// Creates a point from another point projected on a scope
   RealPoint(const RealPoint &pt, Scope scop);

   /// Default assignment operator
   RealPoint &operator=(const RealPoint &) = default;

   /// Default destructor
   ~RealPoint() = default;

   /// Returns the scope of this (sorted set of variables)
   Scope scope() const;

   /// Gets the value of v in this
   double get(const Variable &v) const;

   /// Sets v to a
   void set(const Variable &v, double a);

   /**
    * @brief Assignment on a scope.
    *
    * this[scop] is assigned to pt[scop]
    */
   void setOnScope(const RealPoint &pt, const Scope &scop);

   /// Gets a sub-point corresponding to this restricted to scop
   RealPoint subPoint(const Scope &scop) const;

   RealPoint *clone() const override;

   /**
    * @brief Tests if this behaves like a vector.
    *
    * Returns true if the scope of this contains the variables whose
    * identifiers are 0, 1, 2, ..., which permits to consider this as a vector.
    * In other words, it is possible to cast this to a RealVector and to access
    * the domains using the variable ids.
    */
   bool isVectorizable() const;

private:
   Scope scop_;
};

} // namespace realpaver

#endif
