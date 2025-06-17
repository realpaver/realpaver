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
 * @file   UnionFind.hpp
 * @brief  Union-Find data structure
 * @author Laurent Granvilliers
 * @date   16 June 2025
 */

#ifndef REALPAVER_UNION_FIND_HPP
#define REALPAVER_UNION_FIND_HPP

#include <iostream>
#include <vector>

namespace realpaver {

class UnionFind {
public:
   /// Creates the partition {{0}, {1}, ..., {n-1}} of the set {0, 1, ..., n-1}
   UnionFind(size_t n);

   /// Default constructor
   UnionFind(const UnionFind &) = default;

   /// No assignment
   UnionFind &operator=(const UnionFind &) = delete;

   /// Default destructor
   ~UnionFind();

   /// Returns the cardinal of the set
   size_t nbElems() const;

   /// Returns the number of parts of the partition
   size_t nbParts() const;

   /// Finds and returns the element representing x
   size_t find(size_t x);

   /// Merges the parts containing x and y
   void merge(size_t x, size_t y);

   /// Returns the vector of parts
   std::vector<std::vector<size_t>> parts();

private:
   std::vector<size_t> par_; // vector of indexes of parent nodes
   std::vector<size_t> h_;   // vector of tree heights
   size_t np_;               // number of parts

   friend std::ostream &operator<<(std::ostream &os, const UnionFind &uf);
};

/// Display on a stream
std::ostream &operator<<(std::ostream &os, const UnionFind &uf);

} // namespace realpaver

#endif
