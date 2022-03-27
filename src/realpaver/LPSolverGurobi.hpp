///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LPSOLVER_GUROBI_HPP
#define REALPAVER_LPSOLVER_GUROBI_HPP

#include <vector>
#include <gurobi_c++.h>
#include "realpaver/LPModel.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a wrapper class for the LP solver Gurobi.
///
/// It inherits the methods for creating a model from its base class.
/// It overrides the optimization method.
///////////////////////////////////////////////////////////////////////////////
class LPSolver : public LPModel {
public:
   /// Default constructor
   LPSolver();

   /// Destructor
   ~LPSolver();

   /// No copy
   LPSolver(const LPSolver&) = delete;

   /// No assignment
   LPSolver& operator=(const LPSolver&) = delete;

   /// Overrides the optimization method
   /// @return true if an optimal solution is found
   ///
   /// A Gurobi model is created first from the LP model. Then the optimize
   /// method of Gurobi is executed.
   bool optimize();

   /// Overrides the optimization method
   /// @return true if an optimal solution is found
   bool reoptimize();

private:
  GRBEnv* env_;
  GRBModel* simplex_;
  std::vector<GRBVar> vars_;

  void makeVars();
  GRBLinExpr makeGrbLinExpr(LinExpr e);
  void makeCtrs();
  void makeObj();
  void makeGurobiSimplex();

   bool run();
};

} // namespace

#endif
