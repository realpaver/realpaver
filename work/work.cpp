#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/BOModel.hpp"

#include "realpaver/PolytopeHullContractor.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::full, "work.log");
   Interval::precision( 16 );
   
   try {
      Problem* prob = new Problem();
      Dag* dag = new Dag();
      Variable x = prob->addRealVar(-1.19, 1.19, "x"),
               y = prob->addRealVar(0.76, 1.42, "y");

      dag->insert( y - sqr(x) == 0 );
      dag->insert( sqr(x) + sqr(y) - 2.0 == 0 );
      cout << *dag << endl;

      IntervalRegion reg = prob->getDomains();
      cout << "reg : " << reg << endl;

      PolytopeHullContractor op(dag, dag->scope());

      Proof proof = op.contract(reg);
      cout << proof << endl << reg << endl;

/*
      dag->eval(reg);

      LPModel lpm;
      dag->linearize(lpm);

      cout << lpm << endl;
      */
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
