///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LOCAL_SOLVER_NLOPT_HPP
#define REALPAVER_LOCAL_SOLVER_NLOPT_HPP

#include "realpaver/LocalSolver.hpp"

#include <nlopt.hpp>

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for local optimization solvers.
///////////////////////////////////////////////////////////////////////////////
class LocalSolverNlopt : public LocalSolver {
public:
   /// Default constructor
   LocalSolverNlopt(const Problem& pb);
   LocalSolverNlopt(const RealFunction& obj, const RealFunctionVector& ctrs);

   /// Virtual destructor
   virtual ~LocalSolverNlopt();

   /// Minimization of a problem
   /// @param reg interval region in the search space
   /// @param src starting point that belongs to the region
   /// @param dest final point found by the optimization procedure
   /// @return an optimization status
   virtual OptimizationStatus minimize(const IntervalRegion& reg,
                                       const RealPoint& src,
                                       RealPoint& dest);


   void set_algorithm_name(nlopt::algorithm alg);

protected:
   std::shared_ptr<nlopt::opt> optimizer_;
   nlopt::algorithm algorithm_ = nlopt::algorithm::LD_SLSQP;

   double f_(const std::vector<double> &x, std::vector<double> &grad, void* f_data);
};

} // namespace

#endif
