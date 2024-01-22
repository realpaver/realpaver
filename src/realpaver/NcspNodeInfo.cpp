///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspNodeInfo.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, NcspNodeInfoType typ)
{
   switch(typ)
   {
      case NcspNodeInfoType::SplitVar:    return os << "split variable";
      case NcspNodeInfoType::SmearSumRel: return os << "smear sum relative";
   }
   return os;   
}

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

NcspNodeInfoSSR::NcspNodeInfoSSR(std::shared_ptr<IntervalSmearSumRel> obj)
      : NcspNodeInfo(NcspNodeInfoType::SmearSumRel),
        obj_(obj),
        sv_(nullptr)
{}

NcspNodeInfoSSR::NcspNodeInfoSSR(const NcspNodeInfoSSR& other)
      : NcspNodeInfo(NcspNodeInfoType::SmearSumRel),
        obj_(other.obj_),
        sv_(nullptr)
{}

NcspNodeInfoSSR::~NcspNodeInfoSSR()
{
   if (sv_ != nullptr) delete sv_;
}

Scope NcspNodeInfoSSR::scope() const
{
   return obj_->scope();
}

bool NcspNodeInfoSSR::isSorted() const
{
   return sv_ != nullptr;
}

void NcspNodeInfoSSR::calculate(const IntervalBox& B)
{
   obj_->calculate(B);
}

double NcspNodeInfoSSR::getSmearSumRel(const Variable& v) const
{
   return obj_->getSmearSumRel(v);
}

void NcspNodeInfoSSR::sort()
{
   // already sorted => nothing to do
   if (sv_ != nullptr) return;

   // copies the values from the shared object
   sv_ = new std::vector<Item>();
   for (size_t i=0; i<nbVars(); ++i)
   {
      Variable v = scope().var(i);
      Item itm = { v, obj_->getSmearSumRel(v) };
      sv_->push_back(itm);
   }

   // sorting
   std::sort(sv_->begin(), sv_->end(), CompItem());
}

Variable NcspNodeInfoSSR::getSortedVar(size_t i) const
{
   ASSERT(sv_ != nullptr, "Vector of smear sum relative values not sorted");

   return (*sv_)[i].var;
}

size_t NcspNodeInfoSSR::nbVars() const
{
   return obj_->scope().size();
}

///////////////////////////////////////////////////////////////////////////////

void NcspNodeInfoMap::insert(int index, std::shared_ptr<NcspNodeInfo> info)
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

size_t NcspNodeInfoMap::size() const
{
   return map_.size();
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

bool NcspNodeInfoMap::hasInfo(int index, NcspNodeInfoType typ) const
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
