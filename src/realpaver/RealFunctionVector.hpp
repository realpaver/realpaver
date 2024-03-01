///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_REAL_FUNCTION_VECTOR_HPP
#define REALPAVER_REAL_FUNCTION_VECTOR_HPP

#include <vector>
#include "realpaver/Dag.hpp"
#include "realpaver/RealFunction.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of the hierarchy of representations of real
/// function vectors.
///////////////////////////////////////////////////////////////////////////////
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

   /// @return the scope of this, i.e. the set of variables
   virtual Scope scope() const = 0;

   /// @return the number of variables in this
   virtual size_t nbVars() const = 0;

   /// @return the number of functions in this
   virtual size_t nbFuns() const = 0;

   /// @return the i-th function of this
   virtual RealFunction fun(size_t i) const = 0;

   /// Evaluates this
   /// @param pt values of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at pt
   ///
   /// val must have nbFuns() components.
   virtual void eval(const RealPoint& pt, RealVector& val) = 0;

   /// Differentiates this (calculates an interval Jacobian matrix)
   /// @param pt values of variables
   /// @param J Jacobian matrix of this at pt such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J must have nbFuns() rows and nbVars() columns.
   virtual void diff(const RealPoint& pt, RealMatrix& J) = 0;

   /// Evaluates and differentiates this
   /// @param pt values of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at pt
   /// @param J Jacobian matrix of this at pt such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J must have nbFuns() rows and nbVars() columns; val must have
   /// nbFuns() components.
   virtual void evalDiff(const RealPoint& pt, RealVector& val,
                         RealMatrix& J) = 0;

   /// Evaluates this and calculates the violation of the constraints
   /// @param pt values of variables
   /// @param val output vector such that val[i] is the evaluation of
   ///        the i-th function at pt
   /// @param viol output vector such that viol[i] is the violation of the
   ///        i-th function / constraint at pt
   virtual void violation(const RealPoint& pt, RealVector& val,
                          RealVector& viol) = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// This is the main of real function vectors.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class RealFunctionVector {
public:
   /// Creates an empty vector
   RealFunctionVector();

   /// Constructor
   /// @param dag a dag whose functions are added in this in the same order
   RealFunctionVector(SharedDag dag);

   /// Constructor that creates a dag
   /// @param lt list of terms inserted in the dag
   RealFunctionVector(const std::initializer_list<Term>& lt);

   /// Constructor that creates a dag
   /// @param lt list of terms inserted in the dag
   /// @param li list of images (bounds) of those terms
   RealFunctionVector(const std::initializer_list<Term>& lt,
                      const std::initializer_list<Interval>& li);

   /// Constructor
   /// @param lf list of functions
   RealFunctionVector(const std::initializer_list<RealFunction>& lf);

   /// Default copy constructor
   RealFunctionVector(const RealFunctionVector&) = default;

   /// No assignment
   RealFunctionVector& operator=(const RealFunctionVector&) = delete;

   /// Default destructor
   ~RealFunctionVector() = default;

   /// @return the scope of this, i.e. the set of variables
   Scope scope() const;

   /// @return the number of variables in this
   size_t nbVars() const;

   /// @return the number of functions in this
   size_t nbFuns() const;

   /// @return the i-th function of this
   RealFunction fun(size_t i) const;

   /// Inserts a function at the end
   /// @param f function inserted
   ///
   /// It may be necessary to switch to another representation if the current
   /// one is not a list.
   void addFun(RealFunction f);

   /// Evaluates this
   /// @param pt values of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at pt
   ///
   /// val must have nbFuns() components.
   void eval(const RealPoint& pt, RealVector& val);

   /// Differentiates this (calculates an interval Jacobian matrix)
   /// @param pt values of variables
   /// @param J Jacobian matrix of this at pt such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J must have nbFuns() rows and nbVars() columns.
   void diff(const RealPoint& pt, RealMatrix& J);

   /// Evaluates and differentiates this
   /// @param pt values of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at pt
   /// @param J Jacobian matrix of this at pt such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J must have nbFuns() rows and nbVars() columns; val must have
   /// nbFuns() components.
   void evalDiff(const RealPoint& pt, RealVector& val, RealMatrix& J);

   /// Evaluates this and calculates the violation of the constraints
   /// @param pt values of variables
   /// @param val output vector such that val[i] is the evaluation of
   ///        the i-th function at pt
   /// @param viol output vector such that viol[i] is the violation of the
   ///        i-th function / constraint at pt
   void violation(const RealPoint& pt, RealVector& val, RealVector& viol);

   /// type of the representation of interval functions vectors
   typedef std::shared_ptr<RealFunctionVectorRep> SharedRep;

   /// @return the representation of this
   SharedRep rep() const;

   /// Constructor
   /// @param rep representation of this
   RealFunctionVector(SharedRep rep);

private:
   SharedRep rep_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a vector of real-valued functions based on a DAG such that
/// all the functions of the DAG are considered.
///////////////////////////////////////////////////////////////////////////////
class RealFunctionVectorDag : public RealFunctionVectorRep {
public:
   /// Constructor
   /// @param dag a dag whose functions are added in this in the same order
   RealFunctionVectorDag(SharedDag dag);

   /// Constructor that creates a dag
   /// @param lt list of terms inserted in the dag
   RealFunctionVectorDag(const std::initializer_list<Term>& lt);

   /// Constructor that creates a dag
   /// @param lt list of terms inserted in the dag
   /// @param li list of images (bounds) of those terms
   RealFunctionVectorDag(const std::initializer_list<Term>& lt,
                         const std::initializer_list<Interval>& li);

   /// Default copy constructor
   RealFunctionVectorDag(const RealFunctionVectorDag&) = default;

   /// No assignment
   RealFunctionVectorDag&
   operator=(const RealFunctionVectorDag&) = delete;

   /// Default destructor
   ~RealFunctionVectorDag() = default;

   /// @return the dag enclosed in this
   SharedDag dag() const;

   ///@{
   Scope scope() const override;
   size_t nbVars() const override;
   size_t nbFuns() const override;
   RealFunction fun(size_t i) const override;
   void eval(const RealPoint& pt, RealVector& val) override;
   void diff(const RealPoint& pt, RealMatrix& J) override;
   void evalDiff(const RealPoint& pt, RealVector& val, RealMatrix& J) override;
   void violation(const RealPoint& pt, RealVector& val,
                  RealVector& viol) override;
   ///@}

private:
   SharedDag dag_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a vector of interval-valued functions based on a a list.
///////////////////////////////////////////////////////////////////////////////
class RealFunctionVectorList : public RealFunctionVectorRep {
public:
   /// Constructor of an empty function vector
   RealFunctionVectorList();

   /// Constructor
   /// @param lf list of functions
   RealFunctionVectorList(const std::initializer_list<RealFunction>& lf);

   /// Default copy constructor
   RealFunctionVectorList(const RealFunctionVectorList&) = default;

   /// No assignment
   RealFunctionVectorList&
   operator=(const RealFunctionVectorList&) = delete;

   /// Default destructor
   ~RealFunctionVectorList() = default;

   /// Inserts a function at the end
   /// @param f function inserted
    void addFun(RealFunction f);

   ///@{
   Scope scope() const override;
   size_t nbVars() const override;
   size_t nbFuns() const override;
   RealFunction fun(size_t i) const override;
   void eval(const RealPoint& pt, RealVector& val) override;
   void diff(const RealPoint& pt, RealMatrix& J) override;
   void evalDiff(const RealPoint& pt, RealVector& val, RealMatrix& J) override;
   void violation(const RealPoint& pt, RealVector& val,
                  RealVector& viol) override;
   ///@}

private:
   std::vector<RealFunction> vf_;
   Scope scop_;
};

} // namespace

#endif
