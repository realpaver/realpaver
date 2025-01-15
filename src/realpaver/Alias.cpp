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
 * @file   Alias.cpp
 * @brief  Alias of term in a problem
 * @author Laurent Granvilliers
 * @date   2024-12-19
 */

#include "realpaver/Alias.hpp"

namespace realpaver {

AliasRep::AliasRep(const std::string& name, Term t)
      : name_(name),
        t_(t),
        scop_()
{
   t.makeScope(scop_);

   THROW_IF(scop_.isEmpty(),
      "Definition of alias " << name << " with constant term " << t);
}

std::string AliasRep::name() const
{
   return name_;
}

Term AliasRep::term() const
{
   return t_;
}

Scope AliasRep::scope() const
{
   return scop_;
}

/*----------------------------------------------------------------------------*/

Alias::Alias(const std::string& name, Term t)
   : rep_(std::make_shared<AliasRep>(name, t))
{}

std::string Alias::name() const
{
   return rep_->name();
}

Term Alias::term() const
{
   return rep_->term();
}

Scope Alias::scope() const
{
   return rep_->scope();
}

std::ostream& operator<<(std::ostream& os, const Alias& a)
{
   os << a.name() << " := " << a.term();
   return os;
}

} // namespace
