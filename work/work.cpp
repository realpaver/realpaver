#include <signal.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "realpaver/Logger.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/Problem.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::full, "work.log");

   try {
      Problem P;
      Variable r = P.addRealVar(0, 1, "r");
      Variable t = P.addRealVar(Interval::zeroPi(), "t");

      r.setTolerance(Tolerance::makeAbs(0.05));
      t.setTolerance(Tolerance::makeAbs(0.05));

      Term tc = 1 + r*(cos(t) + r*cos(2*t));
      Term ts = sin(t) + r*sin(2*t);
      Term trdn = sqr(cos(t)) + sqr(sin(t));

      IntervalRegion X = P.getDomains();

      Proof certif = tc.contract(X, Interval::zero());
      cout << endl << "TC -> " << certif << endl;
      cout << X << endl;

      certif = ts.contract(X, Interval::zero());
      cout << endl << "TS -> " << certif << endl;
      cout << X << endl;

      certif = trdn.contract(X, Interval(1, 1));
      cout << endl << "TRDN -> " << certif << endl;
      cout << X << endl;

      certif = tc.contract(X, Interval::zero());
      cout << endl << "TC -> " << certif << endl;
      cout << X << endl;

      certif = ts.contract(X, Interval::zero());
      cout << endl << "TS -> " << certif << endl;
      cout << X << endl;

      certif = trdn.contract(X, Interval(1, 1));
      cout << endl << "TRDN -> " << certif << endl;
      cout << X << endl;

      certif = tc.contract(X, Interval::zero());
      cout << endl << "TC -> " << certif << endl;
      cout << X << endl;

      certif = ts.contract(X, Interval::zero());
      cout << endl << "TS -> " << certif << endl;
      cout << X << endl;

      certif = trdn.contract(X, Interval(1, 1));
      cout << endl << "TRDN -> " << certif << endl;
      cout << X << endl;

      certif = tc.contract(X, Interval::zero());
      cout << endl << "TC -> " << certif << endl;
      cout << X << endl;

      certif = ts.contract(X, Interval::zero());
      cout << endl << "TS -> " << certif << endl;
      cout << X << endl;

      certif = trdn.contract(X, Interval(1, 1));
      cout << endl << "TRDN -> " << certif << endl;
      cout << X << endl;

      certif = tc.contract(X, Interval::zero());
      cout << endl << "TC -> " << certif << endl;
      cout << X << endl;
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
