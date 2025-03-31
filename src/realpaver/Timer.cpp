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
 * @file   Timer.cpp
 * @brief  Timer measuring computation times
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/Timer.hpp"

namespace realpaver {

Timer::Timer()
    : on_(false)
    , start_()
    , total_()
{
}

void Timer::start()
{
   if (!on_)
   {
      start_ = std::chrono::system_clock::now();
      on_ = true;
   }
}

void Timer::stop()
{
   if (on_)
   {
      PointType end = std::chrono::system_clock::now();
      total_ += (end - start_);
      on_ = false;
   }
}

void Timer::reset()
{
   on_ = false;
   total_ = DurationType(0);
}

double Timer::elapsedTime() const
{
   DurationType d = total_;
   if (on_)
   {
      PointType now = std::chrono::system_clock::now();
      d += (now - start_);
   }
   return std::chrono::duration_cast<UnitType>(d).count() / 1000.0;
}

} // namespace realpaver
