// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_SCOPE_MAP_HPP
#define REALPAVER_SCOPE_MAP_HPP

#include <unordered_map>

namespace realpaver {

/*****************************************************************************
 * Abstract class of mappings between two sets of integers.
 * 
 * Any set {i0, i1, ..., ik} maps to the interval 0..k.
 *****************************************************************************/
class AbstractScopeMap {
public:
   // destructor
   virtual ~AbstractScopeMap();

   // index of id in 0..k
   virtual size_t index(size_t id) const = 0;

   // returns a clone
   virtual AbstractScopeMap* clone() const = 0;
};

/*****************************************************************************
 * Class of scope mappings such that {i0, i1, ..., ik} is equal to 0..k.
 * 
 * Hence, this class represents the identity mapping.
 *****************************************************************************/
class ScopeIdMap : public AbstractScopeMap {
public:
   // constructors
   ScopeIdMap(size_t last);
   ScopeIdMap(const ScopeIdMap& other);

   // Override
   size_t index(size_t id) const;
   ScopeIdMap* clone() const;

private:
   size_t last_;
};

/*****************************************************************************
 * Class of scope mappings such that {i0, i1, ..., ik} is equal to
 * a sequence i(l), i(l+1), ..., i(l+k).
 *****************************************************************************/
class ScopeSeqMap : public AbstractScopeMap {
public:
   // constructor
   ScopeSeqMap(size_t first, size_t last);
   ScopeSeqMap(const ScopeSeqMap& other);

   // Override
   size_t index(size_t id) const;
   ScopeSeqMap* clone() const;

private:
   size_t first_, last_;
};

/*****************************************************************************
 * Class of scope mappings based on an hash table.
 *****************************************************************************/
class ScopeHashMap : public AbstractScopeMap {
public:
   // constructors
   ScopeHashMap();
   ScopeHashMap(const ScopeHashMap& other);

   // Override
   size_t index(size_t id) const;
   ScopeHashMap* clone() const;

   // inserts a variable 'id'
   void insert(size_t id);

private:
   std::unordered_map<size_t,size_t> m_;  // hash map
   size_t next_;     // value of the next variable inserted
};

} // namespace

#endif
