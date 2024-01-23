///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_PROPAGATOR_ACID_HPP
#define REALPAVER_NCSP_PROPAGATOR_ACID_HPP

#include "realpaver/ContractorCID.hpp"
#include "realpaver/IntervalSmearSumRel.hpp"
#include "realpaver/IntervalSlicer.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator that implements the adaptive CID strategy.
///////////////////////////////////////////////////////////////////////////////
class PropagatorACID : public Contractor {
public:
   /// Constructor
   /// @param ssr calculator of smear sum rel values
   /// @param op contractor of slices
   /// @param nbs number of slices of CID contractors
   PropagatorACID(std::shared_ptr<IntervalSmearSumRel> ssr,
                  SharedContractor op,
                  size_t nbs);

   /// Default copy constructor
   PropagatorACID(const PropagatorACID&) = default;

   /// No assignment
   PropagatorACID& operator=(const PropagatorACID&) = delete;

   /// Default destructor
   ~PropagatorACID() = default;

   /// @return the number of slices of CID contractors
   size_t nbSlices() const;

   /// Assigns the number of slices of CID contractors
   /// @param nbs an integer greater than 2
   void setNbSlices(size_t nbs);

   /// @return the number of CID contractors applied in the contraction method
   size_t nbCID() const;

   /// Assigns the number of CID contractors applied in the contraction method
   /// @param nbcid an integer between 1 and n where n is the number of
   ///        variables
   void setNbCID(size_t nbcid);

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   std::shared_ptr<IntervalSmearSumRel> ssr_;   // calculator of smear
                                                // sum rel values
   SharedContractor op_;   // contractor of slices
   size_t nbs_;            // number of slices of CID contractors
   size_t nbcid_;          // number of CID contractors applied
};

} // namespace

#endif
