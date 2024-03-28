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
 * @date   2022-5-6
*/

#ifndef REALPAVER_COMMON_HPP
#define REALPAVER_COMMON_HPP

#include <iostream>
#include <limits>

namespace realpaver {

/** @brief Calculates the hash code of a value using std::hash */
template <typename T>
std::size_t hash1(const T& x)
{
   std::hash<T> fun;
   return fun(x);
}

/** @brief Combines two hash codes */
std::size_t hash2(std::size_t h1, std::size_t h2);

/** @brief Combines three hash codes */
std::size_t hash3(std::size_t h1, std::size_t h2, std::size_t h3);

/** @brief Combines four hash codes */
std::size_t hash4(std::size_t h1, std::size_t h2,
                  std::size_t h3, std::size_t h4);

/** @brief Certificate returned by solvers of optimization problems */
enum class OptimizationStatus {
   Optimal,            ///< optimum found
   Infeasible,         ///< proved infeasible
   StopOnIterLimit,    ///< iteration limit exceeded
   StopOnNodeLimit,    ///< node limit exceeded
   StopOnTimeLimit,    ///< time limit exceeded
   Other               ///< other status
};

/** @brief Output on a stream of an optimization status */
std::ostream& operator<<(std::ostream& os, OptimizationStatus status);

/** @brief Certificate returned by interval solvers */
enum class Proof {
   Empty,      ///< proof of infeasible region
   Maybe,      ///< no proof
   Feasible,   ///< proof of feasible region
   Inner       ///< proof of inner region
};

/** @brief Output on a stream of a certificate of proof */
std::ostream& operator<<(std::ostream& os, Proof proof);

} // namespace

#endif
