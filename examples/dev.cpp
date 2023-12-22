#include <iostream>
#include "realpaver/Contractor3B.hpp"
#include "realpaver/ContractorConstraint.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/DomainSlicerFactory.hpp"
#include "realpaver/Exception.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Tolerance.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   try
   {
      
      double lb = -1.90558487645,
             rb = -1.9064672114;

      cout << Double::isClose(lb, rb, 1.0e-4, 0.0) << endl;
      

/*
LOW.   Propagation test on x [0.001R]
LOW.     [-1.90670349983, -1.90558487645] -> [-1.90670349983, -1.9064672114] not reduced enough
LOW.   Propagation test on y [0.001R]
LOW.     [0.603723250967, 0.604468999883] -> [0.604311474273, 0.604468999883] not reduced enough
*/

/*
      
      
      Problem P;
      Variable x = P.addRealVar(-1, 2, "x"),
               y = P.addRealVar(0, 6, "y"),
               n = P.addIntVar(1, 7, "n"),
               b = P.addBinaryVar("b");

      std::unique_ptr<DomainSlicerMap>
         map = DomainSlicerFactory::makeBisectionStrategy(P.scope());   

      DomainBox reg(P.scope());
      cout << reg << endl;

      Variable vsel = b;
      cout << "split var: " << vsel.getName() << endl;

      DomainSlicer* slicer = map->getSlicer(vsel);

      Domain* dom = reg.get(vsel);

      size_t ns = slicer->apply(dom);
      cout << "nb slices: " << ns << endl;

      DomainSlicer::iterator it = slicer->begin();
      while (it != slicer->end())
      {
         std::unique_ptr<Domain> slice = slicer->next(it);
         cout << "a slice: " << (*slice) << endl;
      }
*/
   }
   catch(Exception e)
   {
      cout << e.message() << endl;
   }
   return 0;
}
