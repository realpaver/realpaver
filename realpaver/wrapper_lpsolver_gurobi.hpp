// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_WRAPPER_LPSOLVER_GUROBI_HPP
#define REALPAVER_WRAPPER_LPSOLVER_GUROBI_HPP

#include <vector>
#include <gurobi_c++.h>
#include "realpaver/lp_model.hpp"

namespace realpaver {

class LPSolver : public LPModel {
public:
  LPSolver();
  ~LPSolver();

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
