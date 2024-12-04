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
 * @file   Timer.hpp
 * @brief  Timer measuring computation times
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_TIMER_HPP
#define REALPAVER_TIMER_HPP

#include <chrono>

namespace realpaver {

/// Timer measuring computation times in seconds
class Timer {
public:
   /// Creates a timer
   Timer();

   /// Starts this timer
   void start();

   /**
    * @brief Stops this timer
    *
    * Accumulates the computation time from the last start
    */
   void stop();

   /// Resets this timer
   void reset();

   /// Returns the accumulated elapsed time in seconds
   double elapsedTime() const;

private:
   using PointType    = std::chrono::time_point<std::chrono::system_clock>;
   using DurationType = std::chrono::duration<double>;
   using UnitType     = std::chrono::milliseconds;

   bool on_;                  // true if this is started
   PointType start_;          // last starting time point
   DurationType total_;       // accumulated elapsed time
};

} // namespace

#endif
