#include "realpaver/AffineForm.hpp"
#include "realpaver_api.hpp"
#include <iomanip>
#include <iostream>

using namespace realpaver;
using namespace std;

int main(void)
{
   Logger::init(LogLevel::full, "newton.log");

   Interval alpha, dzeta, delta, x("0.2", "1.2");

   std::cerr << std::setprecision(16) << std::endl;
   AffineForm::chebyshevCos(x, alpha, dzeta, delta);

   return 0;
}

// [1, 10]
// [1, 4.087500000000001]
// [1, 1.996879778287463]
// [1.375778838455192, 1.437013531872623]
// [1.414068266834451, 1.414409744371276]
// [1.414213559529903, 1.414213565673288]
// Proof: feasible
