#include <iostream>
#include "realpaver/DomainSlicerFactory.hpp"
#include "realpaver/Exception.hpp"
#include "realpaver/Problem.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   try
   {
      Problem P;
      Variable x = P.addRealVar(-1, 2, "x"),
               y = P.addRealVar(0, 6, "y"),
               n = P.addIntVar(1, 7, "n"),
               b = P.addBinaryVar("b");

      std::unique_ptr<DomainSlicerMap>
         map = DomainSlicerFactory::makeBisectionStrategy(P.scope());   

      SearchRegion reg(P.scope());
      cout << reg << endl;

      Variable vsel = b;
      cout << "split var: " << vsel.getName() << endl;

      DomainSlicer* slicer = map->getSlicer(vsel);

      Domain* dom = reg.getDomain(vsel);

      size_t ns = slicer->apply(dom);
      cout << "nb slices: " << ns << endl;

      DomainSlicer::iterator it = slicer->begin();
      while (it != slicer->end())
      {
         Domain* slice = slicer->next(it);
         cout << "a slice: " << (*slice) << endl;
         delete slice;
      }

   }
   catch(Exception e)
   {
      cout << e.message() << endl;
   }
   return 0;
}
