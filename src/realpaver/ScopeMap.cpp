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
 * @file   ScopeMap.cpp
 * @brief  Map between two sets of integers
 * @author Laurent Granvilliers
 * @date   2022-5-6
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/ScopeMap.hpp"

namespace realpaver {

AbstractScopeMap::~AbstractScopeMap()
{}

/*----------------------------------------------------------------------------*/

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

size_t ScopeIdMap::hashCode() const
{
   size_t h = 0;

   for (size_t i=1; i<=last_; ++i)
      h = hash2(h, i);

   return h;
}

/*----------------------------------------------------------------------------*/

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

size_t ScopeSeqMap::hashCode() const
{
   size_t h = first_;

   for (size_t i=first_+1; i<=last_; ++i)
      h = hash2(h, i);

   return h;
}

/*----------------------------------------------------------------------------*/

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
   m_.insert(std::make_pair(id, next_));
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

size_t ScopeHashMap::hashCode() const
{
   auto it = m_.begin();

   if (it == m_.end())
      return 0;

   size_t h = it->first;
   ++it;

   while (it != m_.end())
   {
      h = hash2(h, it->first);
      ++it;
   }

   return h;
}

} // namespace
