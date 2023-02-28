///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_NEWTON_HPP
#define REALPAVER_INTERVAL_NEWTON_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/IntervalFunctionVector.hpp"

namespace realpaver {

class IntervalNewton : public Contractor {
public:
   IntervalNewton(IntervalFunctionVector F);

   ///@{
   Scope scope() const override;
   Proof contract(IntervalRegion& reg) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   IntervalFunctionVector F_;
   IntervalMatrix jac_;
   IntervalVector y_, b_;
};

} // namespace

#endif
