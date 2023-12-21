///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_ZERO_ONE_HPP
#define REALPAVER_ZERO_ONE_HPP

#include <iostream>

namespace realpaver {

class Range;

///////////////////////////////////////////////////////////////////////////////
/// This represents a subset of {0, 1}.
///////////////////////////////////////////////////////////////////////////////
class ZeroOne {
public:
   /// Creates the set {0, 1}
   ZeroOne();

   /// Creates an element
   /// @param zro true if 0 belongs to this
   /// @param one true if 1 belongs to this
   ZeroOne(bool zro, bool one);

   /// Default copy constructor
   ZeroOne(const ZeroOne&) = default;

   /// Default assignment operator
   ZeroOne& operator=(const ZeroOne&) = default;

   /// Default destructor
   ~ZeroOne() = default;

   ///@{
   static ZeroOne universe();
   static ZeroOne zero();
   static ZeroOne one();
   static ZeroOne emptyset();
   ///@}

   /// @return true if this is empty
   bool isEmpty() const;

   /// @return true if this is reduced to 0
   bool isZero() const;

   /// @return true if this is reduced to 1
   bool isOne() const;

   /// @return true if this is {0, 1}
   bool isUniverse() const;

   /// @return true if 0 is in this
   bool hasZero() const;

   /// @return true if 1 is in this
   bool hasOne() const;

   /// Assignment method
   /// @param b true in order to insert 0 in this, false to remove it
   /// @return a reference to this
   ZeroOne& setZero(bool b);

   /// Assignment method
   /// @param b true in order to insert 1 in this, false to remove it
   /// @return a reference to this
   ZeroOne& setOne(bool b);

   /// @return a range that corresponds to this
   Range toRange() const;

private:
   bool zro_, one_;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const ZeroOne& zo);

} // namespace

#endif
