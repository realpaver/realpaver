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
 * @file   ContractorPolytope.hpp
 * @brief  Polytope Hull contractor
 * @author Laurent Granvilliers
 * @date   2025-fev-11
 */

#ifndef REALPAVER_CONTRACTOR_POLYTOPE_HPP
#define REALPAVER_CONTRACTOR_POLYTOPE_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/Linearizer.hpp"
#include "realpaver/LPSolver.hpp"

namespace realpaver {

/**
 * @brief Polytope Hull contractor.
 *
 * This is a contractor based on linear relaxations of constraint systems,
 * which is parameterized by the relaxation method.
 *
 * Given a constraint system S, and a region R, it generates an outer
 * approximation A of the set of solutions to S in R defined by a polytope.
 * For each variable x, two LPs are solved: min or max x s.t. A in
 * order to contract the domain of x in R.
 *
 * Not all these LPs have to be solved in general, and the order can be
 * tuned according to Achterberg's heuristics, see: A. Baharev, T. Achterberg,
 * and E. Rév. Computation of an extractive distillation column with affine
 * arithmetic. AIChE journal, 2009, 55(7):1695-1704.
 */
class ContractorPolytope : public Contractor {
public:
   /// Creates a contractor based on a given relaxation technique
   ContractorPolytope(std::unique_ptr<Linearizer> lzr);

   /// Default destructor
   ~ContractorPolytope() = default;

   /// No copy
   ContractorPolytope(const ContractorPolytope&) = delete;

   /// No assignment
   ContractorPolytope& operator=(const ContractorPolytope&) = delete;

   /// Returns the tolerance on bounds / primal solutions
   double getBoundTol() const;

   /// Assigns the tolerance on bounds / primal solutions
   void setBoundTol(double tol);

   /// Returns the relaxation tolerance for the equations
   double getRelaxTol() const;

   /// Assigns the relaxation tolerance for the equations
   void setRelaxTol(double tol);

   /// Sets a time limit in seconds of the optimization method
   /// @param s a time limit in seconds
   void setMaxSeconds(double s);

   /// Returns the time limit in seconds of the optimization method
   double getMaxSeconds() const;

   /// Sets a limit of iterations of the optimization method
   void setMaxIter(size_t n);

   /// Returns the maximum number of iterations of the optimization method
   size_t getMaxIter() const;

   /// Returns the feasibility tolerance
   double getFeasTol() const;

   /// Assigns the feasibility tolerance to tol
   void setFeasTol(double tol);

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

private:
   std::unique_ptr<Linearizer> lzr_;   // relaxation method

   // parameters of the LP solver
   double maxseconds_;           // time limit
   size_t maxiter_;              // iteration limit
   double feastol_;              // feasibility tolerance

   // tunes the LP solver
   void tuneLPSolver(LPSolver& solver);

   // contraction methods
   Proof contractImpl(IntervalBox& B);
   Proof run(LPSolver& solver, IntervalBox& B, int *lb, int& nlb, int *rb,
             int& nrb, int& iv, LPSense& sense);

   // selects the next variable and the optimization sense
   void selectAchterberg(LPSolver& solver, IntervalBox& B, int *lb, int& nlb,
                         int *rb, int& nrb, int& iv, LPSense& sense);
   void selectNext(int *lb, int& nlb, int *rb, int& nrb, int& iv,
                   LPSense& sense);

   // used for debugging
   static void printState(int *lb, int& nlb, int *rb, int& nrb, int& iv,
                          LPSense& sense);
};

/// type of shared contractors
using SharedContractorPolytope = std::shared_ptr<ContractorPolytope>;

} // namespace

#endif
