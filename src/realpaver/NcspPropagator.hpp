/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   NcspPropagator.hpp
 * @brief  Propagators of NCSP solver
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_NCSP_PROPAGATOR_HPP
#define REALPAVER_NCSP_PROPAGATOR_HPP

#include <vector>
#include "realpaver/ContractorFactory.hpp"
#include "realpaver/ContractorVar3BCID.hpp"
#include "realpaver/NcspContext.hpp"
#include "realpaver/NcspNode.hpp"

namespace realpaver {

/**
 * @brief Base class of propagators of NCSP solver.
 * 
 * A propagator is an algorithm / operator that contracts the domains of
 * variables occurring in a NCSP search node.
 * 
 * Concrete propagators are built in contractor factories.
 */
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

   /**
    * @brief Contraction method.
    * 
    * It applies first contractImpl and then the domain contractors for
    * variables with disconnected domains.
    * 
    * Returns a certificate of proof
    */
   Proof contract(NcspNode& node, NcspContext& ctx);

   /// Reduces box by intersecting it with B
   static Proof contractBox(const IntervalBox& B, DomainBox& box);

protected:
   IntervalBox* B_;

   /// Contraction method to be overriden in sub-classes
   virtual Proof contractImpl(NcspNode& node, NcspContext& ctx) = 0;
};

/*----------------------------------------------------------------------------*/

/// HC4 constraint propagation algorithm
class NcspHC4 : public NcspPropagator {
public:
   /// Constructor
   NcspHC4(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;
};

/*----------------------------------------------------------------------------*/

/// BC4 constraint propagation algorithm
class NcspBC4 : public NcspPropagator {
public:
   /// Constructor
   NcspBC4(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;
};

/*----------------------------------------------------------------------------*/

/// Interval Newton operator
class NcspNewton : public NcspPropagator {
public:
   /// Constructor
   NcspNewton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;
};

/*----------------------------------------------------------------------------*/

/// Polytope contractor
class NcspPolytope : public NcspPropagator {
public:
   /// Constructor
   NcspPolytope(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;
};

/*----------------------------------------------------------------------------*/

/// ACID algorithm based on HC4 contractors
class NcspACID : public NcspPropagator {
public:
   /// Constructor
   NcspACID(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   SharedContractor op_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Applies in sequence NcspHC4 and NcspNewton.
 *
 * The Newton operator is used only for square systems of equations.
 */
class NcspHC4Newton : public NcspPropagator {
public:
   /// Constructor
   NcspHC4Newton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspHC4 hc4_;
   NcspNewton newton_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Applies in sequence NcspBC4 and NcspNewton.
 *
 * The Newton operator is used only for square systems of equations.
 */
class NcspBC4Newton : public NcspPropagator {
public:
   /// Constructor
   NcspBC4Newton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspBC4 bc4_;
   NcspNewton newton_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Applies in sequence NcspACID and NcspNewton.
 * 
 * The Newton operator is used only for square systems of equations.
 */
class NcspACIDNewton : public NcspPropagator {
public:
   /// Constructor
   NcspACIDNewton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspACID acid_;
   NcspNewton newton_;
};

/*----------------------------------------------------------------------------*/

/// Applies in sequence NcspHC4 and NcspPolytope
class NcspHC4Polytope : public NcspPropagator {
public:
   /// Constructor
   NcspHC4Polytope(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspHC4 hc4_;
   NcspPolytope poly_;
};

/*----------------------------------------------------------------------------*/

/// Applies in sequence NcspBC4 and NcspPolytope
class NcspBC4Polytope : public NcspPropagator {
public:
   /// Constructor
   NcspBC4Polytope(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspBC4 bc4_;
   NcspPolytope poly_;
};

/*----------------------------------------------------------------------------*/

/// Applies in sequence NcspACID and NcspPolytope
class NcspACIDPolytope : public NcspPropagator {
public:
   /// Constructor
   NcspACIDPolytope(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspACID acid_;
   NcspPolytope poly_;
};

/*----------------------------------------------------------------------------*/

/// Applies in sequence NcspHC4, NcspPolytope and NcspNewton
class NcspHC4PolytopeNewton : public NcspPropagator {
public:
   /// Constructor
   NcspHC4PolytopeNewton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspHC4Polytope hc4poly_;
   NcspNewton newton_;
};

/*----------------------------------------------------------------------------*/

/// Applies in sequence NcspBC4, NcspPolytope, and NcspNewton
class NcspBC4PolytopeNewton : public NcspPropagator {
public:
   /// Constructor
   NcspBC4PolytopeNewton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspBC4Polytope bc4poly_;
   NcspNewton newton_;
};

/*----------------------------------------------------------------------------*/

/// Aplies in sequence NcspACID, NcspPolytope, and NcspNewton
class NcspACIDPolytopeNewton : public NcspPropagator {
public:
   /// Constructor
   NcspACIDPolytopeNewton(ContractorFactory& facto);

   Proof contractImpl(NcspNode& node, NcspContext& ctx) override;

private:
   NcspACIDPolytope acidpoly_;
   NcspNewton newton_;
};

} // namespace

#endif
