///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_SCOPE_HPP
#define REALPAVER_SCOPE_HPP

#include <map>
#include <memory>
#include "realpaver/ScopeMap.hpp"
#include "realpaver/Variable.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a representations of a scope, i.e. an ordered set of variables.
///
/// Given a set of variables {xi1, ..., xik} ordered by their indexes
/// i1, ..., ik, a scope mapping maps every xij to its index j in the scope.
///////////////////////////////////////////////////////////////////////////////
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

   /// @return the number of variables
   size_t size() const;

   /// Gets the index of a variable in this
   /// @param v a variable
   /// @return index of v
   size_t index(const Variable& v) const;

   /// Access to a variable
   /// @param i an index between 0 and size()-1
   /// @return the i-th variable in this
   Variable var(size_t i) const;

   /// Inserts a variable with a given number of occurrences in this
   /// @param v a variable
   /// @param n number of occurrences of v
   void insert(Variable v, size_t n);

   /// Removes a variable from this
   /// @param v variable removed
   void remove(const Variable& v);

   /// Removes a number of occurrences of a variable from this
   /// @param v variable removed
   /// @param n number of occurrences of v
   void remove(const Variable& v, size_t n);

   /// Output on a stream
   /// @param os output stream
   void print(std::ostream& os) const;

   /// @return the smallest variable identifier in this
   size_t minVarId() const;

   /// @return the greatest variable identifier in this
   size_t maxVarId() const;

private:
   struct Comp {
      bool operator()(Variable x, Variable y) const
      {
         return x.id() < y.id();
      }
   };

   typedef typename std::map<Variable, size_t, Comp> MapType;
   typedef typename MapType::const_iterator MapIteratorType;
   MapType m_;

   AbstractScopeMap* scopeMap_;   // map v.id() -> 0..size()-1
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

      /// @return the occurrence counter of var()
      size_t count() { return it_->second; }

      /// @return the map iterator enclosed
      MapIteratorType get() { return it_; }

   private:
      MapIteratorType it_;    // map iterator
   };

   /// @return an iterator to the beginning of this
   const_iterator begin() const;

   /// @return an iterator to the end of this
   const_iterator end() const;

   /// Finds a variable in this
   /// @param v a variable
   /// @return iterator pointing to v
   const_iterator find(const Variable& v);
};

///////////////////////////////////////////////////////////////////////////////
/// This is an ordered set of variables.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///
/// The behaviour of the shared pointer follows a copy-on-write (cow) pointer.
/// If the scope is modified by insert or remove and if its representation is
/// shared by several pointers, then its pointer is first detached and a new
/// representation is created. There is no consequence for the other pointers.
///////////////////////////////////////////////////////////////////////////////
class Scope {
public:
   /// Creates an empty scope
   Scope();

   /// Creates a scope from a list
   /// @param l list of variables
   Scope(const std::initializer_list<Variable>& l);

   /// Default copy constructor
   Scope(const Scope& other) = default;

   /// Default assignment operator
   Scope& operator=(const Scope&) = default;

   /// Default destructor
   ~Scope() = default;

   /// @return the number of variables
   size_t size() const;

   /// @return true if this is empty
   bool isEmpty() const;

   /// Gets the index of a variable in this
   /// @param v a variable
   /// @return index of 'v' in 0..size()-1
   size_t index(const Variable& v) const;

   /// Access to a variable
   /// @param i an index between 0 and size()-1
   /// @return the i-th variable in this
   Variable var(size_t i) const;

   /// Gets the occurrence counter of a variable
   /// @param v a variable
   /// @return number of occurrences of 'v' in this
   size_t count(const Variable& v) const;

   /// Membership test
    /// @param v a variable
   /// @return true if 'v' belongs to this
   bool contains(const Variable& v) const;

   /// @return the smallest variable identifier in this
   size_t minVarId() const;

   /// @return the greatest variable identifier in this
   size_t maxVarId() const;

   /// Inserts a variable in this
   /// @param v a variable
   void insert(Variable v);

   /// Inserts a variable with a given number of occurrences in this
   /// @param v a variable
   /// @param n number of occurrences of v
   void insert(Variable v, size_t n);

   /// Inserts a list of variables in this
   /// @param l list of variables
   void insert(const std::initializer_list<Variable>& l);

   /// Inserts a scope in this
   /// @param other scope inserted
   void insert(const Scope& other);

   /// Removes a variable from this
   /// @param v variable removed
   void remove(const Variable& v);

   /// Removes a number of occurrences of a variable from this
   /// @param v variable removed
   /// @param n number of occurrences of v
   void remove(const Variable& v, size_t n);

   /// Removes a scope from this
   /// @param other scope removed
   ///
   ///For every couple (v,n) from other, calls remove(v,n)
   void remove(const Scope& other);

   /// Output on a stream
   /// @param os output stream
   void print(std::ostream& os) const;

   /// Inclusion test
   /// @param other a scope
   /// @return true if this contains the 'other' scope
   bool contains(const Scope& other) const;

   /// Disjunction test
   /// @param other a scope
   /// @return true if this and the 'other' scope are disjoint
   bool disjoint(const Scope& other) const;

   /// Intersection test
    /// @param other a scope
   /// @return true if this and the 'other' scope overlap
   bool overlaps(const Scope& other) const;

   /// Equality test
   /// @param other a scope
   /// @return true if this and the 'other' scope are equal
   bool operator==(const Scope& other) const;

   /// Disequality test
   /// @param other a scope
   /// @return true if this and the 'other' scope are different
   bool operator!=(const Scope& other) const;

   /// Type of constant iterators
   typedef typename ScopeRep::const_iterator const_iterator;

   /// @return an iterator to the beginning of this
   const_iterator begin() const;

   /// @return an iterator to the end of this
   const_iterator end() const;

   /// Finds a variable in this
   /// @param v a variable
   /// @return iterator pointing to v
   const_iterator find(const Variable& v) const;

   /// @return a clone of this
   ///
   /// A new representation is created, i.e. the new scope shares
   /// no representation with this.
   Scope clone() const;

   /// @return true if this scope is shared, false otherwise
   bool isShared() const;

   /// @return the length of the longest variable name of this
   size_t nameMaxLength() const;

private:
   std::shared_ptr<ScopeRep> rep_;

   Scope(std::shared_ptr<ScopeRep> rep);
};

/// Output on a stream
/// @param os an output stream
/// @param s a scope
/// @return the stream given as input
std::ostream& operator<<(std::ostream& os, const Scope& s);

/// Intersection of scopes
/// @param s a scope
/// @param t a scope
/// @return the intersection of s and t
Scope operator&(Scope s, Scope t);

/// Union of scopes
/// @param s a scope
/// @param t a scope
/// @return the union of s and t
Scope operator|(Scope s, Scope t);

} // namespace

#endif
