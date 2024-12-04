#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "realpaver/Variable.hpp"

using namespace std;



int main(int argc, char ** argv)
{
   if (argc < 2) return 1;

   std::string str(argv[1]);
   int n = stoi(str);

   std::string name = "Katsura" + str;

   cout << "Generator of problem " << name << endl;

   vector<realpaver::Variable> vars(n);


   return 0;
}
   
   
//~ void u(ofstream& of, int k, int n)
//~ {
   //~ if (k > n || k < -n)
      //~ of << 0;
   //~ else
   //~ {
      //~ of << "u";
      //~ if (k < 0)
         //~ of << -k;
      //~ else
         //~ of << k;
      //~ }
//~ }

//~ void prod(ofstream& of, int k, int l, int n)
//~ {
   //~ if (abs(k) == abs(l))
   //~ {
      //~ u(of, k, n);
      //~ of << "^2";
   //~ }
   //~ else
   //~ {
      //~ u(of, k, n);
      //~ of << "*";
      //~ u(of, l, n);
   //~ }
//~ }

//~ int main(int argc, char ** argv)
//~ {
   //~ if (argc < 2) return 1;

   //~ std::string str(argv[1]);
   //~ int n = stoi(str);

   //~ std::string name = "Katsura" + str;

   //~ cout << "Generator of problem " << name << endl;

   //~ ofstream of;
   //~ of.open(name + ".rpv", ios::out);

   //~ of << "# " << name << endl << endl;

   //~ of << "Variables" << endl;
   //~ for (int i=0; i<=n; ++i)
   //~ {
      //~ of << "  u" << i << " in [0, 1]";
      //~ if (i<n) of << ",";
      //~ of << endl;
   //~ }
   //~ of << ";" << endl << endl;


   //~ of << "Constraints" << endl;
   //~ for (int m=0; m<=n-1; ++m)
   //~ {
      //~ for (int p=-n; p<=n; ++p)
      //~ {
         //~ prod(of, p, m-p, n);
         //~ if (p<n) of << " + ";
      //~ }
      //~ of << " == ";
      //~ u(of, m, n);
      //~ of << ",";
      //~ of << endl;
   //~ }

   //~ for (int p=-n; p<=n; ++p)
   //~ {
      //~ u(of, p, n);
      //~ if (p<n) of << "+";
   //~ }
   //~ of << " == 1" << endl;
   //~ of << ";" << endl;

   //~ of.close();

   //~ return 0;
//~ }
