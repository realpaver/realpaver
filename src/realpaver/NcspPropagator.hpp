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

#include <vector>
#include "realpaver/ContractorFactory.hpp"
#include "realpaver/ContractorVar3BCID.hpp"
#include "realpaver/NcspContext.hpp"
#include "realpaver/NcspNode.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of propagators for NCSPs.
///////////////////////////////////////////////////////////////////////////////
class NcspPropagator {
public:
   /// Constructor
   NcspPropagator();

   /// Default copy constructor
   NcspPropagator(const NcspPropagator&) = default;

   /// No assignment
   NcspPropagator& operator=(const NcspPropagator&) = delete;

   /// Virtual destructor
   virtual ~NcspPropagator();

   /// Contraction method that applies first contractImpl and then
   /// the domain contractors for variables with disconnected domains
   /// @param a node whose box is contracted
   /// @param ctx a solving context
   /// @return a certificate of proof
   Proof contract(NcspNode& node, NcspContext& ctx);

   /// Intersection with assignment between boxes
   /// @param B a reduced box
   /// @param box a box to be reduced as B inter box
   static Proof contractBox(const IntervalBox& B, DomainBox& box);

protected:
   IntervalBox* B_;

   /// Contraction method to be overriden
   /// @param a node whose box is contracted
   /// @param ctx a solving context
   /// @return a certificate of proof
   virtual Proof contractImpl(NcspNode& node, NcspContext& ctx) = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs based on HC4 contractors.
///////////////////////////////////////////////////////////////////////////////
class NcspHC4 : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspHC4(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs based on BC4 contractors.
///////////////////////////////////////////////////////////////////////////////
class NcspBC4 : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspBC4(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs that applies the interval Newton operator.
///////////////////////////////////////////////////////////////////////////////
class NcspNewton : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspNewton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs that implements the ACID strategy
/// based on HC4 contractors.
///////////////////////////////////////////////////////////////////////////////
class NcspACID : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspACID(ContractorFactory& facto);

   /// Destructor
   ~NcspACID();

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor hc4_;           // HC4
   Scope scop_;                     // scope
   SharedContractorACID acid_;      // ACID

   std::vector<ContractorVar3BCID*>
      var3BCID_;                    // var3BCID_[i] is the var3BCID contractor
                                    // associated with the i-th variable
                                    // of the scope

   size_t n_;        // number of variables of the scope
   int numVarCID_;   // number of var3BCID contractors that must be applied
                     // in a call to the contract method

   std::vector<size_t> kVarCID_;

   size_t call_;        // number of calls of the contract method
   int cycleLength_;    // number of calls in a cycle made of an exploitation
                        // phase and a learning phase
   int learnLength_;    // number of calls in the learning phase
   double ctRatio_;     // threshold on the reduction gain

   // Let ctcGains = g_0, ..., g_(p-1) be the reduction ratios obtained by the
   // p var3BCID contractors applied in some call of the contract method.
   //
   // If g_i <= ctRatio for each i then it returns 0, which means that no
   // contractor is efficient enough.
   //
   // Otherwise let k be the greatest integer such that g_k > ctRatio, which
   // indicates that the gains are not enough after k. Then it returns k+1,
   // which the maximum number of contractors that must be applied in order to
   // obtain a sufficient gain.
   static int lastSignificantGain(std::vector<double>& ctcGains,
                                  double ctRatio);

   // It returns the average
   static size_t avgNbVarCID(std::vector<size_t>& v);
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs based on HC4 contractors and the interval
/// Newton operator.
///
/// Interval Newton is used only for square systems of equations.
///////////////////////////////////////////////////////////////////////////////
class NcspHC4Newton : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspHC4Newton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspHC4 hc4_;
   NcspNewton newton_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs based on BC4 contractors and the interval
/// Newton operator.
///
/// Interval Newton is used only for square systems of equations.
///////////////////////////////////////////////////////////////////////////////
class NcspBC4Newton : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspBC4Newton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspBC4 bc4_;
   NcspNewton newton_;
};

///////////////////////////////////////////////////////////////////////////////
class NcspACIDNewton : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspACIDNewton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspACID acid_;
   NcspNewton newton_;
};

} // namespace

#endif
