///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_DOM_HPP
#define REALPAVER_CONTRACTOR_DOM_HPP

#include <vector>
#include "realpaver/Contractor.hpp"
#include "realpaver/Variable.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor for variables having non interval domains.
///
/// Given a variable v and an interval box X, let dom be the initial domain
/// of v and let X[v] be the domain of v in X. This operator contracts X[v] as
/// the hull of X[v] inter dom.
///
/// Such a contractor is able to manage one variable or a list of variables.
///////////////////////////////////////////////////////////////////////////////
class ContractorDom : public Contractor {
public:
   /// Creates a contractor with no variable
   ContractorDom();

   /// Creates a contractor for one variable
   /// @param v a variable
   ContractorDom(Variable v);

   /// Creates a contractor for a list of variables
   /// @param l list of variables
   ContractorDom(const std::initializer_list<Variable>& l);

   /// Inserts a variable in the list of variables
   /// @param v a variable
   void insertVar(Variable v);

   /// @return the number of variables
   size_t nbVars() const;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& box) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   Scope s_;
};

/// Type of shared pointers on contractors
typedef std::shared_ptr<ContractorDom> SharedContractorDom;

} // namespace

#endif
