///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INT_CONTRACTOR_HPP
#define REALPAVER_INT_CONTRACTOR_HPP

#include <vector>
#include "realpaver/Contractor.hpp"
#include "realpaver/Variable.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor for integer variables.
///
/// Given an interval vector X and integer variable v, let X[v] be the domain
/// of v. This operator contracts X[v] as the hull of all the integers in X[v].
///
/// Such a contractor is able to manage one variable or a list of variables.
///////////////////////////////////////////////////////////////////////////////
class IntContractor : public Contractor {
public:
   /// Creates acontractor with no variable
   IntContractor();

   /// Creates a contractor for one variable
   /// @param v an integer variable
   IntContractor(const Variable& v);

   /// Creates a contractor for a list of variables
   /// @param l list of integer variables
   IntContractor(const std::initializer_list<Variable>& l);

   /// Inserts a variable in the list of variables
   /// @param v an integer variable
   void insert(const Variable& v);

   ///@{
   /// Overrides (Contractor)
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalRegion& reg);
   void print(std::ostream& os) const;
   ///@}

private:
   Scope s_;
   Bitset b_;
};

} // namespace

#endif
