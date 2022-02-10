// This file is part of Realpaver. License: see COPYING file.

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

std::ostream& operator<<(std::ostream& os, const OptimizationStatus& status)
{
   switch(status)
   {
      case OptimizationStatus::Optimal:         return os << "Optimal";
      case OptimizationStatus::Infeasible:      return os << "Infeasible";
      case OptimizationStatus::StopOnTimeLimit: return os << "StopOnTimeLimit";
      case OptimizationStatus::StopOnIterLimit: return os << "StopOnIterLimit";
      case OptimizationStatus::Other:           return os << "Other";
   }
   return os;
}

std::ostream& operator<<(std::ostream& os, const Proof& p)
{
   switch(p)
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
