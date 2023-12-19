///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_3BCID_HPP
#define REALPAVER_CONTRACTOR_3BCID_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/IntervalSlicer.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor that combines Constructive Interval Disjunction with
/// 3B consistency.
///
/// Given a variable, a box B, and a contractor op, a shaving process tries
/// to remove slices of the domain of v in B (3B). When no slice can be removed
/// then the CID contractor is applied.
///////////////////////////////////////////////////////////////////////////////
class Contractor3BCID : public Contractor {
public:
   /// Creates a contractor
   /// @param op a contractor
   /// @param v a variable
   /// @param slicer3B a slicer for 3B consistency
   /// @param slicerCID 
   Contractor3BCID(SharedContractor op, Variable v,
                   std::unique_ptr<IntervalSlicer> slicer3B,
                   std::unique_ptr<IntervalSlicer> slicerCID);

   /// Creates a contractor without any variable (to be fixed later)
   /// @param op a contractor
   /// @param slicer3B a slicer for 3B consistency
   /// @param slicerCID 
   Contractor3BCID(SharedContractor op,
                   std::unique_ptr<IntervalSlicer> slicer3B,
                   std::unique_ptr<IntervalSlicer> slicerCID);

   /// Default destructor
   ~Contractor3BCID() = default;

   /// No copy
   Contractor3BCID(const Contractor3BCID&) = delete;

   /// No assignment
   Contractor3BCID& operator=(const Contractor3BCID&) = delete;

   /// @return the variable whose domain is sliced
   Variable getVar() const;

   /// Assigns the variable whose domain is sliced
   /// @param v a variable that belongs to the scope of this
   void setVar(Variable v);

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& box) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   SharedContractor op_;
   Variable v_;
   std::unique_ptr<IntervalSlicer> slicer3B_;
   std::unique_ptr<IntervalSlicer> slicerCID_;
};


} // namespace

#endif
