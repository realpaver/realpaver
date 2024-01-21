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

NcspNodeInfoSSR::NcspNodeInfoSSR(IntervalFunctionVector F)
      : NcspNodeInfo(NcspNodeInfoType::SmearSumRel),
        F_(F),
        scop_(F.scope()),
        ssr_(),
        sv_(nullptr)
{
   for (size_t i=0; i<F_.nbVars(); ++i)
   {
      Item itm = { scop_.var(i), 0.0 };
      ssr_.push_back(itm);
   }
}

NcspNodeInfoSSR::NcspNodeInfoSSR(const NcspNodeInfoSSR& other)
      : NcspNodeInfo(NcspNodeInfoType::SmearSumRel),
        F_(other.F_),
        scop_(other.scop_),
        ssr_(other.ssr_),
        sv_(nullptr)
{}

NcspNodeInfoSSR::~NcspNodeInfoSSR()
{
   if (sv_ != nullptr) delete sv_;
}

Scope NcspNodeInfoSSR::scope() const
{
   return scop_;
}

bool NcspNodeInfoSSR::isSorted() const
{
   return sv_ != nullptr;
}

void NcspNodeInfoSSR::calculateSSR(const IntervalBox& B)
{
   IntervalMatrix jac(F_.nbFuns(), F_.nbVars());
   RealMatrix S(F_.nbFuns(), F_.nbVars(), 0.0);

   // calculates the partial derivatives
   F_.diff(B, jac);

   // calculates the relative smear values
   for (size_t i=0; i<F_.nbFuns(); ++i)
   {
      double sum = 0.0;
      for (size_t j=0; j<F_.nbVars(); ++j)
      {
         const auto& v = scop_.var(j);
         double smear = jac.get(i, j).mag() * B.get(v).width();
         S.set(i, j, smear);
         sum += smear;
      }
      if (sum != 0.0)
      {
         for (size_t j=0; j<F_.nbVars(); ++j)
            S.set(i, j, S.get(i, j) / sum);
      }
   }

   // calculates the smearRelSum values
   for (size_t j=0; j<F_.nbVars(); ++j)
   {
      ssr_[j].val = 0.0;

      for (size_t i=0; i<F_.nbFuns(); ++i)
         ssr_[j].val += S.get(i, j);
   }
}

double NcspNodeInfoSSR::getSSR(const Variable& v) const
{
   return ssr_[scop_.index(v)].val;
}

void NcspNodeInfoSSR::sort()
{
   if (sv_ == nullptr) return;
   sv_ = new std::vector<Item>(ssr_);
   std::sort(sv_->begin(), sv_->end(), CompItem());
}

Variable NcspNodeInfoSSR::getSortedVar(size_t i) const
{
   ASSERT(sv_ != nullptr, "Vector of smear sum relative values not sorted");

   return (*sv_)[i].var;
}

size_t NcspNodeInfoSSR::nbVars() const
{
   return scop_.size();
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
