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
 * @file   ContractorFactory.hpp
 * @brief  Factory of interval contractors
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CONTRACTOR_FACTORY_HPP
#define REALPAVER_CONTRACTOR_FACTORY_HPP

#include "realpaver/ContractorACID.hpp"
#include "realpaver/ContractorAffine.hpp"
#include "realpaver/ContractorBC4.hpp"
#include "realpaver/ContractorDomain.hpp"
#include "realpaver/ContractorHC4.hpp"
#include "realpaver/ContractorPolytope.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Env.hpp"
#include "realpaver/IntervalNewton.hpp"
#include "realpaver/Problem.hpp"
#include <vector>

namespace realpaver {

/**
 * @brief Factory of interval contractors on a problem.
 *
 * The contractors are parameterized by values found in an environment.
 * A DAG is created from equations and inequality constraints of the given
 * problem and the contractors are based on it.
 */
class ContractorFactory {
public:
   /// Creates a factory given a problem and an environment for the parameters
   ContractorFactory(const Problem &pbm, std::shared_ptr<Env> env);

   /// Default destructor
   ~ContractorFactory() = default;

   /// No copy
   ContractorFactory(const ContractorFactory &) = delete;

   /// No assignment
   ContractorFactory &operator=(const ContractorFactory &) = delete;

   /// Returns the environment of this
   std::shared_ptr<Env> getEnv() const;

   /// Returns the dag of equations and inequality constraints
   SharedDag getDag() const;

   /// Returns an IntervalSmearSumRel object on the DAG
   std::shared_ptr<IntervalSmearSumRel> makeSSR();

   /**
    * @brief Returns an HC4 contractor.
    *
    * Creates a contractor that implements a constraint propagation algorithm
    * based on HC4 contractors.
    * - An HC4 contractor is created for each equation and inequality
    *   constraint.
    * - A constraint contractor is created for each other constraint.
    * - A domain contractor is created for each variable whose initial domain
    *   is not connected.
    *
    * tol is the tolerance driving propagation steps.
    */
   SharedContractorHC4 makeHC4(double tol);

   /// Same as makeHC4(tol) with tol extracted from the environment
   SharedContractorHC4 makeHC4();

   /**
    * @brief Returns a BC4 contractor.
    *
    * Creates a contractor that implements a constraint propagation algorithm
    * based on BC4 contractors.
    * - A BC4 contractor is created for each equation or inequality constraint.
    * - A constraint contractor is created for each other constraint.
    * - A domain contractor is created for each variable whose initial domain
    *   is not connected.
    */
   SharedContractorBC4 makeBC4();

   /**
    * @brief Returns an affine contractor.
    *
    * Creates a contractor that implements a constraint propagation algorithm
    * based on affine contractors.
    * - An affine contractor is created for each equation and inequality
    *   constraint.
    * - A constraint contractor is created for each other constraint.
    * - A domain contractor is created for each variable whose initial domain
    *   is not connected.
    *
    * tol is the tolerance driving propagation steps.
    */
   SharedContractorAffine makeAffine(double tol);

   /// Same as makeAffine(tol) with tol extracted from the environment
   SharedContractorAffine makeAffine();

   /**
    * @brief Returns an ACID contractor.
    *
    * Creates a contractor that applies CID contractors in sequence. The
    * contractor for slices is created by makeHC4().
    *
    * Returns nullptr if it is not possible to create the contractor.
    */
   SharedContractorACID makeACID();

   /**
    * @brief Returns an interval Newton operator.
    *
    * Creates an interval Newton operator associated with the equations if
    * there are at least two equations and the system is square, nullptr
    * otherwise.
    */
   std::shared_ptr<IntervalNewton> makeNewton();

   /**
    * @brief Returns a domain contractor.
    *
    * Creates a domain contractor for each variable whose initial domain
    * is not connected if there is at least one such variable, nullptr
    * otherwise.
    */
   std::shared_ptr<ContractorDomain> makeContractorDomain();

   /**
    * @brief Returns a polytope hull contractor.
    *
    * Creates a contractor that calculates the hull of a polytope
    * generated by a relaxation method.
    */
   SharedContractorPolytope makePolytope();

private:
   std::shared_ptr<Env> env_;   // environment for parameters
   SharedDag dag_;              // DAG of numerical constraints
   std::vector<size_t> ve_;     // indexes of equations in the DAG
   std::vector<size_t> vi_;     // indexes of inequalities in the DAG
   std::vector<Constraint> vc_; // other constraints

   Scope se_; // scope of equations
   Scope si_; // scope of inequalities
   Scope sc_; // scope of the other constraints
};

} // namespace realpaver

#endif
