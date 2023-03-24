#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
   if (argc != 2) return 1;

   string sdim(argv[1]);
   int dim = stoi(sdim);
   
   string filename = "BroydenTridiagonal";
   filename += sdim;
   filename += ".rp";

   ofstream of;
   of.open(filename, ofstream::out);

   of << "# Broyden tridiagonal function" << endl;
   of << "# In: Moré JJ, Garbow BS, Hillstrom KE." << endl;
   of << "#     Testing unconstrained optimization software." << endl;
   of << "#     ACM Trans Math Software 1981;7(1):17–41." << endl << endl;

   of << "Variables" << endl;
   for (int i=1; i<dim; ++i)
   {
      of << "   x" << i << " in [-10, 10]," << endl;
   }
   of << "   x" << dim << " in [-10, 10];" << endl;

   of << endl << "Constraints" << endl;
   for (int i=1; i<=dim; ++i)
   {
//      of << "(3-2*x" << i << ")*x" << i;
      of << "-2*((x" << i << "-3/4)^2 - 9/16)";
      if (i>1) of << "-x" << i-1;
      if (i<dim) of << "-2*x" << i+1;
      of << "+1 == 0";
      of << ((i<dim) ? "," : ";");
      of << endl;
   }

   of.close();
   
   return 0;
}
