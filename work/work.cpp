#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::internal, "work.log");
   Interval::precision( 16 );

   try {
      string filename = "";

      Problem problem;
      Parser parser;
      
      bool res = parser.parseFile(filename, problem);
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }


   return 0;
}
