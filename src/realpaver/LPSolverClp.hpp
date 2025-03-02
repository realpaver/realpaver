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
 * @file   LPSolverClp.hpp
 * @brief  Wrapper class for the LP solver Clp
 * @author Laurent Granvilliers, Raphaël Chenouard
 * @date   2024-4-11
*/

#ifndef REALPAVER_LPSOLVER_CLP_HPP
#define REALPAVER_LPSOLVER_CLP_HPP

#include <ClpSimplex.hpp>
#include "realpaver/LPModel.hpp"

namespace realpaver {

/**
 * @brief Wrapper class for the LP solver Clp.
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

   LPStatus optimize() override;
   LPStatus reoptimize() override;
   double costSolution() const override;
   RealVector primalSolution() const override;
   RealVector dualSolution() const override;
   bool infeasibleRay(RealVector& ray) const override;

private:
   ClpSimplex* simplex_;

   // makes a Clp problem from a LPModel
   void makeVars();
   void makeCtrs();
   void makeCost();
   void makeSimplex();
   void setOptions();

   // optimization
   LPStatus run();

   // gets the status from Clp
   LPStatus toLPStatus();
};

} // namespace

#endif
