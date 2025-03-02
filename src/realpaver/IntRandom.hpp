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
 * @file   IntRandom.hpp
 * @brief  Generator of pseudo-random integers.
 * @author Laurent Granvilliers
 * @date   2024-26-11
 */

#ifndef REALPAVER_INT_RANDOM_HPP
#define REALPAVER_INT_RANDOM_HPP

#include <random>

namespace realpaver {

/**
 * @brief Generator of pseudo-random integers.
 *
 * It produces a sequence of pseudo-random values of type int that are
 * uniformly distributed in a given range, using the methods next(),
 * next(a, b) and nextBit().
 */
class IntRandom {
public:
   /**
    * @brief Constructor.
    *
    * The seed used to initialize the generator is obtained from the
    * system time.
    */
   IntRandom();

   /// Constructor given a seed used to initialize the generator
   IntRandom(unsigned seed);

   /// Default copy constructor
   IntRandom(const IntRandom&) = default;

   /// Default assignment
   IntRandom& operator=(const IntRandom&) = default;

   /// Default destructor
   ~IntRandom() = default;

   /// Returns a pseudo-random integer between 0 and the greatest int value
   int next();

   /// Returns a pseudo-random integer between a and b
   int next(int a, int b);

   /// Returns a pseudo-random boolean
   bool nextBool();

   /// Resets the distribution object
   void reset();

private:
   std::default_random_engine gen_;          // generator
   std::uniform_int_distribution<int> dis_;  // distribution
};

} // namespace

#endif
