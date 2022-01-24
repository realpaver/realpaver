#ifndef RPO_SIMPLEX_GUROBI_HPP
#define RPO_SIMPLEX_GUROBI_HPP

#include <vector>

#include "gurobi_c++.h"
#include "rpo_lin_model.hpp"

class RpoSimplex : public RpoLinModel {
public:
  RpoSimplex();
  ~RpoSimplex();

  bool optimize();

private:
  GRBEnv* env_;
  GRBModel* simplex_;
  std::vector<GRBVar> vars_;

  void makeVars();
  GRBLinExpr makeGrbLinExpr(RpoLinExpr e);
  void makeCtrs();
  void makeObj();
  void makeGurobiSimplex();
};

#endif
