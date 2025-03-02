#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char ** argv)
{
   string filename = "Trigexp1-";

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
   of << "Variables" << endl;
   for (int i=1; i<=n; ++i)
   {
      of << "x" << i << " in [-1e2, 1e2]";
      if (i<n) of << "," << endl;
   }
   of << ";" << endl << endl;

   //////////
   of << "Constraints" << endl;
   of << "3*x1^3+2*x2-5+sin(x1-x2-2)*sin(x1+x2) == 0," << endl;
   for (int i=2; i<n; ++i)
   {
      of << "3*x" << i << "^3+2*x" << i+1 << "-5+sin(x" << i << "-x" << i+1;
      of << ")*sin(x" << i << "+x" << i+1 << ")-4*x" << i;
      of << "-x" << i-1 << "*exp(x" << i-1 << "-x" << i << ")-3 == 0," << endl;
   }
   of << "4*x" << n << "-x" << n-1 << "*exp(x" << n-1 << "-x" << n << ")-3 == 0;" << endl;

   of.close();

   return 0;
}
