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
 * @file   DoubleRandom.cpp
 * @brief  Generator of pseudo-random floating-point numbers.
 * @author Laurent Granvilliers
 * @date   17 Nov 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/DoubleRandom.hpp"
#include <limits>

namespace realpaver {

DoubleRandom::DoubleRandom()
    : gen_(std::chrono::steady_clock::now().time_since_epoch().count())
    , dis_(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max())
{
}

DoubleRandom::DoubleRandom(unsigned seed)
    : gen_(seed)
    , dis_(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max())
{
}

DoubleRandom::DoubleRandom(double lo, double up)
    : gen_(std::chrono::steady_clock::now().time_since_epoch().count())
    , dis_(lo, up)
{
   ASSERT("lo <= up", "Bad initialization of the pseudo-randolm generator");
}

DoubleRandom::DoubleRandom(double lo, double up, unsigned seed)
    : gen_(seed)
    , dis_(lo, up)
{
   ASSERT("lo <= up", "Bad initialization of the pseudo-randolm generator");
}

double DoubleRandom::next()
{
   return dis_(gen_);
}

void DoubleRandom::reset()
{
   dis_.reset();
}

} // namespace realpaver
