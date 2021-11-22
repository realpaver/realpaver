// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_contractor.hpp"

namespace realpaver {

Contractor::~Contractor()
{}

void Contractor::print(std::ostream& os) const
{}

std::ostream& operator<<(std::ostream& os, const Contractor& op)
{
   op.print(os);
   return os;
}

} // namespace
