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
 * @date   2024-4-11
 */

#ifndef REALPAVER_LPSOLVER_SOPLEX_HPP
#define REALPAVER_LPSOLVER_SOPLEX_HPP

#include "realpaver/LPModel.hpp"
#include <soplex.h>

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
   LPSolver(const LPSolver &) = delete;

   /// No assignment
   LPSolver &operator=(const LPSolver &) = delete;

   LPStatus optimize() override;
   LPStatus reoptimize() override;
   double costSolution() const override;
   RealVector primalSolution() const override;
   RealVector dualSolution() const override;
   bool infeasibleRay(RealVector &ray) const override;

private:
   soplex::SoPlex *simplex_;

   void makeVars();
   void makeCtrs();
   void makeCost();
   void makeSoplexSimplex();
   void setOptions();

   // optimization
   LPStatus run();

   // gets the status from Highs
   LPStatus toLPStatus() const;
};

} // namespace realpaver

#endif
