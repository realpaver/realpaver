///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BO_PRESOLVER_HPP
#define REALPAVER_BO_PRESOLVER_HPP

#include "realpaver/BOModel.hpp"
#include "realpaver/Propagator.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This prevolves a Bound Optimization Problem.
///
/// It enforces a propagation step on the set of equations df / dxi = 0
/// using BO contractors. It leads to possibly instanciate some variables
/// xi by proving that df / dxi does not vanish in the initial region.
///////////////////////////////////////////////////////////////////////////////
class BOPresolver {
public:
   /// Creates a presolver from a moddel
   BOPresolver(BOModel& model);

   /// No copy
   BOPresolver(const BOPresolver&) = delete;

   /// No assignment
   BOPresolver& operator=(const BOPresolver&) = delete;

   /// Default destructor
   ~BOPresolver() = default;

   /// Presolves this using constraint propagation
   /// @return false if it is proved that there is no solution
   ///
   /// Use getContractedRegion() to get the contracted region
   bool presolve();

   /// @return the contracted region after presolving this
   IntervalVector getContractedRegion() const;

private:
   ContractorVector pool_;       // pool of contractors
   Propagator propagator_;       // propagator based on the pool of contractors
   SharedIntervalVector init_;   // initial region
   IntervalVector region_;       // contracted region
};

} // namespace

#endif
