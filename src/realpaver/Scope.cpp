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
 * @file   Scope.cpp
 * @brief  Scope of variables
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

ScopeRep::ScopeRep()
    : m_()
    , scopeMap_(nullptr)
    , hcode_(0)
    , minid_(0)
    , maxid_(0)
{
}

ScopeRep::ScopeRep(const ScopeRep &other)
    : m_(other.m_)
    , scopeMap_(nullptr)
    , hcode_(other.hcode_)
    , minid_(other.minid_)
    , maxid_(other.maxid_)
{
   if (other.scopeMap_ != nullptr)
      scopeMap_ = other.scopeMap_->clone();
}

ScopeRep::~ScopeRep()
{
   if (scopeMap_ != nullptr)
      delete scopeMap_;
}

typename ScopeRep::const_iterator ScopeRep::find(const Variable &v)
{
   auto it = m_.find(v);
   return const_iterator(it);
}

void ScopeRep::makeMap()
{
   if ((minid_ == 0) && (maxid_ == size() - 1))
   {
      scopeMap_ = new ScopeIdMap(maxid_);
   }
   else if (maxid_ - minid_ == size() - 1)
   {
      scopeMap_ = new ScopeSeqMap(minid_, maxid_);
   }
   else
   {
      ScopeHashMap *aux = new ScopeHashMap();
      for (auto it : m_)
         aux->insert(it.first.id());

      scopeMap_ = aux;
   }

   hcode_ = scopeMap_->hashCode();
}

size_t ScopeRep::size() const
{
   return m_.size();
}

size_t ScopeRep::index(const Variable &v) const
{
   return scopeMap_->index(v.id());
}

Variable ScopeRep::var(size_t i) const
{
   auto it = m_.begin();
   std::advance(it, i);
   return it->first;
}

size_t ScopeRep::minVarId() const
{
   return minid_;
}

size_t ScopeRep::maxVarId() const
{
   return maxid_;
}

typename ScopeRep::const_iterator ScopeRep::begin() const
{
   return const_iterator(m_.cbegin());
}

typename ScopeRep::const_iterator ScopeRep::end() const
{
   return const_iterator(m_.cend());
}

void ScopeRep::insert(Variable v, size_t n)
{
   auto it = m_.find(v);

   if (it != m_.end())
   {
      if (it->first != v)
         THROW("Bad insertion in a scope: two different variables "
               << "have the same index : " << v.id());

      else
         it->second += n;
   }
   else
   {
      m_.insert(std::make_pair(v, n));
      size_t id = v.id();

      if (size() == 1)
      {
         minid_ = maxid_ = id;
      }
      else
      {
         if (id < minid_)
            minid_ = id;
         if (id > maxid_)
            maxid_ = id;
      }

      if (scopeMap_ != nullptr)
         delete scopeMap_;

      makeMap();
   }
}

void ScopeRep::remove(const Variable &v)
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
         minid_ = maxid_ = jt->first.id();
         for (++jt; jt != m_.cend(); ++jt)
         {
            size_t id = jt->first.id();
            if (minid_ > id)
               minid_ = id;
            if (maxid_ < id)
               maxid_ = id;
         }

         makeMap();
      }
   }
}

void ScopeRep::remove(const Variable &v, size_t n)
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

size_t ScopeRep::hashCode() const
{
   return hcode_;
}

void ScopeRep::print(std::ostream &os) const
{
   os << "{";
   bool first = true;
   for (auto it : m_)
   {
      if (first)
         first = false;
      else
         os << ", ";

      os << it.first.getName();
   }
   os << "}";
}

bool ScopeRep::isIdentity() const
{
   return scopeMap_->isIdentity();
}

/*----------------------------------------------------------------------------*/

Scope::Scope()
    : rep_(std::make_shared<ScopeRep>())
{
}

Scope::Scope(const std::initializer_list<Variable> &l)
    : rep_(std::make_shared<ScopeRep>())
{
   for (Variable v : l)
      insert(v);
}

Scope::Scope(std::shared_ptr<ScopeRep> rep)
    : rep_(rep)
{
}

void Scope::insert(Variable v)
{
   insert(v, 1);
}

size_t Scope::hashCode() const
{
   ASSERT(rep_ != nullptr, "Scope with null pointer");

   return rep_->hashCode();
}

void Scope::insert(Variable v, size_t n)
{
   if (isShared())
   {
      // copy-on-write (cow) pointer
      rep_ = std::make_shared<ScopeRep>(*rep_.get());
   }

   rep_->insert(v, n);
}

void Scope::remove(const Variable &v)
{
   remove(v, 1);
}

void Scope::remove(const Variable &v, size_t n)
{
   ASSERT(contains(v), "No variable " << v.getName() << " in this scope");

   if (isShared())
   {
      // copy-on-write (cow) pointer
      rep_ = std::make_shared<ScopeRep>(*rep_.get());
   }

   rep_->remove(v, n);
}

size_t Scope::size() const
{
   ASSERT(rep_ != nullptr, "Scope with null pointer");

   return rep_->size();
}

bool Scope::isEmpty() const
{
   return size() == 0;
}

bool Scope::contains(const Variable &v) const
{
   auto it = rep_->find(v);

   // no variable having the same index
   if (it == end())
      return false;

   // checks if the two variables correspond
   return (*it) == v;
}

size_t Scope::minVarId() const
{
   ASSERT(rep_ != nullptr, "Scope with null pointer");

   return rep_->minVarId();
}

size_t Scope::maxVarId() const
{
   ASSERT(rep_ != nullptr, "Scope with null pointer");

   return rep_->maxVarId();
}

void Scope::print(std::ostream &os) const
{
   ASSERT(rep_ != nullptr, "Scope with null pointer");

   rep_->print(os);
}

typename Scope::const_iterator Scope::begin() const
{
   ASSERT(rep_ != nullptr, "Scope with null pointer");

   return rep_->begin();
}

typename Scope::const_iterator Scope::end() const
{
   ASSERT(rep_ != nullptr, "Scope with null pointer");

   return rep_->end();
}

typename Scope::const_iterator Scope::find(const Variable &v) const
{
   ASSERT(rep_ != nullptr, "Scope with null pointer");

   auto it = rep_->find(v);

   // no variable having the same index
   if (it == rep_->end())
      return it;

   // one variable havibg the same index but different from v
   if (*it != v)
      return rep_->end();

   // variable found
   return it;
}

size_t Scope::index(const Variable &v) const
{
   ASSERT(rep_ != nullptr, "Scope with null pointer");
   ASSERT(contains(v), "No variable " << v.getName() << " in this scope");

   return rep_->index(v);
}

Variable Scope::var(size_t i) const
{
   ASSERT(rep_ != nullptr, "Scope with null pointer");
   ASSERT(i < size(), "Bad access to a variable in a scope @ " << i);

   return rep_->var(i);
}

void Scope::insert(const std::initializer_list<Variable> &l)
{
   for (Variable v : l)
      insert(v);
}

void Scope::insert(const Scope &other)
{
   for (Variable v : other)
      insert(v, other.count(v));
}

void Scope::remove(const Scope &other)
{
   for (Variable v : other)
      remove(v, other.count(v));
}

size_t Scope::count(const Variable &v) const
{
   auto it = find(v);
   return (it == end()) ? 0 : it.count();
}

Scope Scope::clone() const
{
   Scope s;
   for (auto it = begin(); it != end(); ++it)
      s.insert(it.var(), it.count());

   return s;
}

bool Scope::isShared() const
{
   return rep_ != nullptr && rep_.use_count() > 1;
}

std::ostream &operator<<(std::ostream &os, const Scope &s)
{
   s.print(os);
   return os;
}

bool Scope::contains(const Scope &other) const
{
   for (auto it = other.begin(); it != other.end(); ++it)
   {
      if (!contains(*it))
         return false;
   }
   return true;
}

bool Scope::disjoint(const Scope &other) const
{
   if (minVarId() > other.maxVarId())
      return true;
   if (maxVarId() < other.minVarId())
      return true;

   for (auto v : *this)
   {
      if (other.contains(v))
         return false;
   }
   return true;
}

bool Scope::overlaps(const Scope &other) const
{
   return !disjoint(other);
}

bool Scope::operator==(const Scope &other) const
{
   if (rep_.get() == other.rep_.get())
      return true;

   if (size() != other.size())
      return false;

   return contains(other);
}

bool Scope::operator!=(const Scope &other) const
{
   return !operator==(other);
}

Scope Scope::setminus(const Scope &scop) const
{
   Scope res;
   for (auto v : (*this))
      if (!scop.contains(v))
         res.insert(v);
   return res;
}

Scope operator&(const Scope &s, const Scope &t)
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

Scope operator|(const Scope &s, const Scope &t)
{
   Scope res;

   for (auto it = s.begin(); it != s.end(); ++it)
      res.insert(*it, it.count());

   for (auto it = t.begin(); it != t.end(); ++it)
      res.insert(*it, it.count());

   return res;
}

size_t Scope::nameMaxLength() const
{
   size_t l = 0;
   for (auto v : *this)
   {
      size_t n = v.getName().length();
      if (n > l)
         l = n;
   }
   return l;
}

bool Scope::isIdentity() const
{
   return rep_->isIdentity();
}

} // namespace realpaver
