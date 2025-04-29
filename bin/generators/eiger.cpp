#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
   string filename = "Eiger-";

   if (argc != 2)
   {
      cout << "ERREUR" << endl;
      return 1;
   }

   int n = stoi(argv[1]);

   filename += string(argv[1]) + string(".rp");

   ofstream of;

   of.open(filename);

   of << "# In: Kearfott R.B." << endl;
   of << "#     Some tests of generalized bisection." << endl;
   of << "#     ACM Trans. on Mathematical Software, 13(3):197-220, 1987." << endl
      << endl;

   //////////
   of << "Variables" << endl;
   for (int i = 1; i <= n; ++i)
   {
      of << "x" << i << " in [-1e8, 1e8]";
      if (i < n)
         of << "," << endl;
   }
   of << ";" << endl << endl;

   //////////
   string c = "1.1";
   of << "Constraints" << endl;
   for (int i = 1; i < n; ++i)
   {
      of << "(x" << i << "-0.01)^2+x" << i + 1 << " == 0," << endl;
   }
   of << "(x" << n << "-0.1)+x1-0.1 == 0;" << endl;

   of.close();

   return 0;
}
