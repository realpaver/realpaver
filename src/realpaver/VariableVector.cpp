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
 * @file   VariableVector.cpp
 * @brief  Vector of variables
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include <sstream>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/VariableVector.hpp"

namespace realpaver {

VariableVectorRep::VariableVectorRep(const std::string& name, int first,
                                     int last)
   : name_(name),
     first_(first),
     last_(last),
     vars_()
{
   ASSERT(first <= last, "Bad indexes for the creation of a variable vector");

   for (int i=first; i<=last; ++i)
   {
      std::ostringstream os;
      os << i;

      std::string sv = name + "[" + os.str() + "]";
      Variable v(sv);
      vars_.push_back(v);
   }
}

void VariableVectorRep::print(std::ostream& os) const
{
   size_t p = vars_.size() - 1;

   os << "(";
   for (size_t i=0; i<=p; ++i)
   {
      os << vars_[i].getName();
      if (i<p) os << ", ";
   }
   os << ")";
}

Variable VariableVectorRep::get(int i) const
{
   ASSERT(i>= first_ && i<=last_, "Bad access in a variable vector @ " << i);
   return vars_[i-first_];
}

void VariableVectorRep::setTolerance(const Tolerance& tol)
{
   for (auto& v : vars_)
      v.setTolerance(tol);
}

size_t VariableVectorRep::hashCode() const
{
   size_t h = 0;
   for (auto& v : vars_)
      h = hash2(h, v.hashCode());
   return h;
}

int VariableVectorRep::size() const
{
   return last_ - first_ + 1;
}

int VariableVectorRep::firstIndex() const
{
   return first_;
}

int VariableVectorRep::lastIndex() const
{
   return last_;
}

/*----------------------------------------------------------------------------*/

VariableVector::VariableVector(const std::string& name, int first, int last)
   : rep_(std::make_shared<VariableVectorRep>(name, first, last))
{}

void VariableVector::print(std::ostream& os) const
{
   rep_->print(os);
}

Variable VariableVector::get(int i) const
{
   return rep_->get(i);
}

Variable VariableVector::operator[](int i) const
{
   return rep_->get(i);
}

VariableVector& VariableVector::setTolerance(const Tolerance& tol)
{
   rep_->setTolerance(tol);
   return *this;
}

size_t VariableVector::hashCode() const
{
   return rep_->hashCode();
}

int VariableVector::size() const
{
   return rep_->size();
}

int VariableVector::firstIndex() const
{
   return rep_->firstIndex();
}

int VariableVector::lastIndex() const
{
   return rep_->lastIndex();
}

std::ostream& operator<<(std::ostream& os, const VariableVector& v)
{
   v.print(os);
   return os;
}

} // namespace
