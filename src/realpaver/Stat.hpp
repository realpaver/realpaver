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
 * @file   Stat.hpp
 * @brief  Statistics on solving processes
 * @author Laurent Granvilliers
 * @date   2022-5-6
 */

#ifndef REALPAVER_STAT_HPP
#define REALPAVER_STAT_HPP

#include <string>
#include <unordered_map>
#include "realpaver/Timer.hpp"

namespace realpaver {

/**
 * @brief Statistics on solving processes.
 *
 * The design of this class follows the singleton design pattern.
 */
class Stat {
public:
   /**
    * @brief Gets an integral quantity.
    * 
    * @param name string representation
    * @return a reference to the quantity
    *
    * Throws an exception if the given name is not registered
    */
   static int& getIntStat(const std::string& name);

   /**
    * @brief Gets a timer.
    * 
    * @param name string representation
    * @return a reference to the timer
    *
    * Throws an exception if the given name is not registered
    */
   static Timer& getTimeStat(const std::string& name);

   /// Resets all the statistics
   static void reset();

   /// Output on a stream
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
