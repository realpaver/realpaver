// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/scope_map.hpp"

namespace realpaver {

AbstractScopeMap::~AbstractScopeMap()
{}

///////////////////////////////////////////////////////////////////////////////

ScopeIdMap::ScopeIdMap(size_t last) : last_(last)
{}

size_t ScopeIdMap::index(size_t id) const
{
   ASSERT(id <= last_, "bad access in a scope index map");

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

ScopeSeqMap::ScopeSeqMap(size_t first, size_t last) :
   first_(first), last_(last)
{}

size_t ScopeSeqMap::index(size_t id) const
{
   ASSERT(id >= first_ && id <= last_, "bad access in a scope index map");

   return id - first_;
}

ScopeSeqMap::ScopeSeqMap(const ScopeSeqMap& other) :
   first_(other.first_), last_(other.last_)
{}

ScopeSeqMap* ScopeSeqMap::clone() const
{
   return new ScopeSeqMap(*this);
}

///////////////////////////////////////////////////////////////////////////////

ScopeHashMap::ScopeHashMap(): m_(), next_(0)
{}

size_t ScopeHashMap::index(size_t id) const
{
   auto it = m_.find(id);

   ASSERT(it != m_.end(), "bad access in a scope index map");

   return it->second;
}

void ScopeHashMap::insert(size_t id)
{
   m_.insert(std::make_pair(id,next_));
   next_++;
}

ScopeHashMap::ScopeHashMap(const ScopeHashMap& other) :
   m_(other.m_),
   next_(other.next_)
{}

ScopeHashMap* ScopeHashMap::clone() const
{
   return new ScopeHashMap(*this);
}

} // namespace
