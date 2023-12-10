///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NLP_SOLVER_NLOPT_HPP
#define REALPAVER_NLP_SOLVER_NLOPT_HPP

#include "realpaver/NLPModel.hpp"
#include <nlopt.hpp>

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an implementation of NLP solver for Nlopt.
///////////////////////////////////////////////////////////////////////////////
class NLPSolver : public NLPModel {
public:
   /// Creates a solver for an optimization problem
   /// @param pb an optimization problem
   NLPSolver(const Problem& pb);

   /// Creates a solver for an unconstrained optimization problem
   /// @param obj an objective function to be minimized
   NLPSolver(const RealFunction& obj);

   /// Creates a solver for a onstrained optimization problem
   /// @param obj an objective function to be minimized
   /// @param ctrs a vector of constraint functions
   NLPSolver(const RealFunction& obj, const RealFunctionVector& ctrs);

   /// Destructor
   ~NLPSolver();

   /// No copy
   NLPSolver(const NLPSolver&) = delete;

   /// No assignment
   NLPSolver& operator=(const NLPSolver&) = delete;

   /// Minimization of a problem
   /// @param box interval box in the search space
   /// @param src starting point that belongs to the box
   /// @return an optimization status
   ///
   /// Both scopes of reg and src must contain the scope of this.
   /// They do not necessarily correspond.
   OptimizationStatus minimize(const IntervalBox& box, const RealPoint& src);

   // Structure used to process a constraint
   struct Ctr {
      NLPSolver* ls;    // the solver vector
      size_t idx;       // constraint index
      bool isleft;      // true if the left bound is considered
   };

private:
   nlopt::opt* optimizer_;       // nlopt optimizer
   nlopt::algorithm nlopt_alg_;  // optimization technique
   std::vector<Ctr> nl_ctrs_;    // management of constraints

   void makeAlgorithm();
   void makeCtrs();
};

} // namespace

#endif
