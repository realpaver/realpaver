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
 * @file   CSPContext.cpp
 * @brief  Search context for CSPs
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/CSPContext.hpp"
#include "realpaver/AssertDebug.hpp"

namespace realpaver {

std::ostream &operator<<(std::ostream &os, CSPNodeInfoType typ)
{
   switch (typ)
   {
   case CSPNodeInfoType::SplitVar:
      return os << "split variable";
   case CSPNodeInfoType::NbCID:
      return os << "nb CID";
   default:
      os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

CSPNodeInfo::CSPNodeInfo(CSPNodeInfoType typ)
    : typ_(typ)
{
}

CSPNodeInfo::~CSPNodeInfo()
{
}

CSPNodeInfoType CSPNodeInfo::getType() const
{
   return typ_;
}

/*----------------------------------------------------------------------------*/

CSPNodeInfoVar::CSPNodeInfoVar(Variable v)
    : CSPNodeInfo(CSPNodeInfoType::SplitVar)
    , v_(v)
{
}

Variable CSPNodeInfoVar::getVar() const
{
   return v_;
}

/*----------------------------------------------------------------------------*/

CSPNodeInfoCID::CSPNodeInfoCID(int nbcid)
    : CSPNodeInfo(CSPNodeInfoType::NbCID)
    , nbcid_(nbcid)
{
}

size_t CSPNodeInfoCID::getNbCID() const
{
   return nbcid_;
}

void CSPNodeInfoCID::setNbCID(size_t nbcid)
{
   nbcid_ = nbcid;
}

/*----------------------------------------------------------------------------*/

void CSPContext::insert(int index, std::shared_ptr<CSPNodeInfo> info)
{
   ASSERT(!hasInfo(index, info->getType()),
          "Info '" << info->getType() << "' already present in the map for node "
                   << index);

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

size_t CSPContext::size() const
{
   return map_.size();
}

void CSPContext::remove(int index)
{
   map_.erase(index);
}

std::shared_ptr<CSPNodeInfo> CSPContext::getInfo(int index, CSPNodeInfoType typ) const
{
   auto it = map_.find(index);
   if (it == map_.end())
      return nullptr;

   else
   {
      for (const auto &info : it->second)
         if (info->getType() == typ)
            return info;

      return nullptr;
   }
}

bool CSPContext::hasInfo(int index, CSPNodeInfoType typ) const
{
   auto it = map_.find(index);
   if (it == map_.end())
      return false;

   else
   {
      for (const auto &info : it->second)
         if (info->getType() == typ)
            return true;

      return false;
   }
}

} // namespace realpaver
