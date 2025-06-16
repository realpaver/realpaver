#include "realpaver_api.hpp"
#include <fstream>
#include <iostream>

using namespace realpaver;
using namespace std;

int main(int argc, char **argv)
{
   try
   {
      if (argc < 2)
         THROW("A file name is required on the command line");

      string filename(argv[1]);
      ifstream infile(filename);

      if (!infile.is_open())
         THROW("File not found");
      else
         infile.close();

      Params params;
      Parser parser(params);
      Problem problem;

      bool ok = parser.parseFile(filename, problem);
      if (!ok)
         THROW("Parse error: " << parser.getParseError());

      cout << problem << endl;
   }

   catch (Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
