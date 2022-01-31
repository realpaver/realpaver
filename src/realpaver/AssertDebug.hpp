///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, a reliable interval solver of nonlinear   //
//                                 constraint satisfaction and optimization  //
//                                 problems over the real numbers.           //
//                                                                           //
// Copyright (c) 1999-2003 Institut de Recherche en Informatique de Nantes   //
//               2004-2016 Laboratoire d'Informatique de Nantes Atlantique   //
//               2017-2022 Laboratoire des Sciences du Numérique de Nantes   //
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
