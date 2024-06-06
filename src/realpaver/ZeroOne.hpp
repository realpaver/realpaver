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
 * @file   ZeroOne.hpp
 * @brief  Subset of {0, 1}
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_ZERO_ONE_HPP
#define REALPAVER_ZERO_ONE_HPP

#include <iostream>

namespace realpaver {

class Range;

/// Subset of {0, 1}
class ZeroOne {
public:
   /// Creates the set {0, 1}
   ZeroOne();

   /**
    * @brief Constructor.
    * 
    * 4 cases given the parameters:
    * - (true, true) -> {0, 1}
    * - (true, false) -> {0}
    * - (false, true) -> {1}
    * - (false, false) -> {}
    */
   ZeroOne(bool zro, bool one);

   /// Default copy constructor
   ZeroOne(const ZeroOne&) = default;

   /// Default assignment operator
   ZeroOne& operator=(const ZeroOne&) = default;

   /// Default destructor
   ~ZeroOne() = default;

   /// Returns {0, 1}
   static ZeroOne universe();

   /// Returns {0}
   static ZeroOne zero();

   /// Returns {1}
   static ZeroOne one();

   /// Returns {}
   static ZeroOne emptyset();

   /// Returns true if this is empty
   bool isEmpty() const;

   /// Returns true if this is reduced to 0
   bool isZero() const;

   /// Returns true if this is reduced to 1
   bool isOne() const;

   /// Returns true if this is {0, 1}
   bool isUniverse() const;

   /// Returns true if 0 is in this
   bool hasZero() const;

   /// Returns true if 1 is in this
   bool hasOne() const;

   /**
    * @brief Assignment method.
    * 
    * Inserts 0 in this if b = true, removes it otherwise.
    * Returns a reference to this.
    */
   ZeroOne& setZero(bool b);

   /**
    * @brief Assignment method.
    * 
    * Inserts 1 in this if b = true, removes it otherwise.
    * Returns a reference to this.
    */
   ZeroOne& setOne(bool b);

   /// Returns a range that corresponds to this
   Range toRange() const;

   /// Equality test
   bool equals(const ZeroOne& other) const;

private:
   bool zro_, one_;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const ZeroOne& zo);

} // namespace

#endif
