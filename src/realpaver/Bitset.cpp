///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Bitset.hpp"
#include "realpaver/Common.hpp"

namespace realpaver {

Bitset::Bitset()
      : size_(0),
        wcount_(0),
        word_(nullptr),
        shadow_word_(0)
{}

Bitset::Bitset(size_t n, size_t val)
      : size_(n),
        wcount_(1 + (n - 1) / BITS_PER_WORD),
        word_(nullptr),
        shadow_word_(0)
{
   ASSERT(n > 0, "creation of bitset with null size... " <<
                 "the default constructor must be used");

   word_ = new word_t[wcount_];

   // shadow word : 00...011...1 with k zeros
   static word_t nz = ~0;
   size_t k = wcount_ * BITS_PER_WORD - n;
   shadow_word_ = (k == 0) ? nz : (nz >> k);

   if (val) setAllOne();
   else setAllZero();
}

Bitset::Bitset(const std::initializer_list<int>& l) : Bitset(l.size(), 0)
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
   wcount_ = other.wcount_;
   word_ = other.word_;
   shadow_word_ = other.shadow_word_;

   other.size_ = other.wcount_ = 0;
   other.word_ = nullptr;

   return *this;
}

Bitset::~Bitset()
{
   if (word_ != nullptr)
      delete[] word_;
}

Bitset::Bitset(const Bitset& other):
   size_(other.size_),
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
   wcount_(other.wcount_),
   word_(other.word_),
   shadow_word_(other.shadow_word_)
{
   other.size_ = other.wcount_ = 0;
   other.word_ = nullptr;
}

size_t Bitset::size() const 
{
   return size_;
}

size_t Bitset::get(size_t i) const
{
   return i >= size() ?
             0 : word_[bitIndex(i)] & (word_t(1) << bitOffset(i));
}

void Bitset::keepShadowBits()
{
   if (wcount_ > 0)
      word_[wcount_-1] &= shadow_word_;
}

void Bitset::setZero(size_t i)
{
   word_[bitIndex(i)] &= ~(word_t(1) << bitOffset(i));
}

void Bitset::setOne(size_t i)
{
   word_[bitIndex(i)] |= (word_t(1) << bitOffset(i));
}

void Bitset::flip(size_t i)
{
   if (get(i)) setZero(i);
   else setOne(i);
}

size_t Bitset::wordCount() const
{
   return wcount_;
}

void Bitset::setAllZero()
{
   for (size_t i=0; i<wcount_; ++i)
      word_[i] = 0;
}

void Bitset::setAllOne()
{
   static word_t nz = ~0;

   for (size_t i=0; i<wcount_; ++i)
      word_[i] = nz;

   keepShadowBits();
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

void Bitset::print(std::ostream& os) const
{
   if (size_ == 0)
      os << "empty bitset";

   else
      for (size_t i=0; i<size_; ++i)
         os << (get(i) ? 1 : 0);
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
   size_t wc = std::min(wordCount(),other.wordCount());

   for (size_t i=0; i<wc; ++i)      
      if (word_[i] & other.word_[i])
         return true;

   return false;
}

Bitset& Bitset::operator&=(const Bitset& other)
{
   size_t nb = std::max(size(), other.size());

   if (nb == 0) return *this;

   Bitset aux(nb);

   size_t wc = std::min(wordCount(), other.wordCount());
   for (size_t i=0; i<wc; ++i)
      aux.word_[i] = word_[i] & other.word_[i];

   if (wordCount() < other.wordCount())
      for (size_t i=wc; i<other.wordCount(); ++i)
         aux.word_[i] = other.word_[i];

   if (wordCount() > other.wordCount())
      for (size_t i=wc; i<wordCount(); ++i)
         aux.word_[i] = word_[i];

   return *this = aux;
}

Bitset& Bitset::operator|=(const Bitset& other)
{
   size_t nb = std::max(size(), other.size());

   if (nb == 0) return *this;

   Bitset aux(nb);

   size_t wc = std::min(wordCount(), other.wordCount());
   for (size_t i=0; i<wc; ++i)
      aux.word_[i] = word_[i] | other.word_[i];

   if (wordCount() < other.wordCount())
      for (size_t i=wc; i<other.wordCount(); ++i)
         aux.word_[i] = other.word_[i];

   if (wordCount() > other.wordCount())
      for (size_t i=wc; i<wordCount(); ++i)
         aux.word_[i] = word_[i];

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

void Bitset::printWord(word_t w)
{
   word_t x = w;
   for (size_t i=0; i<BITS_PER_WORD; ++i)
   {
      std::cout << (x >> (BITS_PER_WORD - 1));
      x = (x << 1);
   }
   std::cout << std::endl;
}

} // namespace
