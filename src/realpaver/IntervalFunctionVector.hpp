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
 * @file   IntervalFunctionVector.hpp
 * @brief  Vector of interval functions
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_INTERVAL_FUNCTION_VECTOR_HPP
#define REALPAVER_INTERVAL_FUNCTION_VECTOR_HPP

#include "realpaver/Dag.hpp"
#include "realpaver/IntervalFunction.hpp"
#include <vector>

namespace realpaver {

/// Base class of the hierarchy of representations of interval function vectors
class IntervalFunctionVectorRep {
public:
   /// Default constructor
   IntervalFunctionVectorRep() = default;

   /// Virtual destructor
   virtual ~IntervalFunctionVectorRep();

   /// Default copy constructor
   IntervalFunctionVectorRep(const IntervalFunctionVectorRep &) = default;

   /// No assignment
   IntervalFunctionVectorRep &operator=(const IntervalFunctionVectorRep &) = delete;

   /// Returns the scope of this, i.e. the set of variables
   virtual Scope scope() const = 0;

   /// Returns the number of variables in this
   virtual size_t nbVars() const = 0;

   /// Returns the number of functions in this
   virtual size_t nbFuns() const = 0;

   /// Returns the i-th function of this
   virtual IntervalFunction fun(size_t i) const = 0;

   /**
    * @brief Evaluates this.
    *
    * val[i] is the result of the evaluation of the i-th function of this on B
    * and val must have nbFuns() components
    */
   virtual void eval(const IntervalBox &B, IntervalVector &val) = 0;

   /**
    * @brief Differentiates this (calculates an interval Jacobian matrix).
    *
    * J is the Jacobian matrix of this on B such that we have the partial
    * derivative dfi / dxj in the i-th row and j-th column of J.
    *
    * J must have nbFuns() rows and nbVars() columns.
    */
   virtual void diff(const IntervalBox &B, IntervalMatrix &J) = 0;

   /**
    * @brief Differentiates this using Hansen's strategy.
    *
    * H[i, j] is the partial derivative dfi / dxj. H must have nbFuns() rows
    * and nbVars() columns.
    */
   virtual void diffHansen(const IntervalBox &B, IntervalMatrix &H) = 0;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Main class of interval function vectors.
 *
 *  This encloses a shared pointer to its representation. It is a lightweight
 *  object that can be copied and assigned.
 */
class IntervalFunctionVector {
public:
   /// @name Constructors
   ///@{

   /// Creates an empty vector
   IntervalFunctionVector();

   /// Creates a vector corresponding to a DAG
   IntervalFunctionVector(SharedDag dag);

   /// Creates  a vector corresponding to a DAG built from a list of terms
   IntervalFunctionVector(const std::initializer_list<Term> &lt);

   /**
    * @brief Constructor.
    *
    * Creates  a vector corresponding to a DAG built from a list of terms and
    * a list of bounds. For each i, we have lt[i] IN li[i].
    */
   IntervalFunctionVector(const std::initializer_list<Term> &lt,
                          const std::initializer_list<Interval> &li);

   /// Creates a vector from a list of fonctions
   IntervalFunctionVector(const std::initializer_list<IntervalFunction> &lf);

   /// Default copy constructor
   IntervalFunctionVector(const IntervalFunctionVector &) = default;
   ///@}

   /// No assignment
   IntervalFunctionVector &operator=(const IntervalFunctionVector &) = delete;

   /// Default destructor
   ~IntervalFunctionVector() = default;

   /// Returns the scope of this, i.e. the set of variables
   Scope scope() const;

   /// Returns the number of variables in this
   size_t nbVars() const;

   /// Returns the number of functions in this
   size_t nbFuns() const;

   /// Returns true if nbVars() == nbFuns() and this is non empty
   bool isSquare() const;

   /// Return sthe i-th function of this
   IntervalFunction fun(size_t i) const;

   /**
    * @brief Inserts a function at the end.
    *
    * It may be necessary to switch to another representation if the current
    * one is not a list.
    */
   void addFun(IntervalFunction f);

   /**
    * @brief Evaluates this.
    *
    * val[i] is the result of the evaluation of the i-th function of this on B
    * and val must have nbFuns() components
    */
   void eval(const IntervalBox &B, IntervalVector &val);

   /**
    * @brief Differentiates this (calculates an interval Jacobian matrix).
    *
    * J is the Jacobian matrix of this on B such that we have the partial
    * derivative dfi / dxj in the i-th row and j-th column of J.
    *
    * J must have nbFuns() rows and nbVars() columns.
    */
   void diff(const IntervalBox &B, IntervalMatrix &J);

   /**
    * @brief Differentiates this using Hansen's strategy.
    *
    * H[i, j] is the partial derivative dfi / dxj. H must have nbFuns() rows
    * and nbVars() columns.
    */
   void diffHansen(const IntervalBox &B, IntervalMatrix &H);

   /// Type of the representation of interval functions vectors
   using SharedRep = std::shared_ptr<IntervalFunctionVectorRep>;

   /// Returns the representation of this
   SharedRep rep() const;

   /// Constructor
   IntervalFunctionVector(SharedRep rep);

private:
   SharedRep rep_;
};

/*----------------------------------------------------------------------------*/

/// Vector of interval functions reflecting a DAG
class IntervalFunctionVectorDag : public IntervalFunctionVectorRep {
public:
   /// Creates a vector corresponding to a DAG
   IntervalFunctionVectorDag(SharedDag dag);

   /// Creates  a vector corresponding to a DAG built from a list of terms
   IntervalFunctionVectorDag(const std::initializer_list<Term> &lt);

   /**
    * @brief Constructor.
    *
    * Creates  a vector corresponding to a DAG built from a list of terms and
    * a list of bounds. For each i, we have lt[i] IN li[i].
    */
   IntervalFunctionVectorDag(const std::initializer_list<Term> &lt,
                             const std::initializer_list<Interval> &li);

   /// Default copy constructor
   IntervalFunctionVectorDag(const IntervalFunctionVectorDag &) = default;

   /// No assignment
   IntervalFunctionVectorDag &operator=(const IntervalFunctionVectorDag &) = delete;

   /// Default destructor
   ~IntervalFunctionVectorDag() = default;

   /// Returns the dag enclosed in this
   SharedDag dag() const;

   Scope scope() const override;
   size_t nbVars() const override;
   size_t nbFuns() const override;
   IntervalFunction fun(size_t i) const override;
   void eval(const IntervalBox &B, IntervalVector &val) override;
   void diff(const IntervalBox &B, IntervalMatrix &J) override;
   void diffHansen(const IntervalBox &B, IntervalMatrix &H) override;

private:
   SharedDag dag_;
};

/*----------------------------------------------------------------------------*/

/// Vector of interval-valued functions based on a list
class IntervalFunctionVectorList : public IntervalFunctionVectorRep {
public:
   /// Constructor of an empty function vector
   IntervalFunctionVectorList();

   /// Constructor
   IntervalFunctionVectorList(const std::initializer_list<IntervalFunction> &lf);

   /// Default copy constructor
   IntervalFunctionVectorList(const IntervalFunctionVectorList &) = default;

   /// No assignment
   IntervalFunctionVectorList &operator=(const IntervalFunctionVectorList &) = delete;

   /// Default destructor
   ~IntervalFunctionVectorList() = default;

   /// Inserts a function at the end
   void addFun(IntervalFunction f);

   Scope scope() const override;
   size_t nbVars() const override;
   size_t nbFuns() const override;
   IntervalFunction fun(size_t i) const override;
   void eval(const IntervalBox &B, IntervalVector &val) override;
   void diff(const IntervalBox &B, IntervalMatrix &J) override;
   void diffHansen(const IntervalBox &B, IntervalMatrix &H) override;

private:
   std::vector<IntervalFunction> vf_;
   Scope scop_;
};

} // namespace realpaver

#endif
