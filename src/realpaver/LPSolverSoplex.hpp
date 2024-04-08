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
 * @file   LPSolverSoplex.hpp
 * @brief  Wrapper class for the LP solver Soplex
 * @author Raphaël Chenouard
 * @date   2023-2-19
*/

#ifndef REALPAVER_LPSOLVER_SOPLEX_HPP
#define REALPAVER_LPSOLVER_SOPLEX_HPP

#include <soplex.h>
#include "realpaver/LPModel.hpp"

namespace realpaver {

/**
 * @brief Wrapper class for the LP solver Soplex.
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
   soplex::SoPlex* simplex_;
  
   void makeVars();
   void makeCtrs();
   void makeObj();
   void makeSoplexSimplex();

   bool run();
};

} // namespace

#endif
