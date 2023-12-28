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

#include <memory>
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
   /// @param scop scope of this such that the domains are extracted from
   ///        the variables
   DomainBox(Scope scop);

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
   Domain* get(const Variable& v) const;

   /// Gets a domain in this
   /// @param i an index between 0 and size()-1
   /// @return the domain of the i-th variable in this
   Domain* get(size_t i) const;

   /// Gets a variable in this
   /// @param i an index between 0 and size()-1
   /// @return the i-th variable of (the scope of) this
   Variable var(size_t i) const;

   /// Assigns the domain of a variable in this
   /// @param v a variable
   /// @param p the new domain of v
   ///
   /// The ownership of the domain pointer is moved from p to this.
   void set(const Variable& v, std::unique_ptr<Domain> p);

   /// @return a clone of this
   DomainBox* clone() const;

   /// Tests if the domain of a variable is splitable
   /// @param v a variable in this
   /// @return true if the domain of v in this has the desired tolerance
   bool isSplitable(const Variable& v) const;

   /// Display on a stream with one variable per line
   /// @param os an output stream
   ///
   /// Example:
   /// x = [1.35, 1.36]
   /// y = 2
   void listPrint(std::ostream& os) const;

   /// Display on a stream using a vector notation
   /// @param os an output stream
   ///
   /// Example:
   /// (x = [1.35, 1.36], y = 2)
   void vecPrint(std::ostream& os) const;

   /// @return the width of the interval box corresponding to the hull of this
   double width() const;

   /// Gap between boxes
   /// @param box a box
   /// @return the maximum gap componentwise between this and B
   ///
   /// Assumption: this and box have the same scope
   double gap(const DomainBox& box) const;

   /// Gap between boxes on a scope
   /// @param box a box
   /// @param scop a scope
   /// @return the maximum gap componentwise between this and box
   ///         restricted to scop
   ///
   /// Assumption: sco is included in the scopes of this and box
   double gapOnScope(const DomainBox& box, const Scope& scop) const;

   /// Glue another box in this
   /// @param box a box whose scope contains the scope of this
   ///
   /// for each variable v in the scope of this, the domain of v in this
   /// is assigned to an interval domain that is the hull of the input
   /// domain of v in this and the domain of v in box
   void glue(const DomainBox& box);

   /// Glue another box in this, restricted to some scope
   /// @param box a box
   /// @param scop a scope included in the scope of this and box
   ///
   /// for each variable v in the given scope, the domain of v in this
   /// is assigned to an interval domain that is the hull of the input
   /// domain of v in this and the domain of v in box
   void glueOnScope(const DomainBox& box, const Scope& scop);

   /// @return the sum of the widths of the hulls of the components of this
   double perimeter() const;

   /// @return the sum of the widths of some components of this
   /// @param scop the components considered
   double perimeterOnScope(const Scope& scop) const;

   /// @return the grid perimeter of this
   ///
   /// Given xi in Di with tolerance Ei for each i, the grid perimeter is the
   /// sum for each i of the following quantity:
   /// - 0.0 if Di has tolerance Ei
   /// - width(Di) / Ei (where Ei is the absolute tolerance) otherwise
   double gridPerimeter() const;

   /// @return the grid perimeter for some components of this
   /// @param scop the components considered   
   double gridPerimeterOnScope(const Scope& scop) const;

private:
   Scope scop_;                  // ordered set of variables
   std::vector<Domain*> doms_;   // domains
};

/// Display in a stream, calls vecPrint
std::ostream& operator<<(std::ostream& os, const DomainBox& box);

} // namespace

#endif
