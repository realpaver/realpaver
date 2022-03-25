///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LPSOLVER_CLP_HPP
#define REALPAVER_LPSOLVER_CLP_HPP

#include <ClpSimplex.hpp>
#include "realpaver/LPModel.hpp"

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

   /// Overrides
   ///
   /// A Clp model is created first from the LP model. Then the initialSolve
   /// method of Clp is executed.
   bool optimize();

   /// Overrides
   ///
   /// Only the objective function is generated again from the LP model.
   /// Then the initialSolve method of Clp is executed.
   bool reOptimize();

private:
   ClpSimplex* simplex_;
  
   void makeVars();
   void makeCtrs();
   void makeObj();
   void makeClpSimplex();

   bool run();
};

} // namespace

#endif
