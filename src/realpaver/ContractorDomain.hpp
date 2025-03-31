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
 * @file   ContractorDomain.hpp
 * @brief  Contractor of domains
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CONTRACTOR_DOMAIN_HPP
#define REALPAVER_CONTRACTOR_DOMAIN_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/Variable.hpp"
#include <vector>

namespace realpaver {

/**
 * @brief Contractor for variables having non interval domains.
 *
 * Given a variable v and an interval box X, let dom be the initial domain
 * of v and let X[v] be the domain of v in X. This operator contracts X[v] as
 * the hull of X[v] inter dom.
 *
 * Such a contractor is able to manage one variable or a list of variables.
 */
class ContractorDomain : public Contractor {
public:
   /// Creates a contractor with no variable
   ContractorDomain();

   /// Creates a contractor for one variable
   ContractorDomain(Variable v);

   /// Creates a contractor for a list of variables
   ContractorDomain(const std::initializer_list<Variable> &l);

   /// Inserts a variable in the list of variables
   void insertVar(Variable v);

   /// Returns the number of variables
   size_t nbVars() const;

   Scope scope() const override;
   Proof contract(IntervalBox &B) override;
   void print(std::ostream &os) const override;

private:
   Scope scop_;
};

/// Type of shared pointers on contractors
using SharedContractorDomain = std::shared_ptr<ContractorDomain>;

} // namespace realpaver

#endif
