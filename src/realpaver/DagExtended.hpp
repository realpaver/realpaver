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
 * @file   DagExtended.hpp
 * @brief  DAG representation of a problem
 * @author Laurent Granvilliers
 * @date   3 Apr 2026
 */

#ifndef REALPAVER_DAG_EXTENDED_HPP
#define REALPAVER_DAG_EXTENDED_HPP

#include "RealPoint.hpp"
#include "realpaver/Constraint.hpp"
#include "realpaver/ConstraintVisitor.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Problem.hpp"
#include <memory>
#include <vector>

namespace realpaver {

class DagFunExtVisitor;

/**
 * @brief Function in an extended DAG.
 *
 * A function represents a constraint inserted in a DAG.
 */
class DagFunExt {
public:
   /// Constructor
   DagFunExt(SharedDag dag, Constraint ctr);

   /// Virtual destructor
   virtual ~DagFunExt();

   /// Default copy constructor
   DagFunExt(const DagFunExt &) = default;

   /// No assignment
   DagFunExt &operator=(const DagFunExt &) = delete;

   /// Returns the constraint enclosed
   Constraint ctr() const;

   /// Returns the DAG
   SharedDag dag() const;

   /**
    * @brief Activates a DAG function.
    *
    * Returns a DAG function (constraint) associated with the given constraint
    * that is active in B; nullptr if there is no function to activate, in particular
    * if the constraint is not active with respect to the given bitset.
    */
   SharedDagFun getActive(const IntervalBox &B, const Bitset &active);

   /**
    * @brief Activates a DAG function.
    *
    * Returns a DAG function (constraint) associated with the given constraint
    * that is active in B; nullptr if there is no function to activate, in particular
    * if the constraint is not active with respect to the given bitset.
    */
   SharedDagFun getActive(const RealPoint &P, const Bitset &active);

   /// Visitor pattern
   virtual void acceptVisitor(DagFunExtVisitor &vis) const = 0;

protected:
   SharedDag dag_;  // dag
   Constraint ctr_; // a constraint

   // Implementation
   virtual SharedDagFun getActiveImpl(const IntervalBox &B) = 0;
   virtual SharedDagFun getActiveImpl(const RealPoint &P) = 0;
};

/*----------------------------------------------------------------------------*/

/// Extended DAG function representing an arithmetic constraint
class DagFunArith : public DagFunExt {
public:
   /// Constructor given a constraint ctr at index i in the DAG
   DagFunArith(SharedDag dag, Constraint ctr, int i);

   /// Default copy constructor
   DagFunArith(const DagFunArith &) = default;

   /// No assignment
   DagFunArith &operator=(const DagFunArith &) = delete;

   /// Default destructor
   ~DagFunArith() = default;

   /// Returns the index of this in the DAG
   int index() const;

   SharedDagFun getActiveImpl(const IntervalBox &B) override;
   SharedDagFun getActiveImpl(const RealPoint &P) override;
   void acceptVisitor(DagFunExtVisitor &vis) const override;

private:
   int idx_; // index in the DAG
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Extended DAG function representing a conditoonal constraint.
 *
 * Only the body of the conditional constraint is inserted in the DAG, if it is
 * an arithmetic constraint. It can be activated depending on the guard of the conditional
 * constraint.
 */
class DagFunCond : public DagFunExt {
public:
   /// Constructor given a constraijt ctr whose body is at index i in the DAG
   DagFunCond(SharedDag dag, Constraint ctr, int i);

   /// Default copy constructor
   DagFunCond(const DagFunCond &) = default;

   /// No assignment
   DagFunCond &operator=(const DagFunCond &) = delete;

   /// Default destructor
   ~DagFunCond() = default;

   SharedDagFun getActiveImpl(const IntervalBox &B) override;
   SharedDagFun getActiveImpl(const RealPoint &P) override;
   void acceptVisitor(DagFunExtVisitor &vis) const override;

private:
   int idx_; // index of body in the DAG
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Extended DAG function representing a piecewise constraint.
 *
 * The arithmetic constraints of the different pieces are inserted in the DAG.
 * They can be activated depending on the domain of the reference variable.
 */
class DagFunPiecewise : public DagFunExt {
public:
   DagFunPiecewise(SharedDag dag, Constraint ctr);

   /// Default copy constructor
   DagFunPiecewise(const DagFunPiecewise &) = default;

   /// No assignment
   DagFunPiecewise &operator=(const DagFunPiecewise &) = delete;

   /// Default destructor
   ~DagFunPiecewise() = default;

   /// Adds the index of a piece in the DAG
   void addIndex(int i);

   SharedDagFun getActiveImpl(const IntervalBox &B) override;
   SharedDagFun getActiveImpl(const RealPoint &P) override;
   void acceptVisitor(DagFunExtVisitor &vis) const override;

private:
   std::vector<int> idx_; // indexes in the DAG
};

/*----------------------------------------------------------------------------*/

/// Base class of visitors of functions in an extended DAG
class DagFunExtVisitor {
public:
   /// Virtual destructor
   virtual ~DagFunExtVisitor();

   /// @name Visit methods
   ///@{
   virtual void apply(const DagFunArith *f);
   virtual void apply(const DagFunCond *f);
   virtual void apply(const DagFunPiecewise *f);
   ///@}
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Extended DAG.
 *
 * An extended DAG is a DAG with a control on the activation of the constraints.
 * Every constraint in this can activate a constraint, e.g.: the constraint itself for
 * an arithmetic constraint, the body of a conditional constraint, one piece of a
 * piecewise constraint.
 */
class DagExtended {
public:
   /// Creates an extended DAG on a problem
   DagExtended(const Problem &pbm);

   /// No copy
   DagExtended(const DagExtended &) = delete;

   /// No assignment
   DagExtended &operator=(const DagExtended &) = delete;

   /// Default destructor
   ~DagExtended() = default;

   /// Returns the DAG enclosed
   SharedDag dag() const;

   /// Returns the scope of this (scope of the DAG enclosed)
   Scope scope() const;

   /// Returns the number of constraints in this
   size_t size() const;

   /// Return true if size()==0
   bool isEmpty() const;

   /// Adds a function representing a constraint of the given problem
   void addFun(std::shared_ptr<DagFunExt> f);

   /// Returns the i-th function of this
   std::shared_ptr<DagFunExt> fun(size_t i) const;

   /**
    * @brief Gets an active function.
    *
    * Returns an active function (constraint) from the i-th function in this; nullptr
    * if there is no active function.
    */
   SharedDagFun getActiveFun(size_t i, const IntervalBox &B, const Bitset &active);
   SharedDagFun getActiveFun(size_t i, const RealPoint &P, const Bitset &active);

   /// Returns the number of constraints that have not been inserted
   size_t nbBadCtrs() const;

   /// Returns the i-th constraints that has not been inserted
   Constraint badCtr(size_t i) const;

private:
   SharedDag dag_;                                // DAG
   std::vector<std::shared_ptr<DagFunExt>> lfun_; // functions
   std::vector<Constraint> lctr_;                 // constraints not inserted
   Scope scope_;
};

/// Shared pointer on extended DAG
using SharedDagExtended = std::shared_ptr<DagExtended>;

/*----------------------------------------------------------------------------*/

/**
 * @brief Visitor that creates an extended DAG function from a constraint.
 *
 * The DAG function is directly inserted in the given extended DAG.
 */
class DagExtendedCreator : public ConstraintVisitor {
public:
   /// Constructor
   DagExtendedCreator(DagExtended *edag_, Constraint ctr);

   void apply(const ConstraintCond *c) override;
   void apply(const ConstraintTable *c) override;
   void apply(const ConstraintPiecewise *c) override;
   void apply(const ConstraintEq *c) override;
   void apply(const ConstraintLe *c) override;
   void apply(const ConstraintLt *c) override;
   void apply(const ConstraintGe *c) override;
   void apply(const ConstraintGt *c) override;
   void apply(const ConstraintIn *c) override;

   /// Returns true if the constraint has been inserted, false otherwise
   bool success() const;

private:
   DagExtended *edag_; // extended DAG
   Constraint ctr_;    // constraint
   bool success_;      // true if the constraint has been inserted

   // code for arithmetic constraints
   void makeArith();
};

} // namespace realpaver

#endif
