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
 * @date   2024-4-11
 */

#ifndef REALPAVER_ASSERT_DEBUG_HPP
#define REALPAVER_ASSERT_DEBUG_HPP

#include "realpaver/Exception.hpp"
#include <iostream>

namespace realpaver {

#if REALPAVER_ASSERT_ON
#define REALPAVER_ASSERT(cond, msg)                                                      \
   if (!(cond))                                                                          \
   THROW(msg)
#else
#define REALPAVER_ASSERT(cond, msg)
#endif

#if REALPAVER_DEBUG_ON
#define REALPAVER_DEBUG(msg) std::cerr << msg << std::endl
#else
#define REALPAVER_DEBUG(msg)
#endif

} // namespace realpaver

#endif
