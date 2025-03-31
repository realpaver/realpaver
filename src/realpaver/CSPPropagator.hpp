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

#include "realpaver/CSPContext.hpp"
#include "realpaver/CSPNode.hpp"
#include "realpaver/ContractorFactory.hpp"
#include <vector>

namespace realpaver {

/// Type of reduction methods for CSPs
enum class CSPPropagAlgo {
   HC4,      ///< propagation algorithm based on HC4 contractors
   BC4,      ///< propagation algorithm based on BC4 contractors
   ACID,     ///< adaptive CID
   Polytope, ///< polytope hull contractor
   Newton    ///< interval Newton operator
};

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
   CSPPropagator(const CSPPropagator &) = default;

   /// No assignment
   CSPPropagator &operator=(const CSPPropagator &) = delete;

   /// Virtual destructor
   virtual ~CSPPropagator();

   /**
    * @brief Contraction method.
    *
    * It applies first contractImpl and then the domain contractors for
    * the variables with disconnected domains.
    *
    * Returns a certificate of proof
    */
   Proof contract(CSPNode &node, CSPContext &ctx);

   /// Reduces box by intersecting it with B
   static Proof contractBox(const IntervalBox &B, DomainBox &box);

   /// Contraction method to be overriden in sub-classes
   virtual Proof contractImpl(IntervalBox &B) = 0;
};

/// Type of shared pointers on CSP propagators
using SharedCSPPropagator = std::shared_ptr<CSPPropagator>;

/*----------------------------------------------------------------------------*/

/// HC4 constraint propagation algorithm
class CSPPropagatorHC4 : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorHC4(ContractorFactory &facto);

   /// Default copy constructor
   CSPPropagatorHC4(const CSPPropagatorHC4 &) = default;

   /// No assignment
   CSPPropagatorHC4 &operator=(const CSPPropagatorHC4 &) = delete;

   /// Default destructor
   ~CSPPropagatorHC4() = default;

   Proof contractImpl(IntervalBox &B) override;

private:
   SharedContractor hc4_;
};

/*----------------------------------------------------------------------------*/

/// BC4 constraint propagation algorithm
class CSPPropagatorBC4 : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorBC4(ContractorFactory &facto);

   /// Default copy constructor
   CSPPropagatorBC4(const CSPPropagatorBC4 &) = default;

   /// No assignment
   CSPPropagatorBC4 &operator=(const CSPPropagatorBC4 &) = delete;

   /// Default destructor
   ~CSPPropagatorBC4() = default;

   Proof contractImpl(IntervalBox &B) override;

private:
   SharedContractor bc4_;
};

/*----------------------------------------------------------------------------*/

/// Interval Newton operator
class CSPPropagatorNewton : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorNewton(ContractorFactory &facto);

   /// Default copy constructor
   CSPPropagatorNewton(const CSPPropagatorNewton &) = default;

   /// No assignment
   CSPPropagatorNewton &operator=(const CSPPropagatorNewton &) = delete;

   /// Default destructor
   ~CSPPropagatorNewton() = default;

   Proof contractImpl(IntervalBox &B) override;

private:
   SharedContractor nwt_;
};

/*----------------------------------------------------------------------------*/

/// Poly hull contractor
class CSPPropagatorPolytope : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorPolytope(ContractorFactory &facto);

   /// Default copy constructor
   CSPPropagatorPolytope(const CSPPropagatorPolytope &) = default;

   /// No assignment
   CSPPropagatorPolytope &operator=(const CSPPropagatorPolytope &) = delete;

   /// Default destructor
   ~CSPPropagatorPolytope() = default;

   Proof contractImpl(IntervalBox &B) override;

private:
   SharedContractor poly_;
};

/*----------------------------------------------------------------------------*/

/// ACID algorithm based on HC4 contractors
class CSPPropagatorACID : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorACID(ContractorFactory &facto);

   /// Default copy constructor
   CSPPropagatorACID(const CSPPropagatorACID &) = default;

   /// No assignment
   CSPPropagatorACID &operator=(const CSPPropagatorACID &) = delete;

   /// Default destructor
   ~CSPPropagatorACID() = default;

   Proof contractImpl(IntervalBox &B) override;

private:
   SharedContractor hc4_;
   SharedContractor acid_;
};

/*----------------------------------------------------------------------------*/

/// List of propagators applied in sequence to contract a CSP node
class CSPPropagatorList : public CSPPropagator {
public:
   /// Constructor
   CSPPropagatorList();

   /// Default copy constructor
   CSPPropagatorList(const CSPPropagatorList &) = default;

   /// No assignment
   CSPPropagatorList &operator=(const CSPPropagatorList &) = delete;

   /// Default destructor
   ~CSPPropagatorList() = default;

   /// Returns the number of contractors
   size_t size() const;

   /// Inserts a new algorithm in the last place
   void pushBack(CSPPropagAlgo alg, ContractorFactory &facto);

   Proof contractImpl(IntervalBox &B) override;

private:
   std::vector<SharedCSPPropagator> v_;
};

} // namespace realpaver

#endif
