///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BITSET_HPP
#define REALPAVER_BITSET_HPP

#include <iostream>

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a sequence of bits.
///
/// The number of bits is fixed when an object is created.
///////////////////////////////////////////////////////////////////////////////
class Bitset {
public:
   /// Creates an empty bitset
   Bitset();

   /// Creates a bitset
   /// @param n number of bits
   /// @param val each bit is initialized to 0 is val = 0, 1 otherwise
   Bitset(size_t n, size_t val = 0);

   /// Creates a bitset
   /// @param l list of bits
   Bitset(const std::initializer_list<int>& l);

   /// Cpy constructor
   Bitset(const Bitset& other);

   /// Move constructor
   Bitset(Bitset&& other);

   /// Destructor
   ~Bitset();

   /// Assignment
   Bitset& operator=(const Bitset& other);

   /// Move assignment
   Bitset& operator=(Bitset&& other);

   /// @return the number of bits
   size_t size() const;

   /// @return the number of words used to store the bits
   size_t wordCount() const;

   /// @return 0 if the i-th bit is equal to 0, another value otherwise
   size_t get(size_t i) const;

   /// Sets a bit to 0
   /// @param i index of the bit assigned
   void setZero(size_t i);

   /// Sets all bits to 0
   void setAllZero();

   /// Sets a bit to 1
   /// @param i index of the bit assigned
   void setOne(size_t i);

   /// Sets all bits to 1
   void setAllOne();

   /// Flips the value of one bit
   /// @param i index of the bit modified
   void flip(size_t i);

   /// Flips all bits
   void flipAll();

   /// @return the number of 0 in this
   size_t nbZero() const;

   /// @return the number of 1 in this
   size_t nbOne() const;

   /// Output on a stream
   /// @param os output stream
   void print(std::ostream& os) const;

   /// @return hash code of this
   size_t hashCode() const;

   /// Overlapping test
   /// @param other a bitset
   /// @return true if this and other share at least one bit equal to 1
   bool overlaps(const Bitset& other) const;

   /// Bitwise AND with assignment
   /// @param other a bitset
   /// @return a reference to this
   Bitset& operator&=(const Bitset& other);

   /// Bitwise OR with assignment
   /// @param other a bitset
   /// @return a reference to this
   Bitset& operator|=(const Bitset& other);

   friend Bitset operator&(const Bitset& b1, const Bitset& b2);
   friend Bitset operator|(const Bitset& b1, const Bitset& b2);
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

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Bitset& b);

/// Bitwise AND
/// @param b1 a bitset
/// @param b2 a bitset
/// @return b1 AND b2
Bitset operator&(const Bitset& b1, const Bitset& b2);

/// Bitwise OR
/// @param b1 a bitset
/// @param b2 a bitset
/// @return b1 OR b2
Bitset operator|(const Bitset& b1, const Bitset& b2);

/// Bitwise NOT
/// @param b a bitset
/// @return NOT b
Bitset operator~(const Bitset& b);

} // namespace

#endif
