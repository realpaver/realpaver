#ifndef REALPAVER_WRAPPER_LPSOLVER_CLP_HPP
#define REALPAVER_WRAPPER_LPSOLVER_CLP_HPP

#include <ClpSimplex.hpp>
#include "realpaver/lp_model.hpp"

namespace realpaver {

/**
 * @brief This is a wrapper class for the LP solver Clp.
 * 
 * 
 */
class LPSolver : public LPModel {
public:
   LPSolver();
   ~LPSolver();

   /**
    * @brief Optimization method
    * 
    * A Clp model is created first from the LP model. Then the optimization
    * method is executed.
    * 
    * @return true if an optimal solution is found
    */
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
