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
 * @file   RealFunctionVector.hpp
 * @brief  Vector of real functions
 * @author Laurent Granvilliers
 * @date   2022-5-6
 */

#ifndef REALPAVER_REAL_FUNCTION_VECTOR_HPP
#define REALPAVER_REAL_FUNCTION_VECTOR_HPP

#include <vector>
#include "realpaver/Dag.hpp"
#include "realpaver/RealFunction.hpp"

namespace realpaver {

/// Base class of the hierarchy of representations of real function vectors
class RealFunctionVectorRep {
public:
   /// Default constructor
   RealFunctionVectorRep() = default;

   /// Virtual destructor
   virtual ~RealFunctionVectorRep();

   /// Default copy constructor
   RealFunctionVectorRep(const RealFunctionVectorRep&) = default;

   /// No assignment
   RealFunctionVectorRep&
   operator=(const RealFunctionVectorRep&) = delete;

   /// Returns the scope of this, i.e. the set of variables
   virtual Scope scope() const = 0;

   /// Returns the number of variables in this
   virtual size_t nbVars() const = 0;

   /// Returns the number of functions in this
   virtual size_t nbFuns() const = 0;

   /// Returns the i-th function of this
   virtual RealFunction fun(size_t i) const = 0;

   /**
    * @brief Evaluates this.
    * 
    * val[i] is the result of the evaluation of the i-th function of this at pt
    * and val must have nbFuns() components
    */
   virtual void eval(const RealPoint& pt, RealVector& val) = 0;

   /**
    * @brief Differentiates this (calculates an interval Jacobian matrix).
    * 
    * J is the Jacobian matrix of this at pt such that we have the partial
    * derivative dfi / dxj in the i-th row and j-th column of J.
    * 
    * J must have nbFuns() rows and nbVars() columns.
    */
   virtual void diff(const RealPoint& pt, RealMatrix& J) = 0;

   /**
    * @brief Evaluates and differentiates this.
    * 
    * val[i] is the result of the evaluation of the i-th function of this at pt
    * and val must have nbFuns() components
    * 
    * J is the Jacobian matrix of this on B such that we have the partial
    * derivative dfi / dxj in the i-th row and j-th column of J.
    * 
    * J must have nbFuns() rows and nbVars() columns.
    */
   virtual void evalDiff(const RealPoint& pt, RealVector& val,
                         RealMatrix& J) = 0;

   /**
    * @brief Evaluates this and calculates the violation of the constraints.
    * 
    * val[i] is the result of the evaluation of the i-th function of this at pt
    * and val must have nbFuns() components
    * 
    * viol[i] is the violation of the i-th function / constraint on B
    */
   virtual void violation(const RealPoint& pt, RealVector& val,
                          RealVector& viol) = 0;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Main class of real function vectors.
 * 
 *  This encloses a shared pointer to its representation. It is a lightweight
 *  object that can be copied and assigned.
 */
class RealFunctionVector {
public:
   /// Creates an empty vector
   RealFunctionVector();

   /// Creates a vector corresponding to a DAG
   RealFunctionVector(SharedDag dag);

   /// Creates  a vector corresponding to a DAG built from a list of terms
   RealFunctionVector(const std::initializer_list<Term>& lt);

   /**
    * @brief Constructor.
    * 
    * Creates  a vector corresponding to a DAG built from a list of terms and
    * a list of bounds. For each i, we have lt[i] IN li[i].
    */
   RealFunctionVector(const std::initializer_list<Term>& lt,
                      const std::initializer_list<Interval>& li);

   /// Creates a vector from a list of fonctions
   RealFunctionVector(const std::initializer_list<RealFunction>& lf);

   /// Default copy constructor
   RealFunctionVector(const RealFunctionVector&) = default;

   /// No assignment
   RealFunctionVector& operator=(const RealFunctionVector&) = delete;

   /// Default destructor
   ~RealFunctionVector() = default;

   /// Returns the scope of this, i.e. the set of variables
   Scope scope() const;

   /// Returns the number of variables in this
   size_t nbVars() const;

   /// Returns the number of functions in this
   size_t nbFuns() const;

   /// Returns the i-th function of this
   RealFunction fun(size_t i) const;

   /**
    * @brief Inserts a function at the end.
    * 
    * It may be necessary to switch to another representation if the current
    * one is not a list.
    */
   void addFun(RealFunction f);

   /**
    * @brief Evaluates this.
    * 
    * val[i] is the result of the evaluation of the i-th function of this at pt
    * and val must have nbFuns() components
    */
   void eval(const RealPoint& pt, RealVector& val);

   /**
    * @brief Differentiates this (calculates an interval Jacobian matrix).
    * 
    * J is the Jacobian matrix of this at pt such that we have the partial
    * derivative dfi / dxj in the i-th row and j-th column of J.
    * 
    * J must have nbFuns() rows and nbVars() columns.
    */
   void diff(const RealPoint& pt, RealMatrix& J);

   /**
    * @brief Evaluates and differentiates this.
    * 
    * val[i] is the result of the evaluation of the i-th function of this at pt
    * and val must have nbFuns() components
    * 
    * J is the Jacobian matrix of this on B such that we have the partial
    * derivative dfi / dxj in the i-th row and j-th column of J.
    * 
    * J must have nbFuns() rows and nbVars() columns.
    */
   void evalDiff(const RealPoint& pt, RealVector& val, RealMatrix& J);

   /**
    * @brief Evaluates this and calculates the violation of the constraints.
    * 
    * val[i] is the result of the evaluation of the i-th function of this at pt
    * and val must have nbFuns() components
    * 
    * viol[i] is the violation of the i-th function / constraint on B
    */
   void violation(const RealPoint& pt, RealVector& val, RealVector& viol);

   /// Type of the representation of interval functions vectors
   using SharedRep = std::shared_ptr<RealFunctionVectorRep>;

   /// Returns the representation of this
   SharedRep rep() const;

   /// Constructor
   RealFunctionVector(SharedRep rep);

private:
   SharedRep rep_;
};

/*----------------------------------------------------------------------------*/

/// Vector of real functions reflecting a DAG
class RealFunctionVectorDag : public RealFunctionVectorRep {
public:
   /// Creates a vector corresponding to a DAG
   RealFunctionVectorDag(SharedDag dag);

   /// Creates  a vector corresponding to a DAG built from a list of terms
   RealFunctionVectorDag(const std::initializer_list<Term>& lt);

   /**
    * @brief Constructor.
    * 
    * Creates  a vector corresponding to a DAG built from a list of terms and
    * a list of bounds. For each i, we have lt[i] IN li[i].
    */
   RealFunctionVectorDag(const std::initializer_list<Term>& lt,
                         const std::initializer_list<Interval>& li);

   /// Default copy constructor
   RealFunctionVectorDag(const RealFunctionVectorDag&) = default;

   /// No assignment
   RealFunctionVectorDag&
   operator=(const RealFunctionVectorDag&) = delete;

   /// Default destructor
   ~RealFunctionVectorDag() = default;

   /// Returns the dag enclosed in this
   SharedDag dag() const;

   Scope scope() const override;
   size_t nbVars() const override;
   size_t nbFuns() const override;
   RealFunction fun(size_t i) const override;
   void eval(const RealPoint& pt, RealVector& val) override;
   void diff(const RealPoint& pt, RealMatrix& J) override;
   void evalDiff(const RealPoint& pt, RealVector& val, RealMatrix& J) override;
   void violation(const RealPoint& pt, RealVector& val,
                  RealVector& viol) override;

private:
   SharedDag dag_;
};

/*----------------------------------------------------------------------------*/

/// Vector of real-valued functions based on a list
class RealFunctionVectorList : public RealFunctionVectorRep {
public:
   /// Constructor of an empty function vector
   RealFunctionVectorList();

   /// Constructor
   RealFunctionVectorList(const std::initializer_list<RealFunction>& lf);

   /// Default copy constructor
   RealFunctionVectorList(const RealFunctionVectorList&) = default;

   /// No assignment
   RealFunctionVectorList&
   operator=(const RealFunctionVectorList&) = delete;

   /// Default destructor
   ~RealFunctionVectorList() = default;

   /// Inserts a function at the end
    void addFun(RealFunction f);

   Scope scope() const override;
   size_t nbVars() const override;
   size_t nbFuns() const override;
   RealFunction fun(size_t i) const override;
   void eval(const RealPoint& pt, RealVector& val) override;
   void diff(const RealPoint& pt, RealMatrix& J) override;
   void evalDiff(const RealPoint& pt, RealVector& val, RealMatrix& J) override;
   void violation(const RealPoint& pt, RealVector& val,
                  RealVector& viol) override;

private:
   std::vector<RealFunction> vf_;
   Scope scop_;
};

} // namespace

#endif
