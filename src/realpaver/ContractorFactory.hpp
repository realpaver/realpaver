///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_FACTORY_HPP
#define REALPAVER_CONTRACTOR_FACTORY_HPP

#include <vector>
#include "realpaver/ContractorACID.hpp"
#include "realpaver/ContractorDomain.hpp"
#include "realpaver/ContractorPropag.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/IntervalNewton.hpp"
#include "realpaver/Problem.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a factory of interval contractors.
///////////////////////////////////////////////////////////////////////////////
class ContractorFactory {
public:
   /// Creates a factory on a problem
   /// @param pbm the input problem
   ContractorFactory(const Problem& pbm);

   /// Default destructor
   ~ContractorFactory() = default;

   /// No copy
   ContractorFactory(const ContractorFactory&) = delete;

   /// No assignment
   ContractorFactory& operator=(const ContractorFactory&) = delete;

   /// @return the dag of equations and inequality constraints
   SharedDag getDag() const;

   /// Creates a contractor that implements a constraint propagation algorithm
   /// based on HC4 contractors
   /// @return the contractor
   ///
   /// An HC4 contractor is created for each equation and inequality constraint.
   /// A constraint contractor is created for each other constraint.
   /// A domain contractor is created for each variable whose initial domain
   /// is not connected.
   SharedContractorPropag makeHC4();

   /// Creates a contractor that implements a constraint propagation algorithm
   /// based on BC4 contractors
   /// @return the propagator
   ///
   /// A BC4 contractor is created for each equation or inequality constraint.
   /// A constraint contractor is created for each other constraint.
   /// A domain contractor is created for each variable whose initial domain
   /// is not connected.
   SharedContractorPropag makeBC4();

   /// Creates a contractor that applies CID contractors in sequence
   /// @param nbs number of slices of CID contractors
   /// @return the contractor
   ///
   /// The contractor for slices is created by makeHC4().
   SharedContractorACID makeACID(size_t nbs);

   /// Creates an interval Newton operator associated with the equations
   /// @return the interval Newton operator associated with the equations
   ///         if there are at least two equations and the system is square,
   ///         nullptr otherwise
   std::shared_ptr<IntervalNewton> makeIntervalNewton();

   /// Creates a domain contractor for each variable whose initial domain
   /// is not connected
   /// @return the contractor if there is at least one such variable,
   ///         nullptr otherwise
   std::shared_ptr<ContractorDomain> makeContractorDomain();

private:
   SharedDag dag_;               // DAG of numerical constraints
   std::vector<size_t> ve_;      // indexes of equations in the DAG
   std::vector<size_t> vi_;      // indexes of inequalities in the DAG
   std::vector<Constraint> vc_;  // other constraints

   Scope se_;                    // scope of equations
   Scope si_;                    // scope of inequalities
   Scope sc_;                    // scope of the other constraints
};

} // namespace

#endif
