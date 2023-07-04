#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/NLPSolver.hpp"
#include "realpaver/Param.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   try {
      Problem problem;
      Variable x = problem.addRealVar(0, 10, "x"),
               y = problem.addRealVar(0, 10, "y");
      
      std::cout<<"Variables created"<<std::endl;

      RealFunction f(sqr(x) + sqr(y));

      std::cout<<"Objective function created"<<std::endl;

      RealFunctionVector G( {y - x + 1, y - x - 2},
                            {Interval::negative(), Interval::positive()} );

      std::cout<<"Constraints functions created"<<std::endl;

      NLPSolver optimizer(f, G);

      std::cout<<"Solver created!"<<std::endl;
   }
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
}
