///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_MAX_CID_CONTRACTOR_HPP
#define REALPAVER_MAX_CID_CONTRACTOR_HPP

#include "realpaver/CIDContractor.hpp"
#include "realpaver/Selector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a CID contractor applied to the best variable identified by a
/// selection strategy.
///////////////////////////////////////////////////////////////////////////////
class MaxCIDContractor : public Contractor {
public:
   /// Creates a contractor
   /// @param op a contractor
   /// @param selector object used to select a variable
   /// @param slicer object used to slice a variable domain
   ///
   /// This owns the slicer and the selector, hence they are destroyed with
   /// this.
   MaxCIDContractor(const SharedContractor& op, Selector* selector,
                    IntervalSlicer* slicer);

   /// Destructor
   ~MaxCIDContractor();

   /// No copy
   MaxCIDContractor(const MaxCIDContractor&) = delete;

   /// No assignment
   MaxCIDContractor& operator=(const MaxCIDContractor&) = delete;

   ///@{
   /// Overrides (Contractor)
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalRegion& reg);
   void print(std::ostream& os) const;
   ///@}

private:
   CIDContractor* op_;
   Selector* selector_;
};

} // namespace

#endif
