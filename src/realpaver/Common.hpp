///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
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
// Hash coding functions
///////////////////////////////////////////////////////////////////////////////

/// Calculates the hash code of a value using std::hash
template <typename T>
std::size_t hash1(const T& x)
{
   std::hash<T> fun;
   return fun(x);
}

/// Combines two hash codes
std::size_t hash2(std::size_t h1, std::size_t h2);

/// Combines three hash codes
std::size_t hash3(std::size_t h1, std::size_t h2, std::size_t h3);

/// Combines four hash codes
std::size_t hash4(std::size_t h1, std::size_t h2,
                  std::size_t h3, std::size_t h4);

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
