#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/RealVector.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::internal, "work.log");
   Interval::precision( 16 );

   try {

      RealVector x = {1.0, 2.0, 3.5};
      cout << x << endl;

      RealVector y = {1.5, 1.0, 0.5};
      cout << y << endl;

      cout << x - y << endl;

      cout << x.scalarProduct(y) << endl;
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }


   return 0;
}
