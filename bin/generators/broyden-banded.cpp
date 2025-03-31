#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
   string filename = "BroydenBanded-";

   if (argc != 2)
   {
      cout << "ERREUR" << endl;
      return 1;
   }

   int n = stoi(argv[1]);

   filename += string(argv[1]) + string(".rp");

   ofstream of;

   of.open(filename);

   of << "# In: Moré JJ, Garbow BS, Hillstrom KE." << endl;
   of << "#     Testing unconstrained optimization software." << endl;
   of << "#     ACM Trans Math Software 1981, 7(1):17–41." << endl << endl;

   //////////
   of << "Variables" << endl;
   for (int i = 1; i <= n; ++i)
   {
      of << "x" << i << " in [-1e2, 1e2]";
      if (i < n)
         of << "," << endl;
   }
   of << ";" << endl << endl;

   //////////
   of << "Constraints" << endl;
   for (int i = 1; i <= n; ++i)
   {
      of << "x" << i << "*(2+5*x" << i << "^2)+1-(";

      int p = std::max(1, i - 5), q = std::min(n, i + 1);
      bool plus = false;
      for (int j = p; j <= q; ++j)
      {
         if (i != j)
         {
            if (plus)
               of << "+";
            of << "x" << j << "*(1+x" << j << ")";
            plus = true;
         }
      }
      of << ") == 0";
      if (i < n)
         of << "," << endl;
   }
   of << ";" << endl;

   of.close();

   return 0;
}
