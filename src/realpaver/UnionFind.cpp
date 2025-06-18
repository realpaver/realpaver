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
 * @file   UnionFind.cpp
 * @brief  Union-Find data structure
 * @author Laurent Granvilliers
 * @date   16 June 2025
 */

#include "realpaver/UnionFind.hpp"
#include <unordered_map>

namespace realpaver {

UnionFind::UnionFind(size_t n)
    : par_(n)
    , h_(n)
    , np_(n)
{
   for (size_t x = 0; x < n; ++x)
   {
      par_[x] = x;
      h_[x] = 0;
   }
}

UnionFind::~UnionFind()
{
}

size_t UnionFind::nbElems() const
{
   return par_.size();
}

size_t UnionFind::nbParts() const
{
   return np_;
}

size_t UnionFind::find(size_t x)
{
   if (par_[x] != x)
      par_[x] = find(par_[x]);
   return par_[x];
}

void UnionFind::merge(size_t x, size_t y)
{
   size_t rx = find(x), ry = find(y);
   if (rx != ry)
   {
      if (h_[rx] > h_[ry])
         par_[ry] = rx;

      else
      {
         par_[rx] = ry;
         if (h_[ry] == h_[rx])
            h_[ry]++;
      }
      np_--;
   }
}

std::vector<std::vector<size_t>> UnionFind::parts()
{
   std::vector<std::vector<size_t>> v(np_);
   std::unordered_map<size_t, size_t> root;

   size_t ip = 0, i;
   for (size_t x = 0; x < nbElems(); ++x)
   {
      size_t rx = find(x);
      auto it = root.find(rx);
      if (it == root.end())
      {
         root.insert(std::make_pair(rx, ip));
         i = ip++;
         v[i].push_back(rx);
      }
      else
      {
         i = it->second;
      }
      if (x != rx)
         v[i].push_back(x);
   }

   return v;
}

std::ostream &operator<<(std::ostream &os, const UnionFind &uf)
{
   UnionFind &aux = const_cast<UnionFind &>(uf);
   std::vector<std::vector<size_t>> v = aux.parts();
   os << "{";
   for (size_t i = 0; i < v.size(); ++i)
   {
      if (i > 0)
         os << ", ";
      std::vector<size_t> part = v[i];
      os << "{";
      for (size_t j = 0; j < part.size(); ++j)
      {
         if (j > 0)
            os << ", ";
         os << part[j];
      }
      os << "}";
   }
   os << "}";
   return os;
}

} // namespace realpaver
