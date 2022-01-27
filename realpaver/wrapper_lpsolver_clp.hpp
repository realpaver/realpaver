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

#ifndef REALPAVER_WRAPPER_LPSOLVER_CLP_HPP
#define REALPAVER_WRAPPER_LPSOLVER_CLP_HPP

#include <ClpSimplex.hpp>
#include "realpaver/lp_model.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a wrapper class for the LP solver Clp.
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
   /// A Clp model is created first from the LP model. Then the initialSolve
   /// method of Clp is executed.
   bool optimize();

private:
   ClpSimplex* simplex_;
  
   void makeVars();
   void makeCtrs();
   void makeObj();
   void makeClpSimplex();
};

} // namespace

#endif
