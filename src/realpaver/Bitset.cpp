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
 * @file   Bitset.cpp
 * @brief  The Bitset class
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include <algorithm>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Bitset.hpp"
#include "realpaver/Common.hpp"

namespace realpaver {

Bitset::Bitset()
      : size_(0),
        first_(0),
        last_(-1),
        wcount_(0),
        word_(nullptr),
        shadow_word_(0)
{}

Bitset::Bitset(size_t n) : Bitset(0, n-1)
{
   ASSERT(n > 0, "Creation of bitset with null size... " <<
                 "the default constructor must be used");
}

Bitset::Bitset(int first, int last)
{
   ASSERT(first <= last, "Creation of bitset with bad indexes");

   size_ = 1 + (size_t)(last - first);
   first_ = first;
   last_ = last;
   wcount_ = 1 + (size_ - 1) / BITS_PER_WORD;
   word_ = new word_t[wcount_];

   // shadow word : 00...011...1 with k zeros
   static word_t nz = ~0;
   size_t k = wcount_ * BITS_PER_WORD - size_;
   shadow_word_ = (k == 0) ? nz : (nz >> k);

   setAllZero();
}

Bitset::~Bitset()
{
   if (word_ != nullptr)
      delete[] word_;
}

Bitset::Bitset(const Bitset& other):
   size_(other.size_),
   first_(other.first_),
   last_(other.last_),
   wcount_(other.wcount_),
   word_(nullptr),
   shadow_word_(other.shadow_word_)
{
   if (other.size() > 0)
   {
      word_ = new word_t[wcount_];
      std::copy_n(other.word_, wcount_, word_);
   }
}

Bitset::Bitset(Bitset&& other) :
   size_(other.size_),
   first_(other.first_),
   last_(other.last_),
   wcount_(other.wcount_),
   word_(other.word_),
   shadow_word_(other.shadow_word_)
{
   other.size_ = other.wcount_ = 0;
   other.word_ = nullptr;
}

Bitset::Bitset(const std::initializer_list<int>& l) : Bitset(l.size())
{
   size_t j = 0;

   for (auto it=l.begin(); it != l.end(); ++it, ++j)
      if (*it != 0)
         setOne(j);
}

Bitset& Bitset::operator=(const Bitset& other)
{
   if (word_ != nullptr)
      delete[] word_;

   size_ = other.size_;
   first_ = other.first_;
   last_ = other.last_;
   wcount_ = other.wcount_;
   shadow_word_ = other.shadow_word_;

   if (size_ > 0)
   {
      word_ = new word_t[wcount_];
      std::copy_n(other.word_, wcount_, word_);
   }

   return *this;
}

Bitset& Bitset::operator=(Bitset&& other)
{
   if (word_ != nullptr)
      delete[] word_;

   size_ = other.size_;
   first_ = other.first_;
   last_ = other.last_;
   wcount_ = other.wcount_;
   word_ = other.word_;
   shadow_word_ = other.shadow_word_;

   other.size_ = other.wcount_ = 0;
   other.word_ = nullptr;

   return *this;
}

size_t Bitset::size() const 
{
   return size_;
}

int Bitset::first() const
{
   return first_;
}

int Bitset::last() const
{
   return last_;
}

size_t Bitset::wordCount() const
{
   return wcount_;
}

void Bitset::keepShadowBits()
{
   if (wcount_ > 0)
      word_[wcount_-1] &= shadow_word_;
}

size_t Bitset::get(int i) const
{
   if (i < first_ || i > last_)
      return 0;

   else
   {
      int j = i - first_;
      return word_[bitIndex(j)] & (word_t(1) << bitOffset(j));
   }
}

Bitset& Bitset::setZero(int i)
{
   int j = i - first_;
   word_[bitIndex(j)] &= ~(word_t(1) << bitOffset(j));

   return *this;
}

Bitset& Bitset::setOne(int i)
{
   int j = i - first_;
   word_[bitIndex(j)] |= (word_t(1) << bitOffset(j));

   return *this;
}

Bitset& Bitset::setAllZero()
{
   for (size_t i=0; i<wcount_; ++i)
      word_[i] = 0;

   return *this;
}

Bitset& Bitset::setAllOne()
{
   static word_t nz = ~0;

   for (size_t i=0; i<wcount_; ++i)
      word_[i] = nz;

   keepShadowBits();

   return *this;
}

void Bitset::flip(int i)
{
   if (get(i)) setZero(i);
   else setOne(i);
}

void Bitset::flipAll()
{
   for (size_t i=0; i<wcount_; ++i)
      word_[i] = ~word_[i];

   keepShadowBits();
}

size_t Bitset::nbZeros() const
{
   size_t nb = 0;

   for (size_t i=0; i<size_; ++i)
      if (!get(i)) ++nb;

   return nb;
}

size_t Bitset::nbOnes() const
{
   size_t nb = 0;

   for (size_t i=0; i<size_; ++i)
      if (get(i)) ++nb;

   return nb;
}

bool Bitset::areAllOnes() const
{
   size_t wlast = wcount_ - 1, w;

   for (size_t i=0; i<wlast; ++i)
   {
      w = ~word_[i];
      if (w != 0) return false;
   }

   // last word
   w = (~word_[wlast]) & shadow_word_;
   if (w != 0) return false;

   return true;
}

bool Bitset::areAllZeros() const
{
   for (size_t i=0; i<wcount_; ++i)
      if (word_[i]!=0) return false;

   return true;
}

void Bitset::print(std::ostream& os) const
{
   if (size_ == 0)
      os << "empty bitset";

   else
   {
      for (int i=first_; i<=last_; ++i)
         os << (get(i) ? 1 : 0);

      os << " (first: " << first_ << ")";
   }
}

size_t Bitset::hashCode() const
{
   size_t h = 0;

   if (size() > 0)
   {
      h = hash1<word_t>(word_[0]);

      for (size_t i=1; i<wcount_; ++i)
         h = hash2(h, hash1<word_t>(word_[i]));
   }

   return h;
}

bool Bitset::overlaps(const Bitset& other) const
{
   if (size_ == 0 || other.size_ == 0)
      return false;

   if (first_ == other.first_ && last_ == other.last_)
   {
      for (size_t i=0; i<wcount_; ++i)      
         if (word_[i] & other.word_[i])
            return true;

      return false;
   }
   else
   {
      // interval of indexes of common bits
      int p = std::max(first_, other.first_),
          q = std::min(last_, other.last_);

      for (int i=p; i<=q; ++i)
         if (get(i) && other.get(i))
            return true;

      return false;
   }
}

Bitset& Bitset::operator&=(const Bitset& other)
{
   if (size_ == 0 || other.size_ == 0)
   {
      Bitset aux;
      return *this = aux;
   }

   if (first_ == other.first_ && last_ == other.last_)
   {
      for (size_t i=0; i<wcount_; ++i)      
         word_[i] &= other.word_[i];

      return *this;
   }

   // interval of indexes of common bits
   int p = std::max(first_, other.first_),
       q = std::min(last_, other.last_);

   if (p > q)
   {
      Bitset aux;
      return *this = aux;
   }

   Bitset aux(p, q);
   aux.setAllZero();

   for (int i=p; i<=q; ++i)
      if (get(i) && other.get(i))
         aux.setOne(i);

   return *this = aux;
}

Bitset& Bitset::operator|=(const Bitset& other)
{
   if (other.size_ == 0) return *this;
   if (size_ == 0) return *this = other;

   if (first_ == other.first_ && last_ == other.last_)
   {
      for (size_t i=0; i<wcount_; ++i)      
         word_[i] |= other.word_[i];

      return *this;
   }

   int p = std::min(first_, other.first_),
       q = std::max(last_, other.last_);

   Bitset aux(p, q);
   aux.setAllZero();

   for (int i=p; i<=q; ++i)
   {
      bool b = false;
      if (i>=first_ && i<=last_ && get(i)) b = true;
      if (i>=other.first_ && i<=other.last_ && other.get(i)) b = true;
      if (b)
         aux.setOne(i);
   }

   return *this = aux;
}

Bitset operator&(const Bitset& b1, const Bitset& b2)
{
   ASSERT(b1.size() > 0 || b2.size() > 0,
          "bitwise AND with two empty bitsets");

   Bitset res(b1);
   res &= b2;
   return res;
}

Bitset operator|(const Bitset& b1, const Bitset& b2)
{
   ASSERT(b1.size() > 0 || b2.size() > 0,
          "bitwise OR with two empty bitsets");

   Bitset res(b1);
   res |= b2;
   return res;
}

Bitset operator~(const Bitset& b)
{
   ASSERT(b.size() > 0, "bitwise NOT over an empty bitset");

   Bitset res(b);
   res.flipAll();
   return res;
}

std::ostream& operator<<(std::ostream& os, const Bitset& b)
{
   b.print(os);
   return os;
}

} // namespace
