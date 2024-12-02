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
 * @file   CSPPropagator.hpp
 * @brief  Propagators of CSP solver
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CSP_PROPAGATOR_HPP
#define REALPAVER_CSP_PROPAGATOR_HPP

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/CSPContext.hpp"
#include "realpaver/CSPNode.hpp"

namespace realpaver {

/**
 * @brief Base class of propagators of CSP solver.
 *
 * A propagator is an algorithm / operator that contracts the domains of
 * variables occurring in a CSP search node.
 *
 * Concrete propagators are built in contractor factories.
 */
class CSPPropagator {
public:
   /// Constructor
   CSPPropagator();

   /// Default copy constructor
   CSPPropagator(const CSPPropagator&) = default;

   /// No assignment
   CSPPropagator& operator=(const CSPPropagator&) = delete;

   /// Virtual destructor
   virtual ~CSPPropagator();

   /**
    * @brief Contraction method.
    *
    * It applies first contractImpl and then the domain contractors for
    * variables with disconnected domains.
    *
    * Returns a certificate of proof
    */
   Proof contract(CSPNode& node, CSPContext& ctx);

   /// Reduces box by intersecting it with B
   static Proof contractBox(const IntervalBox& B, DomainBox& box);

protected:
   IntervalBox* B_;

   /// Contraction method to be overriden in sub-classes
   virtual Proof contractImpl(CSPNode& node, CSPContext& ctx) = 0;
};

/*----------------------------------------------------------------------------*/

/// HC4 constraint propagation algorithm
class CSPPropagatorHC4 : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorHC4(ContractorFactory& facto);

   Proof contractImpl(CSPNode& node, CSPContext& ctx) override;

private:
   SharedContractor op_;
};

/*----------------------------------------------------------------------------*/

/// BC4 constraint propagation algorithm
class CSPPropagatorBC4 : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorBC4(ContractorFactory& facto);

   Proof contractImpl(CSPNode& node, CSPContext& ctx) override;

private:
   SharedContractor op_;
};

/*----------------------------------------------------------------------------*/

/// Interval Newton operator
class CSPPropagatorNewton : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorNewton(ContractorFactory& facto);

   Proof contractImpl(CSPNode& node, CSPContext& ctx) override;

private:
   SharedContractor op_;
};

/*----------------------------------------------------------------------------*/

/// ACID algorithm based on HC4 contractors
class CSPPropagatorACID : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorACID(ContractorFactory& facto);

   Proof contractImpl(CSPNode& node, CSPContext& ctx) override;

private:
   SharedContractor hc4_;
   SharedContractor op_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Applies in sequence CSPPropagatorHC4 and CSPPropagatorNewton.
 *
 * The Newton operator is used only for square systems of equations.
 */
class CSPPropagatorHC4Newton : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorHC4Newton(ContractorFactory& facto);

   Proof contractImpl(CSPNode& node, CSPContext& ctx) override;

private:
   CSPPropagatorHC4 hc4_;
   CSPPropagatorNewton newton_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Applies in sequence CSPPropagatorBC4 and CSPPropagatorNewton.
 *
 * The Newton operator is used only for square systems of equations.
 */
class CSPPropagatorBC4Newton : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorBC4Newton(ContractorFactory& facto);

   Proof contractImpl(CSPNode& node, CSPContext& ctx) override;

private:
   CSPPropagatorBC4 bc4_;
   CSPPropagatorNewton newton_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Applies in sequence CSPPropagatorACID and CSPPropagatorNewton.
 *
 * The Newton operator is used only for square systems of equations.
 */
class CSPPropagatorACIDNewton : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorACIDNewton(ContractorFactory& facto);

   Proof contractImpl(CSPNode& node, CSPContext& ctx) override;

private:
   CSPPropagatorACID acid_;
   CSPPropagatorNewton newton_;
};

} // namespace

#endif
