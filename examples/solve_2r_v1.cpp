#include <iostream>
using namespace std;

#include "realpaver_api.hpp"
using namespace realpaver;

int main()
{
   try
   {
      Params params;
      params.loadParam("../../examples/param.txt");

      Problem problem;

      Parser parser(params);
      bool ok = parser.parseFile("../../examples/2r-robot.rp", problem);

      if (ok)
      {
         CSPSolver solver(problem);
         solver.getEnv()->setParams(params);
         solver.solve();
         for (size_t i = 0; i < solver.nbSolutions(); ++i)
         {
            pair<DomainBox, Proof> sol = solver.getSolution(i);
            cout << "SOL " << i << ": " << sol.first << " (" << sol.second << ")" << endl;
         }
         cout << solver.nbSolutions() << " solutions found in " << solver.getSolvingTime()
              << " s, using " << solver.getTotalNodes() << " nodes." << endl;
      }
   }
   catch (Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
