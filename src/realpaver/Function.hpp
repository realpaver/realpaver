///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_FUNCTION_HPP
#define REALPAVER_FUNCTION_HPP

#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a base class for functions.
///////////////////////////////////////////////////////////////////////////////
class Function {
public:
   /// Virtual destructor
   virtual ~Function();

   /// @return the scopeof this, i.e. the set of variables
   virtual Scope funScope() const = 0;

   /// @return the number of variables in this
   virtual size_t funArity() const = 0;
};

} // namespace

#endif
