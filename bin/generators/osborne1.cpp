#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
   string filename = "Osborne1.rp";
   int n = 5;
   int m = 33;
   string y[] = {"0.844", "0.908", "0.932", "0.936", "0.925", "0.908", "0.881",
                 "0.850", "0.818", "0.784", "0.751", "0.718", "0.685", "0.658",
                 "0.628", "0.603", "0.580", "0.558", "0.538", "0.522", "0.506",
                 "0.490", "0.478", "0.467", "0.457", "0.448", "0.438", "0.431",
                 "0.424", "0.420", "0.414", "0.411", "0.406"};

   ofstream of;
   of.open(filename);

   of << "# In: Moré JJ, Garbow BS, Hillstrom KE." << endl;
   of << "#     Testing unconstrained optimization software." << endl;
   of << "#     ACM Trans Math Software 1981, 7(1):17–41." << endl << endl;

   //////////
   of << "Constants" << endl;
   for (int i = 1; i <= m; ++i)
   {
      of << "t" << i << " = " << 10 * (i - 1);
      if (i < m)
         of << "," << endl;
   }
   of << ";" << endl << endl;

   //////////
   of << "Variables" << endl;
   for (int i = 1; i <= n; ++i)
   {
      of << "x" << i << " in [-1e0, 1e0]";
      if (i < n)
         of << "," << endl;
   }
   of << ";" << endl << endl;

   //////////
   of << "Constraints" << endl;
   for (int i = 1; i <= m; ++i)
   {
      of << "x1+x2*exp(-t" << i << "*x4)+x3*exp(-t" << i << "*x5) == " << y[i - 1];
      if (i < m)
         of << "," << endl;
   }
   of << ";" << endl;

   of.close();

   return 0;
}
