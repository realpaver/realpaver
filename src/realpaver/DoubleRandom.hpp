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
 * @file   DoubleRandom.hpp
 * @brief  Generator of pseudo-random floating-point numbers.
 * @author Laurent Granvilliers
 * @date   17 Nov 2025
 */

#ifndef REALPAVER_DOUBLE_RANDOM_HPP
#define REALPAVER_DOUBLE_RANDOM_HPP

#include <random>

namespace realpaver {

/**
 * @brief Generator of pseudo-random integers.
 *
 * It produces a sequence of pseudo-random values of type double that are
 * uniformly distributed in a given range, using the methods next() and
 * next(a, b).
 */
class DoubleRandom {
public:
   /**
    * @brief Constructor.
    *
    * The seed used to initialize the generator is
    * obtained from the system time.
    */
   DoubleRandom();

   /**
    * @brief Constructor.
    *
    * The seed used to initialize the generator is
    * also given as input.
    */
   DoubleRandom(unsigned seed);

   /**
    * @brief Constructor.
    *
    * The generated numbers lie in [lo, up]. The seed used to initialize the generator is
    * obtained from the system time.
    */
   DoubleRandom(double lo, double up);

   /**
    * @brief Constructor.
    *
    * The generated numbers lie in [lo, up]. The seed used to initialize the generator is
    * also given as input.
    */
   DoubleRandom(double lo, double up, unsigned seed);

   /// Default copy constructor
   DoubleRandom(const DoubleRandom &) = default;

   /// Default assignment
   DoubleRandom &operator=(const DoubleRandom &) = default;

   /// Default destructor
   ~DoubleRandom() = default;

   /// Returns a pseudo-random integer between 0 and the greatest int value
   double next();

   /// Resets the distribution object
   void reset();

private:
   std::default_random_engine gen_;             // generator
   std::uniform_real_distribution<double> dis_; // distribution
};

} // namespace realpaver

#endif
