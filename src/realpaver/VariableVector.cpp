///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

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

void VariableVectorRep::setDomain(const Interval& x)
{
   for (auto& v : vars_)
      v.setDomain(x);
}

void VariableVectorRep::setInteger()
{
   for (auto& v : vars_)
      v.setInteger();
}

void VariableVectorRep::setBinary()
{
   for (auto& v : vars_)
      v.setBinary();
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

///////////////////////////////////////////////////////////////////////////////

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

VariableVector& VariableVector::setDomain(const Interval& x)
{
   rep_->setDomain(x);
   return *this;
}

VariableVector& VariableVector::setInteger()
{
   rep_->setInteger();
   return *this;
}

VariableVector& VariableVector::setBinary()
{
   rep_->setBinary();
   return *this;
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
