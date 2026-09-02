#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
   string filename = "Noon";

   if (argc != 2)
   {
      cout << "ERREUR" << endl;
      return 1;
   }

   int n = stoi(argv[1]);

   filename += string(argv[1]) + string("-M.rp");

   ofstream of;

   of.open(filename);

   of << "# In: V. W. Noonburg." << endl;
   of << "#     A neural network modeled by an adaptive Lotka-Volterra system." << endl;
   of << "#     SIAM J. Appl. Math (1988)." << endl << endl;

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
   string c = "1.1";
   of << "Constraints" << endl;
   for (int i = 1; i <= n; ++i)
   {
      of << "x" << i << "*(";
      bool plus = false;
      for (int j = 1; j <= n; ++j)
      {
         if (j != i)
         {
            if (plus)
               of << "+";
            of << "x" << j << "^2";
            plus = true;
         }
      }
      of << "-" << c << ")" << "+1 == 0";
      if (i < n)
         of << "," << endl;
   }
   of << ";" << endl;

   of.close();

   return 0;
}
