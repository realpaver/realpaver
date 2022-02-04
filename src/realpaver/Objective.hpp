///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_OBJECTIVE_HPP
#define REALPAVER_OBJECTIVE_HPP

#include "realpaver/term.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an objective function.
///////////////////////////////////////////////////////////////////////////////
class Objective {
public:
   /// Default copy constructor
   Objective(const Objective&) = default;

   /// Default assignment operator
   Objective& operator=(const Objective&) = default;

   /// Default destructor
   ~Objective() = default;

   /// @return the expression of this
   Term getTerm() const;

   /// @return true if this is constant (variable free)
   bool isConstant() const;

   /// @return true if this is linear
   bool isLinear() const;

   /// @return true if this has to be minimized
   bool isMinimization() const;

   /// @return true if this has to be maximized
   bool isMaximization() const;

   friend Objective minimize(const Term& f);
   friend Objective maximize(const Term& f);

private:
   Term f_;
   bool minimization_;

   // Creates an objective function
   Objective(const Term& f, bool minimization = true);
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Objective& obj);

/// Creates an objective function
/// @param f expression
/// @return minimize f
Objective minimize(const Term& f);

/// Creates an objective function
/// @param f expression
/// @return maximize f
Objective maximize(const Term& f);

} // namespace

#endif
