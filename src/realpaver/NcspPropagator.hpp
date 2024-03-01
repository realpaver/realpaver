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
/// This is a propagator for NCSPs corresponding to the HC4 constraint
/// propagation algorithm.
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
/// This is a propagator for NCSPs corresponding to the BC4 constraint
/// propagation algorithm.
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
/// This is a propagator for NCSPs that applies the polytope contractor.
///////////////////////////////////////////////////////////////////////////////
class NcspPolytope : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspPolytope(ContractorFactory& facto);

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

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs that applies in sequence a NcspHC4
/// operator and a NcspNewton operator.
///
/// The Newton operator is used only for square systems of equations.
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
/// This is a propagator for NCSPs that applies in sequence a NcspBC4
/// operator and a NcspNewton operator.
///
/// The Newton operator is used only for square systems of equations.
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
/// This is a propagator for NCSPs that applies in sequence a NcspACID
/// operator and a NcspNewton operator.

///
/// The Newton operator is used only for square systems of equations.
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

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs that applies in sequence a NcspHC4
/// operator and a NcspPolytope operator.
///////////////////////////////////////////////////////////////////////////////
class NcspHC4Polytope : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspHC4Polytope(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspHC4 hc4_;
   NcspPolytope poly_;;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs that applies in sequence a NcspBC4
/// operator and a NcspPolytope operator.
///////////////////////////////////////////////////////////////////////////////
class NcspBC4Polytope : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspBC4Polytope(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspBC4 bc4_;
   NcspPolytope poly_;;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs that applies in sequence a NcspACID
/// operator and a NcspPolytope operator.
///////////////////////////////////////////////////////////////////////////////
class NcspACIDPolytope : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspACIDPolytope(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspACID acid_;
   NcspPolytope poly_;;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs that applies in sequence a NcspHC4
/// operator, a NcspPolytope operator and a NcspNewton operator.
///////////////////////////////////////////////////////////////////////////////
class NcspHC4PolytopeNewton : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspHC4PolytopeNewton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspHC4Polytope hc4poly_;
   NcspNewton newton_;;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs that applies in sequence a NcspBC4
/// operator, a NcspPolytope operator and a NcspNewton operator.
///////////////////////////////////////////////////////////////////////////////
class NcspBC4PolytopeNewton : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspBC4PolytopeNewton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspBC4Polytope bc4poly_;
   NcspNewton newton_;;
};


///////////////////////////////////////////////////////////////////////////////
/// This is a propagator for NCSPs that applies in sequence a NcspACID
/// operator, a NcspPolytope operator and a NcspNewton operator.
///////////////////////////////////////////////////////////////////////////////
class NcspACIDPolytopeNewton : public NcspPropagator {
public:
   /// Creates a contractor
   /// @param facto a factory
   NcspACIDPolytopeNewton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspACIDPolytope acidpoly_;
   NcspNewton newton_;;
};

} // namespace

#endif
