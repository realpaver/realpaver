///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
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
   /// Creates a contractor with no variable
   IntContractor();

   /// Creates a contractor for one variable
   /// @param v an integer variable
   IntContractor(Variable v);

   /// Creates a contractor for a list of variables
   /// @param l list of integer variables
   IntContractor(const std::initializer_list<Variable>& l);

   /// Inserts a variable in the list of variables
   /// @param v an integer variable
   void insertVar(Variable v);

   /// @return the number of variables
   size_t nbVars() const;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalRegion& reg) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   Scope s_;
};

/// Type of shared pointers on contractors
typedef std::shared_ptr<IntContractor> SharedIntContractor;

} // namespace

#endif
