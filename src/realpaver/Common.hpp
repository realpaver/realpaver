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
 * @file   Common.hpp
 * @brief  Common types and functions
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_COMMON_HPP
#define REALPAVER_COMMON_HPP

#include <iostream>

namespace realpaver {

/// Returns the hash code of a value using std::hash
template <typename T> size_t hash1(const T &x)
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

/*----------------------------------------------------------------------------*/

/// Certificate returned by solvers of optimization problems
enum class OptimizationStatus {
   Optimal,         ///< optimum found
   Infeasible,      ///< proved infeasible
   StopOnIterLimit, ///< iteration limit exceeded
   StopOnNodeLimit, ///< node limit exceeded
   StopOnTimeLimit, ///< time limit exceeded
   Other            ///< other status
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, OptimizationStatus status);

/*----------------------------------------------------------------------------*/

/** @brief Certificate returned by interval solvers */
enum class Proof {
   Empty,    ///< proof of infeasible region
   Maybe,    ///< no proof
   Feasible, ///< proof of feasible region
   Inner     ///< proof of inner region
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, Proof proof);

} // namespace realpaver

#endif
