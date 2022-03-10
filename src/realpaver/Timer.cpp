///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/Timer.hpp"

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

} // namespace
