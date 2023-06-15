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

#include "realpaver/LocalOptimizer.hpp"

#include <nlopt.hpp>

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for local optimization solvers.
///////////////////////////////////////////////////////////////////////////////
class LocalOptimizerNlopt : public LocalOptimizer {
public:
   /// Default constructor
   LocalOptimizerNlopt(const Problem& pb);
   LocalOptimizerNlopt(const RealFunction& obj, const RealFunctionVector& ctrs);

   /// Virtual destructor
   virtual ~LocalOptimizerNlopt();

   /// Minimization of a problem
   /// @param reg interval region in the search space
   /// @param src starting point that belongs to the region
   /// @return an optimization status
   virtual OptimizationStatus minimize(const IntervalRegion& reg,
                                       const RealPoint& src);


   void algorithm_name(nlopt::algorithm alg);
   nlopt::algorithm algorithm_name() const;

protected:
   std::shared_ptr<nlopt::opt> optimizer_;                              // Point to the nlp optimizer object
   nlopt::algorithm algorithm_ = nlopt::algorithm::LN_NELDERMEAD;       // solving algorithm for nlopt

};

} // namespace

#endif
