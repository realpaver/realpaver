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

   /// Resets all the statistics
   static void reset();

private:
   static Stat instance_;  // singleton

   std::unordered_map<std::string, int> intmap_;

   Stat();
   ~Stat() = default;
   Stat(const Stat&) = delete;
   Stat& operator=(const Stat&) = delete;
};

} // namespace

#endif
