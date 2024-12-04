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
 * @file   Objective.cpp
 * @brief  Objective function
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Objective.hpp"

namespace realpaver {

ObjectiveRep::ObjectiveRep(Term f, bool minimization)
      : f_(f),
        minimization_(minimization)
{}

Term ObjectiveRep::getTerm() const
{
   return f_;
}

bool ObjectiveRep::isConstant() const
{
   return f_.isConstant();
}

bool ObjectiveRep::dependsOn(const Variable& v) const
{
   return f_.dependsOn(v);
}

bool ObjectiveRep::isLinear() const
{
   return f_.isLinear();
}

bool ObjectiveRep::isMinimization() const
{
   return minimization_;
}

bool ObjectiveRep::isMaximization() const
{
   return !minimization_; 
}

/*----------------------------------------------------------------------------*/

Objective::Objective(Term f, bool minimization)
      : rep_(std::make_shared<ObjectiveRep>(f, minimization))
{}

Term Objective::getTerm() const
{
   return rep_->getTerm();
}

bool Objective::isConstant() const
{
   return rep_->isConstant();
}

bool Objective::dependsOn(const Variable& v) const
{
   return rep_->dependsOn(v);
}

bool Objective::isLinear() const
{
   return rep_->isLinear();
}

bool Objective::isMinimization() const
{
   return rep_->isMinimization();
}

bool Objective::isMaximization() const
{
   return rep_->isMaximization(); 
}

Objective MIN(Term f)
{
   return Objective(f, true);
}

Objective MAX(Term f)
{
   return Objective(f, false);
}

std::ostream& operator<<(std::ostream& os, Objective obj)
{
   if (obj.isMaximization())
      os << "MAX ";
   else
      os << "MIN ";

   return os << obj.getTerm();
}

} // namespace
