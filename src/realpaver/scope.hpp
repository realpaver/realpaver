// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_SCOPE_HPP
#define REALPAVER_SCOPE_HPP

#include <map>
#include <memory>
#include "realpaver/Bitset.hpp"
#include "realpaver/scope_map.hpp"
#include "realpaver/Variable.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of representations of scopes, i.e. sets of variables.
 *****************************************************************************/
class ScopeRep {
public:
   // constructors
   ScopeRep();
   ScopeRep(const ScopeRep& other);

   ScopeRep& operator=(const ScopeRep& other) = delete;

   // destructor
   ~ScopeRep();

   // number of variables
   size_t size() const;

   // returns the index of 'v'
   size_t index(const Variable& v) const;

   // inserts 'v' with 'n' occurrences
   void insert(const Variable& v, size_t n);

   // removes 'v' from the scope
   void remove(const Variable& v);

   // removes 'n' occurrences of 'v' from the scope
   void remove(const Variable& v, size_t n);

   // assigns the counter of occurrences of 'v' to 1
   void resetCount(const Variable& v);

   // resets all the counters
   void resetCountAll();

   // output on a stream
   void print(std::ostream& os) const;

   // minimum variable id
   size_t minIndex() const;

   // maximum variable id
   size_t maxIndex() const;

private:
   struct comp {
      bool operator()(Variable x, Variable y)
      {
         return x.getId() < y.getId();
      }
   };

   typedef typename std::map< Variable, size_t, comp > MapType;
   typedef typename MapType::const_iterator MapIteratorType;
   MapType m_;

   AbstractScopeMap* scopeMap_;   // map v.id() -> 0..size()-1
   size_t minid_;                 // min {v.id() : v in this}
   size_t maxid_;                 // max {v.id() : v in this}

   // recreates the map of indexes
   void makeMap();

public:
   // constant iterators
   class const_iterator {
   public:
      // default constructor
      const_iterator() : it_() {}

      // constructor
      const_iterator(MapIteratorType it) : it_(it) {}

      // dereference operator
      Variable operator*() { return it_->first; }

      // increment operators
      const_iterator& operator++() { ++it_; return *this; }

      const_iterator operator++(int)
      {
         const_iterator res = *this;
         ++(*this);
         return res;
      }

      // comparison
      bool operator==(const const_iterator& other) const
      {
         return it_ == other.it_;
      }

      bool operator!=(const const_iterator& other) const
      {
         return it_ != other.it_;
      }

      // get the variable pointed by this iterator
      Variable var() { return it_->first; }

      // get the occurrence counter of var()
      size_t count() { return it_->second; }

      // get the map iterator enclosed
      MapIteratorType get() { return it_; }

   private:
      MapIteratorType it_;    // map iterator
   };

   // returns an iterator to the beginning
   const_iterator begin() const;

   // returns an iterator to the end
   const_iterator end() const;

   // finds 'v' in this scope
   const_iterator find(const Variable& v);
};

inline size_t ScopeRep::size() const
{
   return m_.size();
}

inline size_t ScopeRep::index(const Variable& v) const
{
   return scopeMap_->index(v.getId());
}

inline size_t ScopeRep::minIndex() const
{
   return minid_;
}

inline size_t ScopeRep::maxIndex() const
{
   return maxid_;
}

inline typename ScopeRep::const_iterator
ScopeRep::begin() const
{
   return const_iterator(m_.cbegin());
}

inline typename ScopeRep::const_iterator
ScopeRep::end() const
{
   return const_iterator(m_.cend());   
}

/*****************************************************************************
 * Class of scopes, i.e.sets of variables.
 * 
 * A scope is implemented as a shared pointer to its representation.
 *****************************************************************************/
class Scope {
public:
   // constructors
   Scope();
   Scope(const std::initializer_list<Variable>& l);
   Scope(const Scope& other) = default;

   Scope& operator=(const Scope&) = default;
   ~Scope() = default;

   // number of variables
   size_t size() const;

   // returns true if this scope is empty
   bool isEmpty() const;

   // returns the index of 'v' in 0..size()-1
   size_t index(const Variable& v) const;

   // returns the occurrence counter of 'v'
   size_t count(const Variable& v) const;

   // returns true 'v' belongs to this scope
   bool contains(const Variable& v) const;

   // returns the minimum variable id
   size_t minIndex() const;

   // returns the maximum variable id
   size_t maxIndex() const;

   // returns a bitset corresponding to this scope
   Bitset toBitset() const;

   // insertion of variables
   void insert(const Variable& v);
   void insert(const Variable& v, size_t n);
   void insert(const std::initializer_list<Variable>& l);
   void insert(const Scope& other);

   // removes v from the scope
   void remove(const Variable& v);

   // removes n occurrences of v from the scope
   void remove(const Variable& v, size_t n);

   // for every couple (v,n) from other, calls remove(v,n)
   void remove(const Scope& other);

   // reset occurrence counters
   void resetCount(const Variable& v);
   void resetCountAll();

   // output on a stream
   void print(std::ostream& os) const;

   // returns true if this scope contains the 'other' scope
   bool contains(const Scope& other) const;

   // returns true if this scope and the 'other' scope are disjoint
   bool disjoint(const Scope& other) const;

   // returns true if this scope and the 'other' scope overlap
   bool overlaps(const Scope& other) const;

   // constant iterators
   typedef typename ScopeRep::const_iterator const_iterator;
   const_iterator begin() const;
   const_iterator end() const;

   // finds 'v' in this scope
   const_iterator find(const Variable& v) const;

   // returns a clone
   Scope clone() const;

private:
   // shared pointer to the scope representation
   std::shared_ptr<ScopeRep> rep_;

   // private constructor
   Scope(std::shared_ptr<ScopeRep> rep);
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const Scope& s);

// intersection of scopes
Scope operator&(Scope s, Scope t);

// union of scopes
Scope operator|(Scope s, Scope t);

inline void Scope::insert(const Variable& v)
{
   rep_->insert(v,1);
}

inline void Scope::insert(const Variable& v, size_t n)
{
   rep_->insert(v,n);
}

inline void Scope::remove(const Variable& v)
{
   rep_->remove(v);
}

inline void Scope::remove(const Variable& v, size_t n)
{
   rep_->remove(v,n);
}

inline size_t Scope::size() const
{
   return rep_->size();
}

inline bool Scope::isEmpty() const
{
   return size() == 0;
}

inline bool Scope::contains(const Variable& v) const
{
   return find(v) != end();
}

inline size_t Scope::minIndex() const
{
   return rep_->minIndex();
}

inline size_t Scope::maxIndex() const
{
   return rep_->maxIndex();
}

inline void Scope::resetCountAll()
{
   rep_->resetCountAll();
}

inline void Scope::print(std::ostream& os) const
{
   rep_->print(os);
}

inline typename Scope::const_iterator Scope::begin() const
{
   return rep_->begin();
}

inline typename Scope::const_iterator Scope::end() const
{
   return rep_->end();   
}

inline typename Scope::const_iterator Scope::find(const Variable& v) const
{
   return rep_->find(v);
}

inline size_t Scope::index(const Variable& v) const
{
   return rep_->index(v);
}

} // namespace

#endif
