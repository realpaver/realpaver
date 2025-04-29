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
 * @file   Stat.cpp
 * @brief  Statistics on solving processes
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/Stat.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Exception.hpp"
#include <map>
#include <sstream>

namespace realpaver {

Stat Stat::instance_;
int Stat::space_ = 30;

Stat::Stat()
    : intmap_()
    , timmap_()
{
#if STAT_ON
   intmap_.insert(std::make_pair("ITV_OP_ADD", 0));
   intmap_.insert(std::make_pair("ITV_OP_ADD_PX", 0));
   intmap_.insert(std::make_pair("ITV_OP_ADD_PY", 0));
   intmap_.insert(std::make_pair("ITV_OP_ADD_PZ", 0));

   intmap_.insert(std::make_pair("ITV_OP_SUB", 0));
   intmap_.insert(std::make_pair("ITV_OP_SUB_PX", 0));
   intmap_.insert(std::make_pair("ITV_OP_SUB_PY", 0));
   intmap_.insert(std::make_pair("ITV_OP_SUB_PZ", 0));

   intmap_.insert(std::make_pair("ITV_OP_USUB", 0));
   intmap_.insert(std::make_pair("ITV_OP_USUB_PX", 0));
   intmap_.insert(std::make_pair("ITV_OP_USUB_PY", 0));

   intmap_.insert(std::make_pair("ITV_OP_MUL", 0));
   intmap_.insert(std::make_pair("ITV_OP_MUL_PX", 0));
   intmap_.insert(std::make_pair("ITV_OP_MUL_PY", 0));
   intmap_.insert(std::make_pair("ITV_OP_MUL_PZ", 0));

   timmap_.insert(std::make_pair("TIMER_POLYTOPE_HULL", Timer()));
   timmap_.insert(std::make_pair("TIMER_HC4", Timer()));
#endif
}

int &Stat::getIntStat(const std::string &name)
{
   auto it = instance_.intmap_.find(name);

   if (it == instance_.intmap_.end())
      THROW("'" << name << "' is not registered as a statistics");

   return it->second;
}

Timer &Stat::getTimeStat(const std::string &name)
{
   auto it = instance_.timmap_.find(name);

   if (it == instance_.timmap_.end())
      THROW("'" << name << "' is not registered as a statistics");

   return it->second;
}

void Stat::reset()
{
#if STAT_ON
   for (auto &it : instance_.intmap_)
      it.second = 0;

   for (auto &it : instance_.timmap_)
      it.second.reset();
#endif
}

void Stat::printSpace(const std::string &str, std::ostream &os)
{
   os << str;
   size_t n = space_ - str.length();
   for (size_t i = 0; i < n; ++i)
      os << ".";
}

void Stat::print(std::ostream &os)
{
   // ordered map
   std::map<std::string, std::string> map;

   // fills the map with int quantities
   for (auto &it : instance_.intmap_)
   {
      int val = it.second;
      if (val > 0)
      {
         std::ostringstream ostr;
         ostr << val;
         map.insert(std::make_pair(it.first, ostr.str()));
      }
   }

   // fills the map with timers
   for (auto &it : instance_.timmap_)
   {
      double val = it.second.elapsedTime();
      if (val > 0.0)
      {
         std::ostringstream ostr;
         ostr << val << " (s)";
         map.insert(std::make_pair(it.first, ostr.str()));
      }
   }

   // prints the map ordered by names
   for (auto &it : map)
   {
      printSpace(it.first, os);
      os << " : " << it.second << std::endl;
   }
}

} // namespace realpaver
