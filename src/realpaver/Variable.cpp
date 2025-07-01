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
 * @file   Variable.cpp
 * @brief  Variable in a problem
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/Exception.hpp"
#include "realpaver/Variable.hpp"
#include <memory>

namespace realpaver {

int VariableRep::NEXT_ID = 0;

std::ostream &operator<<(std::ostream &os, VariableStatus status)
{
   switch (status)
   {
   case VariableStatus::Decision:
      return os << "Decision";
   case VariableStatus::Objective:
      return os << "Objective";
   case VariableStatus::Auxiliary:
      return os << "Auxiliary";
   default:
      os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

VariableRep::VariableRep(const std::string &name)
    : name_(name)
    , id_(NEXT_ID++)
    , dom_(nullptr)
    , status_(VariableStatus::Decision)
    , tol_(0.0, 0.0)
    , split_(true)
{
   dom_ = new IntervalDomain(Interval::universe());
}

VariableRep::~VariableRep()
{
   if (dom_ != nullptr)
      delete dom_;
}

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

void VariableRep::setName(const std::string &name)
{
   name_ = name;
}

Domain *VariableRep::getDomain() const
{
   return dom_;
}

void VariableRep::setDomain(Domain *dom)
{
   THROW_IF(dom == nullptr, "The domain of " + getName() + " is null");

   if (dom_ != nullptr)
      delete dom_;
   dom_ = dom;
}

VariableStatus VariableRep::getStatus() const
{
   return status_;
}

void VariableRep::setStatus(VariableStatus status)
{
   status_ = status;
}

bool VariableRep::isBinary() const
{
   return dom_->isBinary();
}

bool VariableRep::isInteger() const
{
   return dom_->isInteger();
}

bool VariableRep::isReal() const
{
   return dom_->isReal();
}

size_t VariableRep::hashCode() const
{
   return id_;
}

Tolerance VariableRep::getTolerance() const
{
   return tol_;
}

void VariableRep::setTolerance(const Tolerance &tol)
{
   tol_ = tol;
}

void VariableRep::setSplitable(bool splitable)
{
   split_ = splitable;
}

bool VariableRep::isSplitable() const
{
   return split_;
}

/*----------------------------------------------------------------------------*/

Variable::Variable(const std::string &name)
    : rep_(std::make_shared<VariableRep>(name))
{
}

Variable::Variable()
    : rep_(nullptr)
{
}

size_t Variable::id() const
{
   return rep_->id();
}

Variable &Variable::setId(size_t id)
{
   rep_->setId(id);
   return *this;
}

std::string Variable::getName() const
{
   return rep_->getName();
}

Variable &Variable::setName(const std::string &name)
{
   rep_->setName(name);
   return *this;
}

Domain *Variable::getDomain() const
{
   return rep_->getDomain();
}

Variable &Variable::setDomain(std::unique_ptr<Domain> dom)
{
   rep_->setDomain(dom.release());
   return *this;
}

VariableStatus Variable::getStatus() const
{
   return rep_->getStatus();
}

Variable &Variable::setStatus(VariableStatus status)
{
   rep_->setStatus(status);
   return *this;
}

bool Variable::isBinary() const
{
   return rep_->isBinary();
}

bool Variable::isInteger() const
{
   return rep_->isInteger();
}

bool Variable::isReal() const
{
   return rep_->isReal();
}

size_t Variable::hashCode() const
{
   return rep_->hashCode();
}

Tolerance Variable::getTolerance() const
{
   return rep_->getTolerance();
}

Variable &Variable::setTolerance(const Tolerance &tol)
{
   rep_->setTolerance(tol);
   return *this;
}

bool Variable::operator==(const Variable &other) const
{
   return rep_.get() == other.rep_.get();
}

bool Variable::operator!=(const Variable &other) const
{
   return rep_.get() != other.rep_.get();
}

Variable &Variable::setSplitable(bool splitable)
{
   rep_->setSplitable(splitable);
   return *this;
}

bool Variable::isSplitable() const
{
   return rep_->isSplitable();
}

std::ostream &operator<<(std::ostream &os, const Variable &v)
{
   os << v.getName() << " in " << (*v.getDomain());

   if (v.isReal())
      os << " " << v.getTolerance();

   return os;
}

Variable Variable::clone() const
{
   Variable v(getName());

   std::unique_ptr<Domain> dom(getDomain()->clone());

   v.setId(id())
       .setDomain(std::move(dom))
       .setTolerance(getTolerance())
       .setStatus(getStatus())
       .setSplitable(isSplitable());

   return v;
}

bool Variable::hasNullRep() const
{
   return rep_ == nullptr;
}

} // namespace realpaver
