#include <iostream>
using namespace std;

#include "realpaver_api.hpp"
using namespace realpaver;

int main()
{
   try
   {
      Params params;   // Default parameters
      Problem problem; // Empty problem
      std::string rp_beam_str = R"RP(
        Constants
          L = 4, # Length
          P = 1e4; # Force
        Variables
          H in [0.1,0.8], # Height
          W in  [0.1,0.6], # Width
          T_f in [0.01,0.05], # Flange Thickness
          T_w in [0.01,0.05], # Web Thickness
          w_max in [0,5],  # Maximal displacement
          sigma_max in [0,1e8], # Density
          E in [0,1e15], # Maximal stress
          rho in [0,1e5]; # Elastic module
        Aliases
          A = H*W - (H-2*T_w)*(W-T_f), # Area
          M = L * A * rho, # Mass
          Y = H/2,
          I_x = ((H-2*T_w)^3*T_f + 2*T_w^3*W + 6*T_w*W*(H-T_w)^2)/12,
          I_y = (2*T_w*W^3 + (H - 2*T_w)*T_f^3)/12,
          I = I_x + I_y; # Momentum of inertia
        Minimize w_max;
        Constraints
          w_max == P*L^3/(48*E*I_x),
          sigma_max == P*Y / (4*I_x),
          sigma_max <= 12.8e6,
          table({rho,E},{
            7800, 2.0e11, # Steel
            2700, 6.7e10, # Aluminium
            4400, 1.1e11, # Titanium
            8800, 1.06e11 # Bronze
          });
         )RP";
      Parser parser;
      parser.parseStr(rp_beam_str, problem);

      Optimizer optimizer(problem);
      optimizer.getEnv()->setParams(params);
      optimizer.optimize();
      cout << "Otpimization status: " << optimizer.getStatus() << endl;
      if (optimizer.isSolvedOptimal())
      {
         cout << "Optimal solution:" << optimizer.bestFeasible() << endl;
         cout << "Optimal value enclosure:" << optimizer.getOptimum() << endl;
      }
      cout << "Number of nodes: " << optimizer.getNbNodes() << endl;
      cout << "Time used: " << optimizer.getSolvingTime() << endl;
   }
   catch (Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
