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
 * @file   Scope.hpp
 * @brief  Scope of variables
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_SCOPE_HPP
#define REALPAVER_SCOPE_HPP

#include <map>
#include <memory>
#include "realpaver/ScopeMap.hpp"
#include "realpaver/Variable.hpp"

namespace realpaver {

/**
 * @brief Representations of a scope, i.e. an ordered set of variables.
 *
 * Given a set of variables {xi1, ..., xik} ordered by their indexes
 * i1, ..., ik, a scope mapping maps every xij to its index j in the scope.
 */
class ScopeRep {
public:
   /// Creates an empty scope representation
   ScopeRep();

   /// Copy constructor
   ScopeRep(const ScopeRep& other);

   /// No assignment
   ScopeRep& operator=(const ScopeRep& other) = delete;

   /// Destructor
   ~ScopeRep();

   /// Returns the number of variables
   size_t size() const;

   /// Gets the index of v
   size_t index(const Variable& v) const;

   /// Returns the i-th variable in this
   Variable var(size_t i) const;

   /// Inserts n occurrences of v
   void insert(Variable v, size_t n);

   /// Removes v from this
   void remove(const Variable& v);

   /// Removes n occurrences ofv from this
   void remove(const Variable& v, size_t n);

   /// Output on a stream
   void print(std::ostream& os) const;

   /// Returns the smallest variable identifier in this
   size_t minVarId() const;

   /// Returns the greatest variable identifier in this
   size_t maxVarId() const;

   /// Returns the hash code of this
   size_t hashCode() const;

private:
   struct Comp {
      bool operator()(Variable x, Variable y) const
      {
         return x.id() < y.id();
      }
   };

   using MapType = std::map<Variable, size_t, Comp>;
   using MapIteratorType = MapType::const_iterator;

   MapType m_;

   AbstractScopeMap* scopeMap_;   // map v.id() -> 0..size()-1
   size_t hcode_;                 // hash code
   size_t minid_;                 // min {v.id() : v in this}
   size_t maxid_;                 // max {v.id() : v in this}

   // recreates the map of indexes
   void makeMap();

public:
   /// Class of constant iterators
   class const_iterator {
   public:
      /// Default constructor
      const_iterator() : it_() {}

      /// Constructor
      const_iterator(MapIteratorType it) : it_(it) {}

      /// Dereference operator
      Variable operator*() { return it_->first; }

      /// Increment operator prefix
      const_iterator& operator++() { ++it_; return *this; }

      /// Increment operator postfix
      const_iterator operator++(int)
      {
         const_iterator res = *this;
         ++(*this);
         return res;
      }

      /// Equality test
      bool operator==(const const_iterator& other) const
      {
         return it_ == other.it_;
      }

      /// Disequality test
      bool operator!=(const const_iterator& other) const
      {
         return it_ != other.it_;
      }

      /// @return the variable pointed by this iterator
      Variable var() { return it_->first; }

      /// Returns the occurrence counter of var()
      size_t count() { return it_->second; }

      /// Returns the map iterator enclosed
      MapIteratorType get() { return it_; }

   private:
      MapIteratorType it_;    // map iterator
   };

   /// Returns an iterator to the beginning of this
   const_iterator begin() const;

   /// Returns an iterator to the end of this
   const_iterator end() const;

   /// Finds a variable in this
   const_iterator find(const Variable& v);
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Ordered set of variables.
 * 
 * The variables are indexed from 0 to size()-1.
 *
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 *
 * The behaviour of the shared pointer follows a copy-on-write (cow) pointer.
 * If the scope is modified by insert or remove and if its representation is
 * shared by several pointers, then its pointer is first detached and a new
 * representation is created. There is no consequence for the other pointers.
 */
class Scope {
public:
   /// Creates an empty scope
   Scope();

   /// Creates a scope from a list of variables
   Scope(const std::initializer_list<Variable>& l);

   /// Default copy constructor
   Scope(const Scope& other) = default;

   /// Default assignment operator
   Scope& operator=(const Scope&) = default;

   /// Default destructor
   ~Scope() = default;

   /// Returns the number of variables
   size_t size() const;

   /// Returns the hash code of this
   size_t hashCode() const;

   /// Returns true if this is empty
   bool isEmpty() const;

   /// Gets the index of v in this
   size_t index(const Variable& v) const;

   /// Returns the i-th variable in this
   Variable var(size_t i) const;

   /// Returns the number of occurrences of v in this
   size_t count(const Variable& v) const;

   /// Membership test
   bool contains(const Variable& v) const;

   /// Returns the smallest variable identifier in this
   size_t minVarId() const;

   /// Returns the greatest variable identifier in this
   size_t maxVarId() const;

   /// Inserts a variable in this
   void insert(Variable v);

   /// Inserts n occurrences of v
   void insert(Variable v, size_t n);

   /// Inserts a list of variables in this
   void insert(const std::initializer_list<Variable>& l);

   /// Inserts a scope in this
   void insert(const Scope& other);

   /// Removes a variable from this
   void remove(const Variable& v);

   /// Removes n  occurrences of v
   void remove(const Variable& v, size_t n);

   /// Removes a scope from this
   void remove(const Scope& other);

   /// Output on a stream
   void print(std::ostream& os) const;

   /// Inclusion test
   bool contains(const Scope& other) const;

   /// Disjunction test
   bool disjoint(const Scope& other) const;

   /// Intersection test
   bool overlaps(const Scope& other) const;

   /// Equality test
   bool operator==(const Scope& other) const;

   /// Disequality test
   bool operator!=(const Scope& other) const;

   /// Type of constant iterators
   using const_iterator = ScopeRep::const_iterator;

   /// Returns an iterator to the beginning of this
   const_iterator begin() const;

   /// Returns an iterator to the end of this
   const_iterator end() const;

   /// Finds a variable in this
   const_iterator find(const Variable& v) const;

   /**
    * @brief Returns a clone of this.
    *
    * A new representation is created, i.e. the new scope shares
    * no representation with this.
    */
   Scope clone() const;

   /// Returns true if this scope is shared, false otherwise
   bool isShared() const;

   /// Returns the length of the longest variable name of this
   size_t nameMaxLength() const;

private:
   std::shared_ptr<ScopeRep> rep_;  // shared representation

   // constructor
   Scope(std::shared_ptr<ScopeRep> rep);
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Scope& s);

/// Returns a new scope corresponding to the intersection of s and t
Scope operator&(Scope s, Scope t);

/// Returns a new scope corresponding to the union of s and t
Scope operator|(Scope s, Scope t);

} // namespace

#endif
