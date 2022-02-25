///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CID_CONTRACTOR_HPP
#define REALPAVER_CID_CONTRACTOR_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/IntervalSlicer.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor implementing Constructive Interval Disjunction.
///
/// Given a region R, a variable v, a slicer and a contractor, the domain of
/// v in R is divided by the slicer, each slice is reduced by the contractor,
/// and the hull of the contracted slices is returned.
///////////////////////////////////////////////////////////////////////////////
class CidContractor : public Contractor {
public:
   /// Creates a contractor
   /// @param op a contractor
   /// @param v a variable
   /// @param a slicer
   ///
   /// This owns the slicer, hence the slicer is destroyed with this.
   CidContractor(const SharedContractor& op, const Variable& v,
                 IntervalSlicer* slicer);

   /// Destructor
   ~CidContractor();

   /// No copy
   CidContractor(const CidContractor&) = delete;

   /// No assignment
   CidContractor& operator=(const CidContractor&) = delete;

   ///@{
   /// Overrides the methods of Contractor
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalVector& X);
   void print(std::ostream& os) const;
   ///@}

private:
   SharedContractor op_;
   Variable v_;
   IntervalSlicer* slicer_;
   Bitset bs_;
};

} // namespace

#endif
