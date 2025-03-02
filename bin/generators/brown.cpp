#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char ** argv)
{
   string filename = "Brown-";

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
   for (int i=1; i<=n; ++i)
   {
      of << "x" << i << " in [-1e8, 1e8]";
      if (i<n) of << "," << endl;
   }
   of << ";" << endl << endl;

   //////////
   string c = "1.1";
   of << "Constraints" << endl;
   for (int i=1; i<n; ++i)
   {
      of << "2*x" << i;
      for (int j=1; j<=n; ++j)
      {
         if (i != j)
         {
           of << "+" << "x" << j;
         }
      }
      of << " == " << n+1 << "," << endl;
   }

   for (int i=1; i<n; ++i)
   {
      of << "x" << i << "*";
   }
   of << "x" << n << " == 1;" << endl;

   of.close();

   return 0;
}
