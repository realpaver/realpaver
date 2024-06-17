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
 * @file   ScopeMap.hpp
 * @brief  Map between two sets of integers
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_SCOPE_MAP_HPP
#define REALPAVER_SCOPE_MAP_HPP

#include <unordered_map>

namespace realpaver {

/**
 * @brief Abstract base class of mappings between two sets of integers.
 *
 * Any ordered set {i0, i1, ..., ik} maps to the interval 0..k, i.e. given
 * a mapping M, we have M(ij) = j for each j, 0 <= j <= k.
 *
 * This is used by the Scope class in order to associate an ordered sequence
 * of integers with a set of variables.
 */
class AbstractScopeMap {
public:
   /// Destructor
   virtual ~AbstractScopeMap();

   /// Returns M(id)
   virtual size_t index(size_t id) const = 0;

   /// Returns a clone of this
   virtual AbstractScopeMap* clone() const = 0;

   /// Calculates an hash code
   virtual size_t hashCode() const = 0;

   /// Returns true if this is an identity map, i.e. every index maps to itself
   virtual bool isIdentity() const;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Identity mapping.
 *
 * The domain of the mapping {i0, i1, ..., ik} is equal to 0..k.
 */
class ScopeIdMap : public AbstractScopeMap {
public:
   /// Creates an identity mapping 0..last -> 0..last
   ScopeIdMap(size_t last);

   /// Copy constructor
   ScopeIdMap(const ScopeIdMap& other);

   size_t index(size_t id) const override;
   ScopeIdMap* clone() const override;
   size_t hashCode() const override;
   bool isIdentity() const override;

private:
   size_t last_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Mapping from a sequence of integers.
 * 
 * A sequence first, first+1, ..., last is mapped to 0, 1, ..., last-first.
 */
class ScopeSeqMap : public AbstractScopeMap {
public:
   /// Creates a mapping for the sequence first..last
   ScopeSeqMap(size_t first, size_t last);

   /// Copy constructor
   ScopeSeqMap(const ScopeSeqMap& other);

   size_t index(size_t id) const override;
   ScopeSeqMap* clone() const override;
   size_t hashCode() const override;

private:
   size_t first_, last_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief General mapping.
 * 
 * A set of indexes i(k), i(k+1), ... is mapped to 0, 1, ...
 */
class ScopeHashMap : public AbstractScopeMap {
public:
   /// Creates an empty mapping
   ScopeHashMap();

   /// Copy constructor
   ScopeHashMap(const ScopeHashMap& other);

   /// Inserts id in the domain of this
   void insert(size_t id);

   size_t index(size_t id) const override;
   ScopeHashMap* clone() const override;
   size_t hashCode() const override;

private:
   std::unordered_map<size_t,size_t> m_;  // hash map
   size_t next_;                          // index of the next element
};

} // namespace

#endif
