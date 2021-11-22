// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_timer.hpp"

namespace realpaver {

Timer::Timer() : on_(false), start_(), total_()
{}

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

size_t Timer::elapsedTime() const
{
   DurationType d = total_;
   if (on_)
   {
      PointType now = std::chrono::system_clock::now();
      d += (now - start_);
   }
   return std::chrono::duration_cast<UnitType>(d).count();      
}

} // namespace
