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
 * @file   Common.cpp
 * @brief  Common types and functions
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/Common.hpp"

namespace realpaver {

size_t hash2(size_t h1, size_t h2)
{
   return h1 ^ (h2 << 1);
}

size_t hash3(size_t h1, size_t h2, size_t h3)
{
   return hash2(h1, hash2(h2,h3));
}

size_t hash4(size_t h1, size_t h2, size_t h3, size_t h4)
{
   return hash2(h1, hash3(h2,h3,h4));
}

std::ostream& operator<<(std::ostream& os, OptimizationStatus status)
{
   switch(status)
   {
      case OptimizationStatus::Optimal:         return os << "Optimal";
      case OptimizationStatus::Infeasible:      return os << "Infeasible";
      case OptimizationStatus::StopOnIterLimit: return os << "StopOnIterLimit";
      case OptimizationStatus::StopOnTimeLimit: return os << "StopOnTimeLimit";
      case OptimizationStatus::StopOnNodeLimit: return os << "StopOnNodeLimit";
      case OptimizationStatus::Other:           return os << "Other";
      default: os.setstate(std::ios::failbit);
   }
   return os;
}

std::ostream& operator<<(std::ostream& os, Proof proof)
{
   switch(proof)
   {
      case Proof::Empty:    return os << "empty";
      case Proof::Maybe:    return os << "maybe";
      case Proof::Feasible: return os << "feasible";
      case Proof::Inner:    return os << "inner";
      default:              os.setstate(std::ios::failbit);
   }
   return os;
}

} // namespace
