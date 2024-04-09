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
 * @file   NcspContext.cpp
 * @brief  Search context for NCSPs
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#include <algorithm>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspContext.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, NcspNodeInfoType typ)
{
   switch(typ)
   {
      case NcspNodeInfoType::SplitVar: return os << "split variable";
      case NcspNodeInfoType::NbCID:    return os << "nb CID";
      default: os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

NcspNodeInfo::NcspNodeInfo(NcspNodeInfoType typ)
      : typ_(typ)
{}

NcspNodeInfo::~NcspNodeInfo()
{}

NcspNodeInfoType NcspNodeInfo::getType() const
{
   return typ_;
}

/*----------------------------------------------------------------------------*/

NcspNodeInfoVar::NcspNodeInfoVar(Variable v)
      : NcspNodeInfo(NcspNodeInfoType::SplitVar),
        v_(v)
{}

Variable NcspNodeInfoVar::getVar() const
{
   return v_;
}

/*----------------------------------------------------------------------------*/

NcspNodeInfoCID::NcspNodeInfoCID(int nbcid)
      : NcspNodeInfo(NcspNodeInfoType::NbCID),
        nbcid_(nbcid)
{}

size_t NcspNodeInfoCID::getNbCID() const
{
   return nbcid_;
}

void NcspNodeInfoCID::setNbCID(size_t nbcid)
{
   nbcid_ = nbcid;
}

/*----------------------------------------------------------------------------*/

void NcspContext::insert(int index, std::shared_ptr<NcspNodeInfo> info)
{
   ASSERT(!hasInfo(index, info->getType()),
          "Info '" << info->getType()
                   << "' already present in the map for node " << index);
   
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

size_t NcspContext::size() const
{
   return map_.size();
}

void NcspContext::remove(int index)
{
   map_.erase(index);
}

std::shared_ptr<NcspNodeInfo>
NcspContext::getInfo(int index, NcspNodeInfoType typ) const
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

bool NcspContext::hasInfo(int index, NcspNodeInfoType typ) const
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
