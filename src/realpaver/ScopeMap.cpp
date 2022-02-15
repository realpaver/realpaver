///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/ScopeMap.hpp"

namespace realpaver {

AbstractScopeMap::~AbstractScopeMap()
{}

///////////////////////////////////////////////////////////////////////////////

ScopeIdMap::ScopeIdMap(size_t last)
      : AbstractScopeMap(),
        last_(last)
{}

size_t ScopeIdMap::index(size_t id) const
{
   ASSERT(id <= last_, "Bad access in a scope index map");

   return id;
}

ScopeIdMap::ScopeIdMap(const ScopeIdMap& other) :
   last_(other.last_)
{}

ScopeIdMap* ScopeIdMap::clone() const
{
   return new ScopeIdMap(*this);
}

///////////////////////////////////////////////////////////////////////////////

ScopeSeqMap::ScopeSeqMap(size_t first, size_t last)
      : AbstractScopeMap(),
        first_(first),
        last_(last)
{}

size_t ScopeSeqMap::index(size_t id) const
{
   ASSERT(id >= first_ && id <= last_, "Bad access in a scope index map");

   return id - first_;
}

ScopeSeqMap::ScopeSeqMap(const ScopeSeqMap& other)
      : first_(other.first_),
        last_(other.last_)
{}

ScopeSeqMap* ScopeSeqMap::clone() const
{
   return new ScopeSeqMap(*this);
}

///////////////////////////////////////////////////////////////////////////////

ScopeHashMap::ScopeHashMap()
      : AbstractScopeMap(),
        m_(),
        next_(0)
{}

size_t ScopeHashMap::index(size_t id) const
{
   auto it = m_.find(id);

   ASSERT(it != m_.end(), "Bad access in a scope index map");

   return it->second;
}

void ScopeHashMap::insert(size_t id)
{
   m_.insert(std::make_pair(id,next_));
   next_++;
}

ScopeHashMap::ScopeHashMap(const ScopeHashMap& other)
      : m_(other.m_),
        next_(other.next_)
{}

ScopeHashMap* ScopeHashMap::clone() const
{
   return new ScopeHashMap(*this);
}

} // namespace
