#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char ** argv)
{
   string filename = "Troesch-";

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
   of << "h = 1/(" << n+1 << ")," << endl;
   of << "R = 10," << endl;
   of << "a = R*h^2;" << endl << endl;

   //////////
   of << "Variables" << endl;
   for (int i=1; i<=n; ++i)
   {
      of << "x" << i << " in [-10, 10]";
      if (i<n) of << "," << endl;
   }
   of << ";" << endl << endl;

   //////////
   of << "Constraints" << endl;

   of << "2*x1+a*sinh(R*x1)-x2 == 0," << endl;
   for (int i=2; i<n; ++i)
   {
      of << "2*x" << i << "+a*sinh(R*x" << i << ")-x" << i-1
         << "-x" << i+1 << " == 0," << endl;
   }
   of << "2*x" << n << "+a*sinh(R*x" << n << ")-x" << n-1 << " == 0;" << endl;

   of.close();

   return 0;
}
