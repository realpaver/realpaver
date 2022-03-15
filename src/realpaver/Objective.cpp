///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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

Objective minimize(Term f)
{
   return Objective(f, true);
}

Objective maximize(Term f)
{
   return Objective(f, false);
}

std::ostream& operator<<(std::ostream& os, Objective obj)
{
   if (obj.isMaximization())
      os << "maximize ";
   else
      os << "minimize ";

   return os << obj.getTerm();
}

} // namespace
