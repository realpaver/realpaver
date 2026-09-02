#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
   string filename = "Bratu-";

   if (argc != 2)
   {
      cout << "ERREUR" << endl;
      return 1;
   }

   int n = stoi(argv[1]);

   filename += string(argv[1]) + string(".rp");

   ofstream of;

   of.open(filename);

   //////////
   of << "Constants" << endl;
   of << "h = 1/(" << n + 1 << ")^2," << endl;
   of << "x0 = 0," << endl;
   of << "x" << n + 1 << "= 0;" << endl << endl;

   //////////
   of << "Variables" << endl;
   for (int i = 1; i <= n; ++i)
   {
      of << "x" << i << " in [-1e8, 20]";
      if (i < n)
         of << "," << endl;
   }
   of << ";" << endl << endl;

   //////////
   of << "Constraints" << endl;
   for (int i = 1; i <= n; ++i)
   {
      of << "x" << i - 1 << "-2*x" << i << "+x" << i + 1;
      of << "+h*exp(x" << i << ") == 0";
      if (i < n)
         of << "," << endl;
   }
   of << ";" << endl;

   of.close();

   return 0;
}
