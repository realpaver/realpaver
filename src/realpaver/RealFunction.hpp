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
 * @file   RealFunction.hpp
 * @brief  Real functions
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_REAL_FUNCTION_HPP
#define REALPAVER_REAL_FUNCTION_HPP

#include "realpaver/Dag.hpp"

namespace realpaver {

/// Base class of the hierarchy of representations of real functions
class RealFunctionRep {
public:
   /// Default constructor
   RealFunctionRep() = default;

   /// Default copy constructor
   RealFunctionRep(const RealFunctionRep &) = default;

   /// No assignment
   RealFunctionRep &operator=(const RealFunctionRep &) = delete;

   /// Virtual destructor
   virtual ~RealFunctionRep();

   /// Assigns the image of this
   void setImage(const Interval &img);

   /// Returns the image of this
   Interval getImage() const;

   /// Returns the scope of this, i.e. the set of variables
   virtual Scope scope() const = 0;

   /// Returns the number of arguments of this
   virtual size_t nbVars() const = 0;

   /// Returns the evaluation of this on pt
   virtual double eval(const RealPoint &pt) = 0;

   /**
    * @brief Differentiates this.
    *
    * G is the output vector such that G[i] if the derivative of this
    * at pt with respect to the i-th variable of its scope
    */
   virtual void diff(const RealPoint &pt, RealVector &G) = 0;

protected:
   Interval img_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Main class of real functions.
 *
 * A real function is supposed to be differentiable.
 *
 * A real function is supposed to be associated with an image which makes
 * it an inequality constraint of the form L <= F(x) <= U. Fix L = -oo and
 * U = +oo to eliminate the constraint.
 *
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class RealFunction {
public:
   /// Constructor from the i-th function of a DAG
   RealFunction(SharedDag dag, size_t i);

   /// Constructor that creates a DAG from a term and assigns its image
   RealFunction(Term t, const Interval &img = Interval::universe());

   /// Default destructor
   ~RealFunction() = default;

   /// Default copy constructor
   RealFunction(const RealFunction &) = default;

   /// No assignment
   RealFunction &operator=(const RealFunction &) = delete;

   /// Assigns the image of this
   void setImage(const Interval &img);

   /// Returns the image of this
   Interval getImage() const;

   /// Returns the scope of this, i.e. the set of variables
   Scope scope() const;

   /// Returns the number of arguments of this
   size_t nbVars() const;

   /// Evaluates this at pt
   double eval(const RealPoint &pt);

   /**
    * @brief Differentiates this.
    *
    * G is the output vector such that G[i] if the derivative of this
    * at pt with respect to the i-th variable of its scope
    */
   void diff(const RealPoint &pt, RealVector &G);

   /// Type of shared pointer to a representation
   using SharedRep = std::shared_ptr<RealFunctionRep>;

   /// Constructor
   RealFunction(SharedRep rep);

   /// Returns the representation of this
   SharedRep rep() const;

private:
   SharedRep rep_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Representation of a real function in a DAG.
 *
 * This class is an adapter of the DagFun class.
 */
class RealFunctionDag : public RealFunctionRep {
public:
   /// Constructor from the i-th function of a DAG
   RealFunctionDag(SharedDag dag, size_t i);

   /// Constructor that creates a DAG from a term and assigns its image
   RealFunctionDag(Term t, const Interval &img = Interval::universe());

   /// Default destructor
   ~RealFunctionDag() = default;

   /// Copy constructor
   RealFunctionDag(const RealFunctionDag &) = default;

   /// No asssignment
   RealFunctionDag &operator=(const RealFunctionDag &) = delete;

   /// Returns the dag
   SharedDag dag() const;

   /// Returns the function index in the dag
   size_t index() const;

   Scope scope() const override;
   size_t nbVars() const override;
   double eval(const RealPoint &pt) override;
   void diff(const RealPoint &pt, RealVector &G) override;

private:
   SharedDag dag_; // DAG
   size_t index_;  // index of function
};

} // namespace realpaver

#endif
