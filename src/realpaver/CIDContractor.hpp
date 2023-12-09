///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
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
/// Given a box B, a variable v, a slicer and a contractor, the domain of
/// v in B is divided by the slicer, each slice is reduced by the contractor,
/// and the hull of the contracted slices is returned.
///
/// The scope of this corresponds to the scope of the given contractor.
///////////////////////////////////////////////////////////////////////////////
class CIDContractor : public Contractor {
public:
   /// Creates a contractor
   /// @param op a contractor
   /// @param v a variable
   /// @param a slicer
   CIDContractor(SharedContractor op, Variable v,
                 std::unique_ptr<IntervalSlicer> slicer);

   /// Creates a contractor without any variable (to be fixed later)
   /// @param op a contractor
   /// @param a slicer
   CIDContractor(SharedContractor op,
                 std::unique_ptr<IntervalSlicer> slicer);

   /// Destructor
   ~CIDContractor();

   /// No copy
   CIDContractor(const CIDContractor&) = delete;

   /// No assignment
   CIDContractor& operator=(const CIDContractor&) = delete;

   /// @return the variable whose domain is sliced
   Variable getVar() const;

   /// Assigns the variable whose domain is sliced
   /// @param v a variable that belongs to the scope of this
   void setVar(Variable v);

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   SharedContractor op_;
   Variable v_;
   std::unique_ptr<IntervalSlicer> slicer_;
};

} // namespace

#endif
