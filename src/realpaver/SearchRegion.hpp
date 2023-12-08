///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_SEARCH_REGION_HPP
#define REALPAVER_SEARCH_REGION_HPP

#include "realpaver/Domain.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a region of a search space occurring in a search node.
///
/// It associates a scope and a vector of variable domains indexed by the
/// local indexes of the variables from the scope.
///////////////////////////////////////////////////////////////////////////////
class SearchRegion {
public:
   /// Creates a region
   /// sco scope of this such that the domains are extracted from the variables
   SearchRegion(Scope sco);

   /// Copy constructor
   SearchRegion(const SearchRegion& reg);

   /// No asignment
   SearchRegion& operator=(const SearchRegion&) = delete;

   /// Destructor
   ~SearchRegion();

   /// @return the scope of this (sorted set of variables)
   Scope scope() const;

   /// @return the number of couples variable-domain
   size_t size() const;

   /// @reeturn true if this is empty
   bool isEmpty() const;

   /// Gets a domain in this
   /// @param v a variable that belongs to the scope of this
   /// @return the domain of v in this
   Domain* getDomain(const Variable& v) const;

   /// Gets a domain in this
   /// @param i an index between 0 and size()-1
   /// @return the domain of the i-th variable in this
   Domain* getDomain(size_t i) const;

   /// Gets a variable in this
   /// @param i an index between 0 and size()-1
   /// @return the i-th variable of (the scope of) this
   Variable getVar(size_t i) const;

   /// @return a clone of this
   SearchRegion* clone() const;

   /// Display on a stream
   /// @param os an output stream
   void print(std::ostream& os) const;

private:
   Scope sco_;                   // ordered set of variables
   std::vector<Domain*> doms_;   // domains
};

/// Display in a stream
std::ostream& operator<<(std::ostream& os, const SearchRegion& reg);

} // namespace

#endif
