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
 * @file   IntervalFunction.hpp
 * @brief  Interval functions
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_INTERVAL_FUNCTION_HPP
#define REALPAVER_INTERVAL_FUNCTION_HPP

#include "realpaver/Dag.hpp"

namespace realpaver {

/**
 * @brief Base class of the hierarchy of representations of interval functions.
 *
 * An interval function is supposed to be differentiable.
 *
 * An interval function is supposed to be associated with an image which makes
 * it an inequality constraint of the form L <= F(x) <= U . Fix L = -oo and
 * U = +oo to eliminate the constraint.
 */
class IntervalFunctionRep {
public:
   /// Default constructor
   IntervalFunctionRep() = default;

   /// Default copy constructor
   IntervalFunctionRep(const IntervalFunctionRep &) = default;

   /// No assignment
   IntervalFunctionRep &operator=(const IntervalFunctionRep &) = delete;

   /// Virtual destructor
   virtual ~IntervalFunctionRep();

   /// Assigns the image of this
   void setImage(const Interval &img);

   /// Returns the image of this
   Interval getImage() const;

   /// Returns the scope of this, i.e. the set of variables
   virtual Scope scope() const = 0;

   /// Returns the number of arguments of this
   virtual size_t nbVars() const = 0;

   /// Returns the evaluation of this on B
   virtual Interval eval(const IntervalBox &B) = 0;

   /**
    * @brief Differentiates this.
    *
    * G is the output vector such that G[i] if the derivative of this in B
    * with respect to the i-th variable of its scope.
    */
   virtual void diff(const IntervalBox &B, IntervalVector &G) = 0;

   /**
    * @brief Differentiates this using Hansen's strategy.
    *
    * G is the output vector such that G[i] if the derivative of this in B
    * with respect to the i-th variable of its scope.
    */
   virtual void diffHansen(const IntervalBox &B, IntervalVector &G) = 0;

private:
   Interval img_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Main class of interval functions.
 *
 * An interval function is supposed to be differentiable.
 *
 * An interval function is supposed to be associated with an image which makes
 * it an inequality constraint of the form L <= F(x) <= U . Fix L = -oo and
 * U = +oo to eliminate the constraint.
 *
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class IntervalFunction {
public:
   /// Constructor from the i-th function of a DAG
   IntervalFunction(SharedDag dag, size_t i);

   /// Constructor that creates a DAG from a term and assigns its image
   IntervalFunction(Term t, const Interval &img = Interval::universe());

   /// Default destructor
   ~IntervalFunction() = default;

   /// Default copy constructor
   IntervalFunction(const IntervalFunction &) = default;

   /// No assignment
   IntervalFunction &operator=(const IntervalFunction &) = delete;

   /// Assigns the image of this
   void setImage(const Interval &img);

   /// Returns the image of this
   Interval getImage() const;

   /// Returns the scope of this, i.e. the set of variables
   Scope scope() const;

   /// @return the number of arguments of this
   size_t nbVars() const;

   /// Returns the evaluation of this on B
   Interval eval(const IntervalBox &B);

   /**
    * @brief Differentiates this.
    *
    * G is the output vector such that G[i] if the derivative of this in B
    * with respect to the i-th variable of its scope
    */
   void diff(const IntervalBox &B, IntervalVector &G);

   /**
    * @brief Differentiates this using Hansen's strategy.
    *
    * G is the output vector such that G[i] if the derivative of this in B
    * with respect to the i-th variable of its scope
    */
   void diffHansen(const IntervalBox &B, IntervalVector &G);

   /// Type of shared pointer to a representation
   using SharedRep = std::shared_ptr<IntervalFunctionRep>;

   /// Constructor
   IntervalFunction(SharedRep rep);

   /// Returns the representation of this
   SharedRep rep() const;

private:
   SharedRep rep_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Representation of an interval function in a DAG.
 *
 * This class is an adapter of the DagFun class.
 */
class IntervalFunctionDag : public IntervalFunctionRep {
public:
   /// Constructor from the i-th function of a DAG
   IntervalFunctionDag(SharedDag dag, size_t i);

   /// Constructor that creates a DAG from a term and assigns its image
   IntervalFunctionDag(Term t, const Interval &img = Interval::universe());

   /// Default destructor
   ~IntervalFunctionDag() = default;

   /// Copy constructor
   IntervalFunctionDag(const IntervalFunctionDag &) = default;

   /// No asssignment
   IntervalFunctionDag &operator=(const IntervalFunctionDag &) = delete;

   /// Returns the dag
   SharedDag dag() const;

   /// Returns the function index in the dag
   size_t index() const;

   Scope scope() const override;
   size_t nbVars() const override;
   Interval eval(const IntervalBox &B) override;
   void diff(const IntervalBox &B, IntervalVector &G) override;
   void diffHansen(const IntervalBox &B, IntervalVector &G) override;

private:
   SharedDag dag_; // DAG
   size_t index_;  // index of function
};

} // namespace realpaver

#endif
