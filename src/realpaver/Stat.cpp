///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Exception.hpp"
#include "realpaver/Stat.hpp"

namespace realpaver {

Stat Stat::instance_;

Stat::Stat()
      : intmap_()
{
#if STAT_ON
   intmap_.insert(std::make_pair("ITV_OP_ADD",                0));
#endif
}

int& Stat::getIntStat(const std::string& name)
{
   auto it = instance_.intmap_.find(name);

   if (it == instance_.intmap_.end())
      THROW("'" << name << "' is not registered as a statistics");

   return it->second;
}

void Stat::reset()
{
#if STAT_ON
   for (auto it : instance_.intmap_)
      it->second = 0;
#endif
}

} // namespace
