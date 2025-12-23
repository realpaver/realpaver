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
 * @file   NodeContext.cpp
 * @brief  Context for search nodes
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NodeContext.hpp"

namespace realpaver {

std::ostream &operator<<(std::ostream &os, NodeInfoType typ)
{
   switch (typ)
   {
   case NodeInfoType::SplitVar:
      return os << "split variable";
   case NodeInfoType::NbCID:
      return os << "nb CID";
   default:
      os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

NodeInfo::NodeInfo(NodeInfoType typ)
    : typ_(typ)
{
}

NodeInfo::~NodeInfo()
{
}

NodeInfoType NodeInfo::getType() const
{
   return typ_;
}

/*----------------------------------------------------------------------------*/

NodeInfoVar::NodeInfoVar(Variable v)
    : NodeInfo(NodeInfoType::SplitVar)
    , v_(v)
{
}

Variable NodeInfoVar::getVar() const
{
   return v_;
}

/*----------------------------------------------------------------------------*/

NodeInfoCID::NodeInfoCID(int nbcid)
    : NodeInfo(NodeInfoType::NbCID)
    , nbcid_(nbcid)
{
}

size_t NodeInfoCID::getNbCID() const
{
   return nbcid_;
}

void NodeInfoCID::setNbCID(size_t nbcid)
{
   nbcid_ = nbcid;
}

/*----------------------------------------------------------------------------*/

void NodeContext::insert(int index, std::shared_ptr<NodeInfo> info)
{
   REALPAVER_ASSERT(!hasInfo(index, info->getType()),
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

size_t NodeContext::size() const
{
   return map_.size();
}

void NodeContext::remove(int index)
{
   map_.erase(index);
}

std::shared_ptr<NodeInfo> NodeContext::getInfo(int index, NodeInfoType typ) const
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

bool NodeContext::hasInfo(int index, NodeInfoType typ) const
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
