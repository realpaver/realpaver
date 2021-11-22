// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_variable.hpp"

namespace realpaver {

VariableRep::VariableRep(const std::string& name) :
   name_(name), id_(0), domain_(), continuous_(true),
   eps_(Precision::absolute(0))
{}

Variable::Variable(const std::string& name) :
   rep_(std::make_shared<VariableRep>(name))
{}

std::ostream& operator<<(std::ostream& os, const Variable& v)
{
   os << "("       << v.name()   << ", "
      << "id="     << v.id()     << ", "
      << "domain=" << v.domain() << ", "
      << (v.isContinuous() ? "continuous" : "discrete") << ", "
      << "precision=" << v.precision() << ")";
   
   return os;
}

Variable Variable::clone() const
{
   Variable v(name());

   v.setId(id())
    .setDomain(domain())
    .setPrecision(precision());

   if (isDiscrete())
      v.setDiscrete();
   else
      v.setContinuous();

   return v;
}

} // namespace
