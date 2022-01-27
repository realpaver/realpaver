///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, a reliable interval solver of nonlinear   //
//                                 constraint satisfaction and optimization  //
//                                 problems over the real numbers.           //
//                                                                           //
// Copyright (C) 2020-2022 Laboratoire des Sciences du Numérique de Nantes   //
//                                                                           //
// Realpaver is a software distributed under the terms of the MIT License.   //
// See the file COPYING.                                                     //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_WRAPPER_LPSOLVER_GUROBI_HPP
#define REALPAVER_WRAPPER_LPSOLVER_GUROBI_HPP

#include <vector>
#include <gurobi_c++.h>
#include "realpaver/lp_model.hpp"

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

private:
  GRBEnv* env_;
  GRBModel* simplex_;
  std::vector<GRBVar> vars_;

  void makeVars();
  GRBLinExpr makeGrbLinExpr(LinExpr e);
  void makeCtrs();
  void makeObj();
  void makeGurobiSimplex();
};

} // namespace

#endif
