// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/scope.hpp"

namespace realpaver {

ScopeRep::ScopeRep() : m_(), scopeMap_(nullptr), minid_(0), maxid_(0)
{}

ScopeRep::ScopeRep(const ScopeRep& other) :
   m_(other.m_),
   scopeMap_(nullptr),
   minid_(other.minid_),
   maxid_(other.maxid_)
{
   if (other.scopeMap_ != nullptr)
      scopeMap_ = other.scopeMap_->clone();
}

ScopeRep::~ScopeRep()
{
   if (scopeMap_ != nullptr)
      delete scopeMap_;
}

typename ScopeRep::const_iterator
ScopeRep::find(const Variable& v)
{
   auto it = m_.find(v);
   return const_iterator(it);
}

void ScopeRep::makeMap()
{
   if ((minid_ == 0) && (maxid_ == size()-1))
   {
      scopeMap_ = new ScopeIdMap(maxid_);
   }
   else if (maxid_ - minid_ == size() - 1)
   {
      scopeMap_ = new ScopeSeqMap(minid_,maxid_);
   }
   else
   {
      ScopeHashMap* aux = new ScopeHashMap();
      for (auto it : m_) aux->insert(it.first.getId());
      scopeMap_ = aux;
   }
}

void ScopeRep::insert(const Variable& v, size_t n)
{
   auto it = m_.find(v);

   if (it != m_.end())
      it->second += n;

   else
   {
      m_.insert(std::make_pair(v,n));
      size_t id = v.getId();

      if (size() == 1)
      {
         minid_ = maxid_ = id;
      }
      else
      {
         if (id < minid_) minid_ = id;
         if (id > maxid_) maxid_ = id;
      }

      if (scopeMap_ != nullptr)
         delete scopeMap_;

      makeMap();
   }
}

void ScopeRep::remove(const Variable& v)
{
   auto it = m_.find(v);
   if (it != m_.cend())
   {
      m_.erase(it);
      delete scopeMap_;

      if (size() == 0)
      {
         scopeMap_ = nullptr;
      }
      else
      {
         auto jt = m_.cbegin();
         minid_ = maxid_ = jt->first.getId();
         for (++jt; jt!=m_.cend(); ++jt)
         {
            size_t id = jt->first.getId();
            if (minid_ > id) minid_ = id;
            if (maxid_ < id) maxid_ = id;
         }

         makeMap();
      }
   }
}

void ScopeRep::remove(const Variable& v, size_t n)
{
   auto it = m_.find(v);
   if (it != m_.end())
   {
      if (it->second > n)
         it->second -= n;

      else
         remove(v);
   }
}

void ScopeRep::resetCount(const Variable& v)
{
   auto it = m_.find(v);
   if (it != m_.cend())
      m_[v] = 1;
}

void ScopeRep::resetCountAll()
{
   for (auto it = m_.cbegin(); it != m_.cend(); ++it)
      resetCount(it->first);
}

void ScopeRep::print(std::ostream& os) const
{
   os << "{";
   bool first = true;
   for (auto it : m_)
   {
      if (first) first = false;
      else os << ", ";

      os << it.first.getName() << "|" << it.second;
   }
   os << "}";
}

///////////////////////////////////////////////////////////////////////////////

Scope::Scope() : rep_(std::make_shared< ScopeRep >())
{}

Scope::Scope(const std::initializer_list<Variable>& l) :
   rep_(std::make_shared<ScopeRep>())
{
   for (Variable v : l)
      insert(v);
}

Scope::Scope(std::shared_ptr<ScopeRep> rep) : rep_(rep)
{}

void Scope::insert(const std::initializer_list<Variable>& l)
{
   for (Variable v : l)
      insert(v);   
}

void Scope::insert(const Scope& other)
{
   for (Variable v : other)
      insert(v,other.count(v));
}

void Scope::remove(const Scope& other)
{
   for (Variable v : other)
      remove(v,other.count(v));
}

size_t Scope::count(const Variable& v) const
{
   auto it = find(v);
   return (it == end()) ? 0 : it.count();
}

Bitset Scope::toBitset() const
{
   Bitset b(1+maxIndex(), 0);

   for (auto it = begin(); it != end(); ++it)
      b.setOne(it.var().getId());

   return b;
}

void Scope::resetCount(const Variable& v)
{
   ASSERT(contains(v),
          "no variable '" << v.getName() << "' belongs to this scope");

   rep_->resetCount(v);
}

Scope Scope::clone() const
{
   Scope s;
   for (auto it = begin(); it != end(); ++it)
      s.insert(it.var(),it.count());

   return s;
}

std::ostream& operator<<(std::ostream& os, const Scope& s)
{
   s.print(os);
   return os;
}

bool Scope::contains(const Scope& other) const
{
   for (auto it = other.begin(); it != other.end(); ++it)
   {
      if (!contains(*it))
         return false;
   }
   return true;
}

bool Scope::disjoint(const Scope& other) const
{
   for (auto v : *this)
   {
      if (other.contains(v))
         return false;
   }
   return true;
}

bool Scope::overlaps(const Scope& other) const
{
   return !disjoint(other);
}

Scope operator&(Scope s, Scope t)
{
   Scope res;

   for (auto v : s)
   {
      auto jt = t.find(v);
      if (jt != t.end())
         res.insert(v);         
   }

   return res;
}

Scope operator|(Scope s, Scope t)
{
   Scope res;

   for (auto it = s.begin(); it != s.end(); ++it)
      res.insert(*it,it.count());

   for (auto it = t.begin(); it != t.end(); ++it)
      res.insert(*it,it.count());

   return res;
}

} // namespace
