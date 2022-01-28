// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_TIMER_HPP
#define REALPAVER_TIMER_HPP

#include <chrono>

namespace realpaver {

/*****************************************************************************
 * Class of timers measuring computation times in milliseconds.
 *****************************************************************************/
class Timer {
public:
   // constructor
   Timer();

   // starts this timer
   void start();

   // stops this timer and accumulates the computation time from
   // the last start
   void stop();

   // resets this timer
   void reset();

   // returns the accumulated elapsed time
   size_t elapsedTime() const;

private:
   typedef std::chrono::time_point<std::chrono::system_clock> PointType;
   typedef std::chrono::duration<double,std::milli> DurationType;
   typedef std::chrono::milliseconds UnitType;

   bool on_;                  // true if started
   PointType start_;          // last starting time point
   DurationType total_;       // accumulated elapsed time
};

} // namespace

#endif
