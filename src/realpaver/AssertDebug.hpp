///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_ASSERT_DEBUG_HPP
#define REALPAVER_ASSERT_DEBUG_HPP

#include <iostream>
#include "realpaver/Exception.hpp"

namespace realpaver {

#if ASSERT_ON
#   define ASSERT(cond, msg)                                \
      if (!(cond))                                          \
         THROW(msg)
#else
#   define ASSERT(cond, msg)
#endif

#if DEBUG_ON
#   define DEBUG(msg)  std::cerr << msg << std::endl
#else
#   define DEBUG(msg)
#endif

} // namespace

#endif
