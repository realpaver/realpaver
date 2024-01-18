///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "AssertDebug.hpp"
#include "NcspNodeInfoMap.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////

NcspNodeInfo::NcspNodeInfo(NcspNodeInfoType typ)
      : typ_(typ)
{}

NcspNodeInfo::~NcspNodeInfo()
{}

NcspNodeInfoType NcspNodeInfo::getType() const
{
   return typ_;
}

///////////////////////////////////////////////////////////////////////////////

NcspNodeInfoVar::NcspNodeInfoVar(Variable v)
      : NcspNodeInfo(NcspNodeInfoType::SplitVar),
        v_(v)
{}

Variable NcspNodeInfoVar::getVar() const
{
   return v_;
}

///////////////////////////////////////////////////////////////////////////////

void NcspNodeInfoMap::insert(int index, std::shared_ptr<NcspNodeInfo> info)
{
   ASSERT(!hasInfo(index, info->getType()), "Bad insertion in a node info map");
   
   auto it = map_.find(index);
   if (it == map_.end())
   {
      ListType l;
      l.push_back(info);
      map_.insert(std::make_pair(index, l));
   }
   else
   {
      it->second.push_back(info);
   }
}

void NcspNodeInfoMap::remove(int index)
{
   map_.erase(index);
}

std::shared_ptr<NcspNodeInfo>
NcspNodeInfoMap::getInfo(int index, NcspNodeInfoType typ) const
{
   auto it = map_.find(index);
   if (it == map_.end())
      return nullptr;

   else
   {
      for (const auto& info : it->second)
         if (info->getType() == typ)
            return info;

      return nullptr;
   }
}

bool NcspNodeInfoMap::hasInfo(int index,  NcspNodeInfoType typ) const
{
   auto it = map_.find(index);
   if (it == map_.end())
      return false;

   else
   {
      for (const auto& info : it->second)
         if (info->getType() == typ)
            return true;

      return false;
   }
}

} // namespace
