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
 * @file   Bitset.hpp
 * @brief  The Bitset class
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#ifndef REALPAVER_BITSET_HPP
#define REALPAVER_BITSET_HPP

#include <iostream>

namespace realpaver {

/**
 * @brief A bitset is a sequence of bits.
 *
 * The number of bits is fixed at runtime when an object is created. The bit
 * indexes are positive integers and the first index may be different from 0.
 */
class Bitset {
public:
   /** @brief Creates an empty bitset */
   Bitset();

   /**
    * @brief Creates a bitset initialized to (0,0,...,0)
    * @param n number of bits
    * 
    * Indexes of bits: 0..n-1
    */
   Bitset(size_t n);

   /**
    * @brief Creates a bitset initialized to (0,0,...,0)
    * @param first index of the first bit
    * @param last index of the last bit
    * 
    * Indexes of bits: first..last
    */
   Bitset(int first, int last);

   /**
    * @brief Creates a bitset in extension given a list of bits
    * @param l list of bits, each bit is equal to 0 or 1
    */
   Bitset(const std::initializer_list<int>& l);

   /** @brief Copy constructor */
   Bitset(const Bitset& other);

   /** @brief Move constructor */
   Bitset(Bitset&& other);

   /** @brief Destructor */
   ~Bitset();

   /** @brief Assignment */
   Bitset& operator=(const Bitset& other);

   /** @brief Move assignment */
   Bitset& operator=(Bitset&& other);

   /** @return the number of bits */
   size_t size() const;

   /** @return the index of the first bit */
   int first() const;

   /** @return the index of the last bit */
   int last() const;

   /** @return the number of words used to store the bits */
   size_t wordCount() const;

   /** @return 0 if the i-th bit is equal to 0, another value otherwise */
   size_t get(int i) const;

   /**
    * @brief Sets a bit to 0
    * @param i bit index
    */
   Bitset& setZero(int i);

   /** @brief Sets all bits to 0 */
   Bitset& setAllZero();

   /**
    * @brief Sets a bit to 1
    * @param i bit index
    */
   Bitset& setOne(int i);

   /** @brief Sets all bits to 1 */
   Bitset& setAllOne();

   /**
    * @brief Flips the value of one bit
    * @param i bit index
    */
   void flip(int i);

   /** @brief Flips all bits */
   void flipAll();

   /** @return the number of 0 in this */
   size_t nbZeros() const;

   /** @return the number of 1 in this */
   size_t nbOnes() const;

   /** @return true if all the bits are equal to 1 */
   bool areAllOnes() const;

   /** @return true if all the bits are equal to 0 */
   bool areAllZeros() const;

   /** @brief Output on a stream. */
   void print(std::ostream& os) const;

   /** @return hash code of this */
   size_t hashCode() const;

   /**
    * @brief Overlapping test
    * @param other a bitset
    * @return true if this and other share at least one bit equal to 1
    */
   bool overlaps(const Bitset& other) const;

   /**
    * @brief Bitwise AND with assignment
    * @param other a bitset
    * @return a reference to this
    * 
    * this(i) := this(i) AND other(i) for each common bit index i.
    */
   Bitset& operator&=(const Bitset& other);

   /**
    * @brief Bitwise OR with assignment
    * @param other a bitset
    * @return a reference to this
    * 
    * this(i) := this(i) OR other(i) for each common bit index i.
    */
   Bitset& operator|=(const Bitset& other);

   friend Bitset operator&(const Bitset& b1, const Bitset& b2);
   friend Bitset operator|(const Bitset& b1, const Bitset& b2);
   friend Bitset operator~(const Bitset& b);

private:
   typedef size_t word_t;
   static const size_t BITS_PER_WORD = 8 * sizeof(word_t);

   size_t size_;     // number of bits
   int first_;       // index of first bit
   int last_;        // index of last bit
   size_t wcount_;   // number of words
   word_t* word_;    // array of words

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
};

/** @brief Output on a stream */
std::ostream& operator<<(std::ostream& os, const Bitset& b);

/**
 * @brief Bitwise AND
 * @param b1 a bitset
 * @param b2 a bitset
 * @return b1 AND b2
 * 
 * The resulting bitset b is such that b(i) := b1(i) AND b2(i) for each
 * common bit index i of b1 and b2.
 */
Bitset operator&(const Bitset& b1, const Bitset& b2);

/**
 * @brief Bitwise OR
 * @param b1 a bitset
 * @param b2 a bitset
 * @return b1 OR b2
 * 
 * The resulting bitset b is such that b(i) := b1(i) OR b2(i) for each
 * common bit index i of b1 and b2.
 */
Bitset operator|(const Bitset& b1, const Bitset& b2);

/**
 * @brief Bitwise NOT
 * @param b a bitset
 * @return NOT b
 */
Bitset operator~(const Bitset& b);

} // namespace

#endif
