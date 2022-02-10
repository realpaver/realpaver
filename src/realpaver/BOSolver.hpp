///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BO_SOLVER_HPP
#define REALPAVER_BO_SOLVER_HPP

#include "realpaver/BOModel.hpp"
#include "realpaver/Preprocessor.hpp"
#include "realpaver/Problem.hpp"

namespace realpaver {

class BOSolver {
public:
   BOSolver(Problem& problem);

   ~BOSolver();

   /// Optimization method
   /// @return true if an optimal solution is found
   bool optimize();

   /// @return the status of the last optimization stage
   OptimizationStatus getStatus() const;

   /// Sets a time limit of the optimization method
   /// @param s a time limit in seconds
   void setMaxSeconds(double s);

   /// @return the time limit of the optimization method
   double getMaxSeconds() const;

   /// @return the enclosure of the optimum after the optimization process
   Interval getObjEnclosure() const;

   /// @return the best point found by the optimization process
   RealVector getBestSolution() const;

private:
   Problem problem_;    // initial problem
   Problem preprob_;    // problem resulting from preprocessing
   Problem solprob_;    // problem resulting from presolving

   BOModel* model_;

   OptimizationStatus status_;
   IntervalVector sol_;
   Interval objval_;

   bool preprocess();
   bool presolve();
   void solve();

   typedef Preprocessor::VarVarMapType VarVarMapType;

   // Maps every variable of the preprocessed prolem to the associated
   // variable in the initial problem
   VarVarMapType vmap21_;

   // Maps every variable of the presolved prolem to the associated
   // variable in the initial problem
   VarVarMapType vmap31_;
};

} // namespace

#endif
