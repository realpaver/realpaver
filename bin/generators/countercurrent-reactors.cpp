#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
   string filename = "CountercurrentReactors-";

   if (argc != 2)
   {
      cout << "ERREUR" << endl;
      return 1;
   }

   int n = stoi(argv[1]);

   filename += string(argv[1]) + string(".rp");

   ofstream of;

   of.open(filename);

   of << "# In: Luksan L. and Vleck J." << endl;
   of << "#     Sparse and partially separable test problems for unconstrained" << endl;
   of << "#     and equality constrained optimization." << endl;
   of << "#     Research Report 767, Institute of Computer Science, Academy of " << endl;
   of << "#     Sciences of the Czeck Republic, Janvier 1999" << endl << endl;

   //////////
   of << "Constants" << endl;
   of << "a = 0.5," << endl;
   of << "b = 1-a," << endl;
   of << "c = 2-a;" << endl << endl;

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
   for (int i = 1; i <= n; ++i)
   {
      if (i == 1)
      {
         of << "a-b*x" << i + 2 << "-x" << i << "*(1+4*x" << i + 1 << ")";
      }
      else if (i == 2)
      {
         of << "-c*x" << i + 2 << "-x" << i << "*(1+4*x" << i - 1 << ")";
      }
      else if (i == n)
      {
         of << "a*x" << i - 2 << "-c-x" << i << "*(1+4*x" << i - 1 << ")";
      }
      else if (i == n - 1)
      {
         of << "a*x" << i - 2 << "-x" << i << "*(1+4*x" << i + 1 << ")";
      }
      else if (i % 2 == 0)
      {
         of << "a*x" << i - 2 << "-c*x" << i + 2 << "-x" << i << "*(1+4*x" << i - 1
            << ")";
      }
      else
      {
         of << "a*x" << i - 2 << "-b*x" << i + 2 << "-x" << i << "*(1+4*x" << i + 1
            << ")";
      }
      of << " == 0";
      if (i < n)
         of << "," << endl;
   }
   of << ";" << endl;

   of.close();

   return 0;
}
