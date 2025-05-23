#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
   string filename = "Brown-M-";

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
      of << "x" << i << " in [-1e8, 1e8]";
      of << "," << endl;
   }
   of << "v in [-1e8, 1e8];" << endl << endl;

   //////////

   of << "Constraints" << endl;
   for (int i = 1; i < n; ++i)
   {
      of << "x" << i << "+v == " << n + 1 << "," << endl;
   }

   for (int i = 1; i < n; ++i)
   {
      of << "x" << i << "*";
   }
   of << "x" << n << " == 1," << endl;

   of << "v == ";

   for (int i = 1; i <= n; ++i)
   {
      of << "x" << i;
      if (i < n)
         of << "+";
   }
   of << ";" << endl;

   of.close();

   return 0;
}
