// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_BITSET_HPP
#define REALPAVER_BITSET_HPP

#include <iostream>

namespace realpaver {

/*****************************************************************************
 * Class of bitsets. A bitset represents a sequence of bits and the number of
 * bits is fixed when an object is created.
 *****************************************************************************/
class Bitset {
public:
   // default constructor
   Bitset();

   // constructor of a bitset of 'n' bits assigned to 'val'
   Bitset(size_t n, size_t val = 0);

   // copy constructor
   Bitset(const Bitset& other);

   // move constructor
   Bitset(Bitset&& other);

   // destructor
   ~Bitset();

   // assignment
   Bitset& operator=(const Bitset& other);

   // move assignment
   Bitset& operator=(Bitset&& other);

   // returns the number of bits
   size_t size() const;

   // returns the number of words used to store the bits
   size_t wordCount() const;

   // returns 0 if the i-th bit is equal to 0, another value otherwise
   size_t get(size_t i) const;

   // assignment methods of one bit or all bits
   void setZero(size_t i);
   void setAllZero();
   void setOne(size_t i);
   void setAllOne();

   // a flip modifies a bit b in 1-b
   void flip(size_t i);
   void flipAll();

   // bit counters
   size_t nbZero() const;
   size_t nbOne() const;

   // output on a stream
   void print(std::ostream& os) const;

   // hash code
   size_t hashCode() const;

   // returns true if two bitsets have one common bit that is equal to one
   bool overlaps(const Bitset& other) const;

   // bitwise AND
   Bitset& operator&=(const Bitset& other);
   friend Bitset operator&(const Bitset& b1, const Bitset& b2);

   // bitwise OR
   Bitset& operator|=(const Bitset& other);
   friend Bitset operator|(const Bitset& b1, const Bitset& b2);

   // bitwise NOT
   friend Bitset operator~(const Bitset& b);

private:
   typedef size_t word_t;
   static const size_t BITS_PER_WORD = 8 * sizeof(word_t);

   size_t size_;    // number of bits
   size_t wcount_;  // number of words
   word_t* word_;  // array of words

   // let (b0, b1, ..., b_{n-1}) be a bitset (i.e. n == size_)
   // let w be the number of words (i.e. w == wcount_)
   // let k be the number of bits in a word
   // word_[0] stores the first k bits in reverse order (b_{k-1},...,b0)
   // word_[1] stores (b_{2k-1},...,bk)
   // ...
   // words[w-1] stores (0,...,0,b_{n-1},...,b_{k*(w-1)}) and the bits
   // 0,...,0 are the shadow bits which must remain equal to 0

   // returns the index of the word that stores the i-th bit
   static size_t bitIndex(size_t i) { return i / BITS_PER_WORD; }

   // returns the position of the i-th bit in the word bitIndex(i)
   static size_t bitOffset(size_t i) { return i % BITS_PER_WORD; }

   // assigns the last irrelevant bits in the last word to 0
   void keepShadowBits();

   // used by keepShadowBits
   word_t shadow_word_;

   // used for debugging
   static void printWord(word_t w);
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const Bitset& b);

// bitwise operations
Bitset operator&(const Bitset& b1, const Bitset& b2);
Bitset operator|(const Bitset& b1, const Bitset& b2);
Bitset operator~(const Bitset& b);

inline size_t Bitset::size() const 
{
   return size_;
}

inline size_t Bitset::get(size_t i) const
{
   return i >= size() ?
             0 : word_[bitIndex(i)] & (word_t(1) << bitOffset(i));
}

inline void Bitset::keepShadowBits()
{
   if (wcount_ > 0)
      word_[wcount_-1] &= shadow_word_;
}

inline void Bitset::setZero(size_t i)
{
   word_[bitIndex(i)] &= ~(word_t(1) << bitOffset(i));
}

inline void Bitset::setOne(size_t i)
{
   word_[bitIndex(i)] |= (word_t(1) << bitOffset(i));
}

inline void Bitset::flip(size_t i)
{
   if (get(i)) setZero(i);
   else setOne(i);
}

inline size_t Bitset::wordCount() const
{
   return wcount_;
}

} // namespace

#endif
