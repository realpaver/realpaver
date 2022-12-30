///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_STAT_HPP
#define REALPAVER_STAT_HPP

#include <string>
#include <unordered_map>
#include "realpaver/Timer.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This collects statistics on solving processes.
///
/// The design of this class follows the singleton design pattern.
///////////////////////////////////////////////////////////////////////////////
class Stat {
public:
   /// Gets an integral quantity
   /// @param name string representation
   /// @return a reference to the quantity
   ///
   /// Throws an exception if the given name is not registered
   static int& getIntStat(const std::string& name);

   /// Gets a timer
   /// @param name string representation
   /// @return a reference to the timer
   ///
   /// Throws an exception if the given name is not registered
   static Timer& getTimeStat(const std::string& name);

   /// Resets all the statistics
   static void reset();

   /// Prints the statistics on a stream
   /// @param os the output stream
   static void print(std::ostream& os);

private:
   static Stat instance_;     // singleton
   static int space_;
   
   std::unordered_map<std::string, int> intmap_;
   std::unordered_map<std::string, Timer> timmap_;

   static void printSpace(const std::string& str, std::ostream& os);

   Stat();
   ~Stat() = default;
   Stat(const Stat&) = delete;
   Stat& operator=(const Stat&) = delete;
};

} // namespace

#endif
