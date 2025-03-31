#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
   string filename = "Yamamura-M-";

   if (argc != 2)
   {
      cout << "ERREUR" << endl;
      return 1;
   }

   int n = stoi(argv[1]);

   filename += string(argv[1]) + string(".rp");

   ofstream of;

   of.open(filename);

   of << "# In: Yamamura K., Kawata H., and Tokue A." << endl;
   of << "#     Interval solution of nonlinear equations using linear programming."
      << endl;
   of << "#     BIT, 38(1):186-199, 1998." << endl << endl;

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
   for (int i = 1; i <= n; ++i)
   {
      of << "2.5*x" << i << "^3-10.5*x" << i << "^2+11.8*x" << i;
      of << "+v == " << i;
      of << "," << endl;
   }

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
