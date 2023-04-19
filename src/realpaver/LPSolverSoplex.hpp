///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LPSOLVER_SOPLEX_HPP
#define REALPAVER_LPSOLVER_SOPLEX_HPP

#include <soplex.h>
#include "realpaver/LPModel.hpp"

// using namespace soplex;

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a wrapper class for the LP solver Soplex.
///
/// It inherits the methods for creating a model from its base class.
/// It implements the optimization method.
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

   /// Optimization method
   /// @return true if an optimal solution is found
   ///
   /// A Gurobi model is created first from the LP model. Then the initialSolve
   /// method of Clp is executed.
   bool optimize();

   /// Optimization method which requires that optimize() has been called at
   /// least once
   /// @return true if an optimal solution is found
   ///
   /// Only the objective function is generated again from the LP model.
   /// Then the initialSolve method of Clp is executed.
   bool reoptimize();

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
