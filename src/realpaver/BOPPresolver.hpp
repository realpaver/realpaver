///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BOP_PRESOLVER_HPP
#define REALPAVER_BOP_PRESOLVER_HPP

#include "realpaver/BOPModel.hpp"
#include "realpaver/propagator.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This prevolves a Bound Optimization Problem.
///
/// It enforces a propagation step on the set of equations df / dxi = 0
/// using BOP contractors. It leads to possibly instanciate some variables
/// xi by proving that df / dxi does not vanish in the initial region.
///////////////////////////////////////////////////////////////////////////////
class BOPPresolver {
public:
   /// Creates a presolver from a moddel
   BOPPresolver(BOPModel& model);

   /// No copy
   BOPPresolver(const BOPPresolver&) = delete;

   /// No assignment
   BOPPresolver& operator=(const BOPPresolver&) = delete;

   /// Default destructor
   ~BOPPresolver() = default;

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
