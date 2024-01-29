///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_PROPAGATOR_HPP
#define REALPAVER_NCSP_PROPAGATOR_HPP

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/NcspContext.hpp"
#include "realpaver/NcspNode.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of propagators for NCSPs.
///////////////////////////////////////////////////////////////////////////////
class NcspPropagator {
public:
   /// Default constructor
   NcspPropagator() = default;

   /// Default copy constructor
   NcspPropagator(const NcspPropagator&) = default;

   /// No assignment
   NcspPropagator& operator=(const NcspPropagator&) = delete;

   /// Virtual destructor
   virtual ~NcspPropagator();

   /// Contraction method
   /// @param a node whose box is contracted
   /// @param ctx a solving context
   /// @return a certificate of proof
   virtual Proof contract(NcspNode& node, NcspContext& ctx) = 0;

   /// Intersection with assignment between boxes
   /// @param B a reduced box
   /// @param box a box to be reduced as B inter box
   static Proof contractBox(const IntervalBox& B, DomainBox& box);
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs based on HC4 contractors.
///////////////////////////////////////////////////////////////////////////////
class NcspHC4 : public NcspPropagator {
public:
   /// @param facto a factory
   NcspHC4(ContractorFactory& facto);

   Proof contract(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;   // HC4
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs based on HC4 contractors and the interval
/// Newton operator.
///
/// Interval Newton is used only for square systems of equations.
///////////////////////////////////////////////////////////////////////////////
class NcspHC4Newton : public NcspPropagator {
public:
   /// @param facto a factory
   NcspHC4Newton(ContractorFactory& facto);

   Proof contract(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;   // HC4 + Newton
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs that implements the ACID strategy
/// based on HC4 contractors.
///////////////////////////////////////////////////////////////////////////////
class NcspACID : public NcspPropagator {
public:
   /// @param facto a factory
   NcspACID(ContractorFactory& facto);

   Proof contract(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor hc4_;        // HC4
   SharedContractorACID acid_;   // ACID
};

} // namespace

#endif
