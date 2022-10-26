///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_SCOPE_MAP_HPP
#define REALPAVER_SCOPE_MAP_HPP

#include <unordered_map>

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an abstract class of mappings between two sets of integers.
///
/// Any ordered set {i0, i1, ..., ik} maps to the interval 0..k, i.e. given
/// a mapping M, we have M(ij) = j for each j, 0 <= j <= k.
///
/// This is used by the Scope class in order to associate an ordered sequence
/// of integers with a set of variables.
///////////////////////////////////////////////////////////////////////////////
class AbstractScopeMap {
public:
   /// Destructor
   virtual ~AbstractScopeMap();

   /// Application of the mapping M
   /// @param id an integer
   /// @return M(id)
   virtual size_t index(size_t id) const = 0;

   /// @return a clone of this
   virtual AbstractScopeMap* clone() const = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// This is an identity mapping.
///
/// The domain of the mapping {i0, i1, ..., ik} is equal to 0..k.
///////////////////////////////////////////////////////////////////////////////
class ScopeIdMap : public AbstractScopeMap {
public:
   /// Creates an identity mapping
   /// @param last last element of the domain of the mapping
   /// 
   /// Mapping 0..last -> 0..last
   ScopeIdMap(size_t last);

   /// Copy constructor
   ScopeIdMap(const ScopeIdMap& other);

   ///@{
   size_t index(size_t id) const override;
   ScopeIdMap* clone() const override;
   ///@}

private:
   size_t last_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a mapping from a sequence of integers.
///
/// The domain of the mapping is equal to a sequence i(l), i(l+1), ..., i(l+k).
///////////////////////////////////////////////////////////////////////////////
class ScopeSeqMap : public AbstractScopeMap {
public:
   /// Creates a sequence mapping
   /// @param first first element of the domain of the mapping
   /// @param last last element of the domain of the mapping
   ScopeSeqMap(size_t first, size_t last);

   /// Copy constructor
   ScopeSeqMap(const ScopeSeqMap& other);

   ///@{
   size_t index(size_t id) const override;
   ScopeSeqMap* clone() const override;
   ///@}

private:
   size_t first_, last_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a general mapping.
///
/// This is implemented by an hash table.
///////////////////////////////////////////////////////////////////////////////
class ScopeHashMap : public AbstractScopeMap {
public:
   /// Creates an empty mapping
   ScopeHashMap();

   /// Copy constructor
   ScopeHashMap(const ScopeHashMap& other);

   /// Inserts an element in the domain of this
   /// @param id element inserted in this
   void insert(size_t id);

   ///@{
   size_t index(size_t id) const override;
   ScopeHashMap* clone() const override;
   ///@}

private:
   std::unordered_map<size_t,size_t> m_;  // hash map
   size_t next_;                          // index of the next element
};

} // namespace

#endif
