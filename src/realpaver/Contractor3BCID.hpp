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

#include "realpaver/Contractor3B.hpp"
#include "realpaver/ContractorCID.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor that combines a 3B contractor and a CID contractor.
///
/// Given a variable and a box B, the 3B contractor is applied. If the domain
/// of v in B is not reduced then B is returned. Oherwise, the CID
/// contractor is applied.
///////////////////////////////////////////////////////////////////////////////
class Contractor3BCID : public Contractor {
public:
   /// Creates a contractor
   /// @param op a contractor
   /// @param v a variable
   /// @param slicer3B a slicer for the 3B contractor
   /// @param slicerCID  a slicer for the CID contractor
   Contractor3BCID(SharedContractor op, Variable v,
                   std::unique_ptr<IntervalSlicer> slicer3B,
                   std::unique_ptr<IntervalSlicer> slicerCID);

   /// Creates a contractor
   /// @param op a contractor
   /// @param v a variable
   /// @param n3B number of slices for the 3B contractor (at least 2)
   /// @param nCID  number of slices for the CID contractor (at least 2)
   Contractor3BCID(SharedContractor op, Variable v, size_t n3B, size_t nCID);

   /// Destructor
   ~Contractor3BCID();

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
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   Variable v_;
   Contractor3B* ctc3B_;
   ContractorCID* ctcCID_;
};


} // namespace

#endif
