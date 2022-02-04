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

Objective::Objective(const Term& f, bool minimization)
      : f_(f), minimization_(minimization)
{}

Term Objective::getTerm() const
{
   return f_;
}

bool Objective::isConstant() const
{
   return f_.isConstant();
}

bool Objective::isLinear() const
{
   return f_.isLinear();
}

bool Objective::isMinimization() const
{
   return minimization_;
}

bool Objective::isMaximization() const
{
   return !minimization_; 
}

Objective minimize(const Term& f)
{
   return Objective(f, true);
}

Objective maximize(const Term& f)
{
   return Objective(f, false);
}

std::ostream& operator<<(std::ostream& os, const Objective& obj)
{
   if (obj.isMaximization())
      os << "maximize ";
   else
      os << "minimize ";

   return os << obj.getTerm();
}

} // namespace
