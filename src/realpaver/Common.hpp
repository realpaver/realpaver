///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_COMMON_HPP
#define REALPAVER_COMMON_HPP

#include <iostream>
#include <limits>

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
// Hash codes
///////////////////////////////////////////////////////////////////////////////

/// Calculates the hash code of a value using std::hash
template <typename T>
size_t hash1(const T& x)
{
   std::hash<T> fun;
   return fun(x);
}

/// Combines two hash codes
size_t hash2(size_t h1, size_t h2);

/// Combines three hash codes
size_t hash3(size_t h1, size_t h2, size_t h3);

/// Combines four hash codes
size_t hash4(size_t h1, size_t h2, size_t h3, size_t h4);


///////////////////////////////////////////////////////////////////////////////
/// This contains int values and operations.
///////////////////////////////////////////////////////////////////////////////
struct Integer {
   /// @return the minimum representable value
   static inline int min()
   {
      return std::numeric_limits<int>::min();
   }

   /// @return the maximum representable value
   static inline int max()
   {
      return std::numeric_limits<int>::max();
   }
};

///////////////////////////////////////////////////////////////////////////////
/// This is a certificate returned by solvers of optimization problems.
///////////////////////////////////////////////////////////////////////////////
enum class OptimizationStatus {
   Optimal,
   Infeasible,
   StopOnIterLimit,
   StopOnNodeLimit,
   StopOnTimeLimit,
   Other
};

/// Output on a stream of an optimization status
std::ostream& operator<<(std::ostream& os, OptimizationStatus status);

///////////////////////////////////////////////////////////////////////////////
/// This is a certificate returned by interval solvers.
///////////////////////////////////////////////////////////////////////////////
enum class Proof {
   Empty,      ///< proof of infeasible region
   Maybe,      ///< no proof
   Feasible,   ///< proof of feasible region
   Inner       ///< proof of inner region
};

/// Output on a stream of a certificate of proof
std::ostream& operator<<(std::ostream& os, Proof proof);

} // namespace

#endif
