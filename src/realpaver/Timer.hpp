///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_TIMER_HPP
#define REALPAVER_TIMER_HPP

#include <chrono>

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a timer measuring computation times in seconds.
///////////////////////////////////////////////////////////////////////////////
class Timer {
public:
   /// Creates a timer
   Timer();

   /// Starts this timer
   void start();

   /// Stops this timer
   ///
   /// Accumulates the computation time from the last start
   void stop();

   /// Resets this timer
   void reset();

   /// @return the accumulated elapsed time in seconds
   size_t elapsedTime() const;

private:
   typedef std::chrono::time_point<std::chrono::system_clock> PointType;
   typedef std::chrono::duration<double> DurationType;
   typedef std::chrono::seconds UnitType;

   bool on_;                  // true if this is started
   PointType start_;          // last starting time point
   DurationType total_;       // accumulated elapsed time
};

} // namespace

#endif
