// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_WRAPPER_LPSOLVER_CLP_HPP
#define REALPAVER_WRAPPER_LPSOLVER_CLP_HPP

#include <ClpSimplex.hpp>
#include "realpaver/lp_model.hpp"

namespace realpaver {

/*****************************************************************************
 * Wrapper class for the Clp solver.
 *****************************************************************************/
class LPSolver : public LPModel {
public:
   LPSolver();
   ~LPSolver();

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
