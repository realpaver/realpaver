// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_COMMON_HPP
#define REALPAVER_COMMON_HPP

#include <iostream>
#include <string>
#include "realpaver/config.hpp"

namespace realpaver {

/*****************************************************************************
 * Hash functions.
 *****************************************************************************/
template <typename T>
size_t Hash1(const T& h)
{
   std::hash<T> fun;
   return fun(h);
}

inline size_t Hash2(size_t h1, size_t h2)
{
   return h1 ^ (h2 << 1);
}

inline size_t Hash3(size_t h1, size_t h2, size_t h3)
{
   return Hash2(h1,Hash2(h2,h3));
}

inline size_t Hash4(size_t h1, size_t h2, size_t h3, size_t h4)
{
   return Hash2(h1,Hash3(h2,h3,h4));
}


/*****************************************************************************
 * Certificates returned by optimizers.
 *****************************************************************************/
enum class OptimizationStatus {
   Optimal,
   Infeasible,
   StopOnTimeLimit,
   StopOnIterLimit,
   Other
};

/*****************************************************************************
 * Certificates returned by provers.
 *****************************************************************************/
enum class Proof {
   Empty,      // empty solution set
   Maybe,      // no proof is derived
   Feasible,   // proof of existence of a solution
   Inner,      // proof of inner box
   Optimal     // proof of optimality
};

std::ostream& operator<<(std::ostream& os, const Proof& p);


#if DEBUG_ON
#   define DEBUG(msg)   std::cerr << msg << std::endl
#else
#   define DEBUG(msg)
#endif

} // namespace

#endif
