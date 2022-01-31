// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/variable.hpp"

namespace realpaver {

VariableRep::VariableRep(const std::string& name) :
      name_(name),
      id_(0),
      domain_(),
      continuous_(true),
      tol_(Tolerance::makeAbs(0.0))
{}

Variable::Variable(const std::string& name) :
   rep_(std::make_shared<VariableRep>(name))
{}

std::ostream& operator<<(std::ostream& os, const Variable& v)
{
   os << v.name()
      << " #"
      << v.id()
      << " "
      << (v.isContinuous() ? "(C)" : "(D)")
      << " in "
      << v.domain()
      << " @ "
      << v.getTolerance();

   return os;
}

Variable Variable::clone() const
{
   Variable v(name());

   v.setId(id())
    .setDomain(domain())
    .setTolerance(getTolerance());

   if (isDiscrete())
      v.setDiscrete();
   else
      v.setContinuous();

   return v;
}

} // namespace
