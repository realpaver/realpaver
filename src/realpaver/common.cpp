// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/common.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, const Proof& p)
{
   switch(p)
   {
      case Proof::Empty:    return os << "empty";
      case Proof::Maybe:    return os << "maybe";
      case Proof::Feasible: return os << "feasible";
      case Proof::Inner:    return os << "inner";
      case Proof::Optimal:  return os << "optimal";
      default:              os.setstate(std::ios::failbit);
   }
   return os;
}

} // namespace
