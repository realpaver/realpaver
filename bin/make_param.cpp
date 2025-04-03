#include "realpaver/Param.hpp"
#include "realpaver/config.hpp"
#include <fstream>
#include <iostream>

using namespace realpaver;
using namespace std;

int main(int argc, char **argv)
{
   cout << REALPAVER_STRING << " - Create a parameter file" << std::endl;
   if (argc != 2)
   {
      cerr << "Bad number of arguments on the command line" << std::endl;
      return 1;
   }

   ofstream of;
   of.open(argv[1], std::ofstream::out);
   if (of.bad())
   {
      cerr << "Open error of parameter file" << std::endl;
      return 1;
   }
   else
   {
      of << "# Parameter file of " << REALPAVER_STRING << std::endl
         << "# This file is generated automatically." << std::endl
         << std::endl;
      Params::Print(of);
      of.close();
      cout << "File " << argv[1] << " created!" << std::endl;
      return 0;
   }
}
