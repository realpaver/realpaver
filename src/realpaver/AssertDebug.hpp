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
 * @file   AssertDebug.hpp
 * @brief  Macros for assertion checking and debugging
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

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
