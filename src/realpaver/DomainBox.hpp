///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_DOMAIN_BOX_HPP
#define REALPAVER_DOMAIN_BOX_HPP

#include "realpaver/Domain.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a list of couples variable / domain.
///
/// It associates a scope and a vector of variable domains indexed by the
/// local indexes of the variables from the scope.
///////////////////////////////////////////////////////////////////////////////
class DomainBox {
public:
   /// Creates a box
   /// sco scope of this such that the domains are extracted from the variables
   DomainBox(Scope sco);

   /// Copy constructor
   DomainBox(const DomainBox& box);

   /// No asignment
   DomainBox& operator=(const DomainBox&) = delete;

   /// Destructor
   ~DomainBox();

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
   DomainBox* clone() const;

   /// Tests if the domain of a variable is splitable
   /// @param v a variable in this
   /// @return true if the domain of v in this has the desired tolerance
   bool isSplitable(const Variable& v) const;

   /// Display on a stream
   /// @param os an output stream
   void print(std::ostream& os) const;

private:
   Scope sco_;                   // ordered set of variables
   std::vector<Domain*> doms_;   // domains
};

/// Display in a stream
std::ostream& operator<<(std::ostream& os, const DomainBox& box);

} // namespace

#endif
