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
 * @file   IntRandom.cpp
 * @brief  Generator of pseudo-random numbers.
 * @author Laurent Granvilliers
 * @date   2024-26-11
 */

#include "realpaver/IntRandom.hpp"
#include <chrono>
#include <limits>

namespace realpaver {

IntRandom::IntRandom()
    : gen_(std::chrono::steady_clock::now().time_since_epoch().count())
    , dis_(0, std::numeric_limits<int>::max())
{
}

IntRandom::IntRandom(unsigned seed)
    : gen_(seed)
    , dis_(0, std::numeric_limits<int>::max())
{
}

int IntRandom::next()
{
   return dis_(gen_);
}

int IntRandom::next(int a, int b)
{
   return next() % (b - a + 1);
}

bool IntRandom::nextBool()
{
   return next() % 2 == 1;
}

void IntRandom::reset()
{
   dis_.reset();
}

} // namespace realpaver
