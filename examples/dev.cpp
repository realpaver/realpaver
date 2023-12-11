#include <iostream>
#include "realpaver/Exception.hpp"
#include "realpaver/DomainSlicer.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   try
   {
      RangeUnionDomain u({Range(0, 1)});
      RangeUnionDomain* bdom = new RangeUnionDomain(u);

      RangeUnionDomainBisecter slicer;

      size_t n = slicer.apply(bdom);
      cout << n << endl;

      DomainSlicer::iterator it = slicer.begin();
      while (it != slicer.end())
      {
         Domain* dom = slicer.next(it);
         cout << (*dom) << endl;
         delete dom;
      }

      delete bdom;
   }
   catch(Exception e)
   {
      cout << e.message() << endl;
   }
   return 0;
}
