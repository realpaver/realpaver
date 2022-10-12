///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/Variable.hpp"

namespace realpaver {

VariableRep::VariableRep(const std::string& name) :
      name_(name),
      id_(0),
      domain_(),
      continuous_(true),
      tol_(Tolerance::makeAbs(0.0))
{}

size_t VariableRep::id() const
{
   return id_;
}

void VariableRep::setId(size_t id)
{
   id_ = id;
}

std::string VariableRep::getName() const
{
   return name_;
}

void VariableRep::setName(const std::string& name)
{
   name_ = name;
}

Interval VariableRep::getDomain() const
{
   return domain_;
}

void VariableRep::setDomain(const Interval& x)
{
   domain_ = x;
}

bool VariableRep::isDiscrete() const
{
   return !continuous_;
}

void VariableRep::setDiscrete()
{
   continuous_ = false;
}

bool VariableRep::isContinuous() const
{
   return continuous_;
}

void VariableRep::setContinuous()
{
   continuous_ = true;
}

size_t VariableRep::hashCode() const
{
   return id_;
}

Tolerance VariableRep::getTolerance() const
{
   return tol_;
}

void VariableRep::setTolerance(const Tolerance& tol)
{
   tol_ = tol;
}

///////////////////////////////////////////////////////////////////////////////

Variable::Variable(const std::string& name) :
   rep_(std::make_shared<VariableRep>(name))
{}

Variable::Variable() : rep_(nullptr)
{}

size_t Variable::id() const
{
   return rep_->id();
}

Variable& Variable::setId(size_t id)
{
   rep_->setId(id);
   return *this;
}

std::string Variable::getName() const
{
   return rep_->getName();
}

Variable& Variable::setName(const std::string& name)
{
   rep_->setName(name);
   return *this;
}

Interval Variable::getDomain() const
{
   return rep_->getDomain();
}

Variable& Variable::setDomain(const Interval& x)
{
   rep_->setDomain(x);
   return *this;
}

bool Variable::isDiscrete() const
{
   return rep_->isDiscrete();
}

bool Variable::isDiscreteBool() const
{
   return isDiscrete() && getDomain().isSetEq(Interval::zeroPlusOne());
}

bool Variable::isDiscreteInt() const
{
   return isDiscrete() && getDomain().isSetNeq(Interval::zeroPlusOne());
}

Variable& Variable::setDiscrete()
{
   rep_->setDiscrete();
   return *this;
}

bool Variable::isContinuous() const
{
   return rep_->isContinuous();
}

Variable& Variable::setContinuous()
{
   rep_->setContinuous();
   return *this;
}

size_t Variable::hashCode() const
{
   return rep_->hashCode();
}

Tolerance Variable::getTolerance() const
{
   return rep_->getTolerance();
}

Variable& Variable::setTolerance(const Tolerance& tol)
{
   rep_->setTolerance(tol);
   return *this;
}

bool Variable::operator==(const Variable& other) const
{
   return rep_.get() == other.rep_.get();
}

std::ostream& operator<<(std::ostream& os, const Variable& v)
{
   os << v.getName()
      << " #"
      << v.id()
      << " "
      << (v.isContinuous() ? "(C)" : "(D)")
      << " in "
      << v.getDomain()
      << " @ "
      << v.getTolerance();

   return os;
}

Variable Variable::clone() const
{
   Variable v(getName());

   v.setId(id())
    .setDomain(getDomain())
    .setTolerance(getTolerance());

   if (isDiscrete())
      v.setDiscrete();
   else
      v.setContinuous();

   return v;
}

bool Variable::hasNullPointer() const
{
   return rep_ == nullptr;
}

} // namespace
