#include <iostream>
#include "realpaver/Contractor3B.hpp"
#include "realpaver/ConstraintContractor.hpp"
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

      Problem P;
      Variable x = P.addRealVar(0, 10, "x");
      Constraint c( in(x, Interval(0.5, 1)) );

      Scope scop( {x} );
      IntervalBox B(scop);
      cout << B << endl;

      shared_ptr<Contractor> ctc = make_shared<ConstraintContractor>(c);
      unique_ptr<IntervalSlicer> slicer = make_unique<IntervalPartitionMaker>(10);

      Contractor3B ctc3b(ctc, x, std::move(slicer));

      Proof proof = ctc3b.contract(B);
      cout << proof << endl;
      cout << B << endl;

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
