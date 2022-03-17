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
      string filename = "model_test";

      Problem problem;
      Parser parser;

      bool res = parser.parseFile(filename, problem);

      if (!res)
         cout << "Parse error: " << parser.getParseError() << endl;
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
