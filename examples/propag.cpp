#include "realpaver_api.hpp"
#include <iostream>

using namespace realpaver;
using namespace std;

int main(void)
{
   Logger::init(LogLevel::full, "propag.log");

   try
   {
      Problem problem;

      Variable x = problem.addRealVar(-7, 3, "x"), y = problem.addRealVar(-3, 6, "y");
      size_t i1 = problem.addCtr(y - sqr(x) == 0),
             i2 = problem.addCtr(sqr(x) + sqr(y) - 2.0 == 0);

      DomainBox dbox(problem.scope());
      cout << "Box:    " << dbox << endl;

      IntervalBox B(dbox);
      cout << "Interval box:     " << B << endl;

      SharedContractor ctc1 = std::make_shared<ContractorHC4Revise>(problem.ctrAt(i1)),
                       ctc2 = std::make_shared<ContractorHC4Revise>(problem.ctrAt(i2));

      ContractorHC4 tor;
      tor.setTol(1.0e-4);
      tor.push(ctc1);
      tor.push(ctc2);

      Proof proof = tor.contract(B);

      cout << "Proof:      " << proof << endl;

      if (proof != Proof::Empty)
         cout << "New box: " << B << endl;
   }
   catch (Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
