#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
   if (argc != 2) return 1;

   string sdim(argv[1]);
   int dim = stoi(sdim);
   
   string filename = "Trigo";
   filename += sdim;
   filename += ".rp";

   ofstream of;
   of.open(filename, ofstream::out);

   of << "# Trigonometric function" << endl;
   of << "# In: Moré JJ, Garbow BS, Hillstrom KE." << endl;
   of << "#     Testing unconstrained optimization software." << endl;
   of << "#     ACM Trans Math Software 1981;7(1):17–41." << endl << endl;

   of << "Variables" << endl;
   for (int i=1; i<dim; ++i)
   {
      of << "   x" << i << " in [0, PI]," << endl;
   }
   of << "   x" << dim << " in [0, PI];" << endl;

   of << endl << "Constraints" << endl;
   for (int i=1; i<=dim; ++i)
   {
      of << dim+i << "-(";
      for (int j=1; j<=dim; ++j)
      {
         if (j == i) continue;
         of << "cos(x" << j << ")";
         if ((i<dim && j<dim) || (i==dim && j<dim-1)) of << "+";
      }
      of << ")-";
      of << i+1 << "*cos(x" << i << ")-sin(x" << i << ") == 0";
      of << ((i<dim) ? "," : ";") << endl;
   }

   of.close();
   
   return 0;
}
