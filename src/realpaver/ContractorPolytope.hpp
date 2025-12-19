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
 * @date   11 Feb 2025
 */

#ifndef REALPAVER_CONTRACTOR_POLYTOPE_HPP
#define REALPAVER_CONTRACTOR_POLYTOPE_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/LPSolver.hpp"
#include "realpaver/Linearizer.hpp"

namespace realpaver {

/**
 * @brief Base class implementing a choice of contraction methods.
 *
 * Any contractor has two contractions methods, one that simply contracts a box B,
 * and one that takes into account an active bitset. Such an objet has an apply(ctc, B)
 * method that must apply one contraction method of ctc on B.
 */
class LinearizeMethod {
public:
   /// Virtual destructor
   virtual ~LinearizeMethod();

   /// Applies a linearization method of lzr on B to create the LP model
   virtual bool apply(Linearizer &lzr, LPModel &lpm, IntervalBox &B) const = 0;
};

/// Calls the linearization method with no bitset
class LinearizeSimple : public LinearizeMethod {
public:
   /// apply lzr.make(solver, B)
   bool apply(Linearizer &lzr, LPModel &lpm, IntervalBox &B) const override;
};

/// Calls the linearization method with an active bitset
class LinearizeActive : public LinearizeMethod {
public:
   /// Creates an object encapsulating an active set
   LinearizeActive(const Bitset &active);

   /// apply lzr.make(solver, B, active)
   bool apply(Linearizer &lzr, LPModel &lpm, IntervalBox &B) const override;

private:
   const Bitset &active_;
};

/*----------------------------------------------------------------------------*/

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
 *
 * If there is an objective variable, it is minimized after all the other variables.
 */
class ContractorPolytope : public Contractor {
public:
   /// Creates a contractor based on a given relaxation technique
   ContractorPolytope(std::unique_ptr<Linearizer> lzr);

   /// Destructor
   ~ContractorPolytope();

   /// No copy
   ContractorPolytope(const ContractorPolytope &) = delete;

   /// No assignment
   ContractorPolytope &operator=(const ContractorPolytope &) = delete;

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

   /**
    * @brief Controls the contraction method.
    *
    * If loop = false then it is applied only once. Otherwise, it is applied
    * in a fixed-point loop given tol as improvement factor, which is a relative
    * tolerance in [0, 1].
    */
   void enforceLoop(bool loop, double tol = 0.0);

   /**
    * @brief Result of the minimization of the objective variable.
    *
    * Returns true if the LP problem "minimize z" where z is the objective variable
    * has been successfully solved by the last call of a contract method, false otherwise.
    */
   bool hasMinimizer() const;

   /**
    * @brief Result of the minimization of the objective variable.
    *
    * If hasMinimizer()==true, it returns the vertex of the polytope leading to the
    * optimum of the problem "minimize z" where z is the objective variable.
    */
   const RealPoint &minimizer() const;

   Scope scope() const override;
   Proof contract(IntervalBox &B) override;
   Proof contract(IntervalBox &B, const Bitset &active) override;
   void print(std::ostream &os) const override;

private:
   std::unique_ptr<Linearizer> lzr_; // relaxation method

   // parameters of the LP solver
   double maxseconds_; // time limit
   size_t maxiter_;    // iteration limit
   double feastol_;    // feasibility tolerance
   bool loop_;         // true when applied in a fixed-point loop
   double looptol_;    // improvement factor of the fixed-point loop
   int id_var_obj_;    // local index of the objective variable in the scope if it
   // exists, -1 otherwise

   bool has_minimizer_;
   RealPoint *minimizer_;

   // tunes the LP solver
   void tuneLPSolver(LPSolver &solver);

   // contraction methods
   Proof contractLoop(IntervalBox &B, const LinearizeMethod &linearize);
   Proof contractImpl(IntervalBox &B, const LinearizeMethod &linearize);
   Proof run(LPSolver &solver, IntervalBox &B, int *lb, int &nlb, int *rb, int &nrb,
             int &iv, LPSense &sense);

   // selects the next variable and the optimization sense
   void selectAchterberg(LPSolver &solver, IntervalBox &B, int *lb, int &nlb, int *rb,
                         int &nrb, int &iv, LPSense &sense);
   void selectNext(int *lb, int &nlb, int *rb, int &nrb, int &iv, LPSense &sense);

   void makeMinimizer(LPSolver &solver);
};

/// type of shared contractors
using SharedContractorPolytope = std::shared_ptr<ContractorPolytope>;

} // namespace realpaver

#endif
