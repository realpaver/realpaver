#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char ** argv)
{
   string filename = "DiscreteIntegral-";

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
   of << "Constants" << endl;
   of << "h = 1/" << n+1 << "," << endl;
   for (int i=1; i<=n; ++i)
   {
      of << "t" << i << " = " << i << "*h," << endl;
      of << "u" << i << " = 1-t" << i;
      if (i<n) of << "," << endl;
   }
   of << ";" << endl << endl;

   //////////
   of << "Variables" << endl;
   for (int i=1; i<=n; ++i)
   {
      of << "x" << i << " in [-1e2, 1e2]";
      if (i<n) of << "," << endl;
   }
   of << ";" << endl << endl;

   //////////
   string c = "1.1";
   of << "Constraints" << endl;
   for (int i=1; i<=n; ++i)
   {
      of << "x" << i << "+h*(";
      for (int j=1; j<=i; ++j)
      {
         if (j > 1) of << "+";
         of << "t" << j << "*(";
         of << "x" << j << "+t" << j << "+1";
         of << ")^3";
      }
      for (int j=i+1; j<=n; ++j)
      {
         of << "+";
         of << "u" << j << "*(";
         of << "x" << j << "+t" << j << "+1";
         of << ")^3";
      }
      of << ") == 0";
      if (i<n) of << "," << endl;
   }
   of << ";" << endl;

   of.close();

   return 0;
}
