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
 * @file   LPSolverGurobi.hpp
 * @brief  Wrapper class for the LP solver Gurobi
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_LPSOLVER_GUROBI_HPP
#define REALPAVER_LPSOLVER_GUROBI_HPP

#include <vector>
#include <gurobi_c++.h>
#include "realpaver/LPModel.hpp"

namespace realpaver {

/**
 * @brief Wrapper class for the LP solver Gurobi.
 *
 * It inherits the methods for creating a model from its base class.
 * It implements the optimization method.
 */
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

   bool optimize() override;
   bool reoptimize() override;

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
