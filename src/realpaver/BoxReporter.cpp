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
 * @file   BoxReporter.cpp
 * @brief  Reporter of solutions of problems
 * @author Laurent Granvilliers
 * @date   2024-12-19
 */

#include "realpaver/BoxReporter.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Problem.hpp"

namespace realpaver {

EntityReported::~EntityReported()
{
}

/*----------------------------------------------------------------------------*/

VariableReported::VariableReported(Variable v)
    : EntityReported()
    , v_(v)
{
}

std::string VariableReported::name() const
{
   return v_.getName();
}

std::unique_ptr<Domain> VariableReported::domain(const DomainBox &box) const
{
   std::unique_ptr<Domain> p(box.get(v_)->clone());
   return p;
}

/*----------------------------------------------------------------------------*/

AliasReported::AliasReported(Alias a)
    : EntityReported()
    , a_(a)
{
}

std::string AliasReported::name() const
{
   return a_.name();
}

std::unique_ptr<Domain> AliasReported::domain(const DomainBox &box) const
{
   return a_.term().eval(box);
}

/*----------------------------------------------------------------------------*/

size_t EntityReportedVector::size() const
{
   return ents_.size();
}

std::shared_ptr<EntityReported> EntityReportedVector::get(size_t i) const
{
   ASSERT(i < size(), "Bad access in a vector of reported entities @ " << i);

   return ents_[i];
}

void EntityReportedVector::addVariable(Variable v)
{
   ents_.push_back(std::make_shared<VariableReported>(v));
}

void EntityReportedVector::addAlias(Alias a)
{
   ents_.push_back(std::make_shared<AliasReported>(a));
}

bool EntityReportedVector::contains(const std::string &name) const
{
   for (size_t i = 0; i < size(); ++i)
   {
      std::shared_ptr<EntityReported> p = ents_[i];
      if (p->name() == name)
         return true;
   }
   return false;
}

void EntityReportedVector::remove(const std::string &name)
{
   for (auto it = ents_.begin(); it != ents_.end(); ++it)
   {
      std::shared_ptr<EntityReported> p = *it;
      if (p->name() == name)
      {
         ents_.erase(it);
         return;
      }
   }
}

size_t EntityReportedVector::maxNameLength() const
{
   size_t lmax = 0;
   for (size_t i = 0; i < ents_.size(); ++i)
   {
      size_t l = ents_[i]->name().length();
      if (l > lmax)
         lmax = l;
   }
   return lmax;
}

/*----------------------------------------------------------------------------*/

BoxReporter::BoxReporter()
    : ents_()
{
}

BoxReporter::BoxReporter(const Problem &p)
    : ents_()
{
   for (size_t i = 0; i < p.nbVars(); ++i)
   {
      Variable v = p.varAt(i);
      if (p.isVarReported(v))
         addVariable(v);
   }

   for (size_t i = 0; i < p.nbAliases(); ++i)
   {
      Alias a = p.aliasAt(i);
      if (p.isAliasReported(a))
         addAlias(a);
   }
}

BoxReporter::~BoxReporter()
{
}

void BoxReporter::addVariable(Variable v)
{
   ents_.addVariable(v);
}

void BoxReporter::addAlias(Alias a)
{
   ents_.addAlias(a);
}

void BoxReporter::remove(const std::string &name)
{
   ents_.remove(name);
}

size_t BoxReporter::maxNameLength() const
{
   return ents_.maxNameLength();
}

/*----------------------------------------------------------------------------*/

StreamReporter::StreamReporter(std::ostream &os)
    : BoxReporter()
    , os_(&os)
    , vertical_(true)
{
}

StreamReporter::StreamReporter(const Problem &p, std::ostream &os)
    : BoxReporter(p)
    , os_(&os)
    , vertical_(true)
{
}

std::ostream *StreamReporter::get() const
{
   return os_;
}

void StreamReporter::setVertical(bool b)
{
   vertical_ = b;
}

void StreamReporter::report(const DomainBox &box)
{
   if (vertical_)
   {
      size_t lmax = maxNameLength();

      for (size_t i = 0; i < ents_.size(); ++i)
      {
         auto e = ents_.get(i);
         (*os_) << e->name();

         size_t n = e->name().length();
         for (size_t i = 0; i < lmax - n; ++i)
            (*os_) << ' ';

         (*os_) << " = " << (*e->domain(box)) << std::endl;
      }
   }
   else
   {
      (*os_) << '(';

      for (size_t i = 0; i < ents_.size(); ++i)
      {
         if (i != 0)
            (*os_) << ", ";
         auto e = ents_.get(i);
         (*os_) << e->name() << " = " << (*e->domain(box));
      }
      (*os_) << ')';
   }
}

} // namespace realpaver
