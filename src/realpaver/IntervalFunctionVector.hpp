///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_FUNCTION_VECTOR_HPP
#define REALPAVER_INTERVAL_FUNCTION_VECTOR_HPP

#include <vector>
#include "realpaver/Dag.hpp"
#include "realpaver/IntervalFunction.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of the hierarchy of representations of interval
/// function vectors.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionVectorRep {
public:
   /// Default constructor
   IntervalFunctionVectorRep() = default;

   /// Virtual destructor
   virtual ~IntervalFunctionVectorRep();

   /// Default copy constructor
   IntervalFunctionVectorRep(const IntervalFunctionVectorRep&) = default;

   /// No assignment
   IntervalFunctionVectorRep&
   operator=(const IntervalFunctionVectorRep&) = delete;

   /// @return the scope of this, i.e. the set of variables
   virtual Scope scope() const = 0;

   /// @return the number of variables in this
   virtual size_t nbVars() const = 0;

   /// @return the number of functions in this
   virtual size_t nbFuns() const = 0;

   /// @return the i-th function of this
   virtual IntervalFunction fun(size_t i) const = 0;

   /// Evaluates this
   /// @param B domains of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at B
   ///
   /// val must have nbFuns() components.
   virtual void eval(const IntervalBox& B, IntervalVector& val) = 0;

   /// Evaluates this
   /// @param pt values of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at pt
   ///
   /// val must have nbFuns() components.
   virtual void pointEval(const RealPoint& pt, IntervalVector& val) = 0;

   /// Differentiates this (calculates an interval Jacobian matrix)
   /// @param B domains of variables
   /// @param J Jacobian matrix of this at B such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J must have nbFuns() rows and nbVars() columns.
   virtual void diff(const IntervalBox& B, IntervalMatrix& J) = 0;

   /// Evaluates and differentiates this
   /// @param B domains of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at B
   /// @param J Jacobian matrix of this at B such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J must have nbFuns() rows and nbVars() columns; val must have
   /// nbFuns() components.
   virtual void evalDiff(const IntervalBox& B, IntervalVector& val,
                         IntervalMatrix& J) = 0;

   /// Evaluates this and calculates the violation of the constraints
   /// @param B domains of variables
   /// @param val output vector such that val[i] is the evaluation of
   ///        the i-th function at B
   /// @param viol output vector such that viol[i] is the violation of the
   ///        i-th function / constraint at B
   virtual void violation(const IntervalBox& B, IntervalVector& val,
                          RealVector& viol) = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// This is the main of interval function vectors.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionVector {
public:
   /// Creates an empty vector
   IntervalFunctionVector();

   /// Constructor
   /// @param dag a dag whose functions are added in this in the same order
   IntervalFunctionVector(SharedDag dag);

   /// Constructor that creates a dag
   /// @param lt list of terms inserted in the dag
   IntervalFunctionVector(const std::initializer_list<Term>& lt);

   /// Constructor that creates a dag
   /// @param lt list of terms inserted in the dag
   /// @param li list of images (bounds) of those terms
   IntervalFunctionVector(const std::initializer_list<Term>& lt,
                          const std::initializer_list<Interval>& li);

   /// Constructor
   /// @param lf list of functions
   IntervalFunctionVector(const std::initializer_list<IntervalFunction>& lf);

   /// Default copy constructor
   IntervalFunctionVector(const IntervalFunctionVector&) = default;

   /// No assignment
   IntervalFunctionVector& operator=(const IntervalFunctionVector&) = delete;

   /// Default destructor
   ~IntervalFunctionVector() = default;

   /// @return the scope of this, i.e. the set of variables
   Scope scope() const;

   /// @return the number of variables in this
   size_t nbVars() const;

   /// @return the number of functions in this
   size_t nbFuns() const;

   /// @return true if nbVars() == nbFuns() and this is non empty
   bool isSquare() const;

   /// @return the i-th function of this
   IntervalFunction fun(size_t i) const;

   /// Inserts a function at the end
   /// @param f function inserted
   ///
   /// It may be necessary to switch to another representation if the current
   /// one is not a list.
   void addFun(IntervalFunction f);

   /// Evaluates this
   /// @param B domains of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at B
   ///
   /// val must have nbFuns() components.
   void eval(const IntervalBox& B, IntervalVector& val);

   /// Evaluates this
   /// @param pt values of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at pt
   ///
   /// val must have nbFuns() components.
   void pointEval(const RealPoint& pt, IntervalVector& val);

   /// Differentiates this (calculates an interval Jacobian matrix)
   /// @param B domains of variables
   /// @param J Jacobian matrix of this at B such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J must have nbFuns() rows and nbVars() columns.
   void diff(const IntervalBox& B, IntervalMatrix& J);

   /// Evaluates and differentiates this
   /// @param B domains of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at B
   /// @param J Jacobian matrix of this at B such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J must have nbFuns() rows and nbVars() columns; val must have
   /// nbFuns() components.
   void evalDiff(const IntervalBox& B, IntervalVector& val, IntervalMatrix& J);

   /// Evaluates this and calculates the violation of the constraints
   /// @param B domains of variables
   /// @param val output vector such that val[i] is the evaluation of
   ///        the i-th function at B
   /// @param viol output vector such that viol[i] is the violation of the
   ///        i-th function / constraint at B
   void violation(const IntervalBox& B, IntervalVector& val, RealVector& viol);

   /// type of the representation of interval functions vectors
   typedef std::shared_ptr<IntervalFunctionVectorRep> SharedRep;

   /// @return the representation of this
   SharedRep rep() const;

   /// Constructor
   /// @param rep representation of this
   IntervalFunctionVector(SharedRep rep);

private:
   SharedRep rep_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a vector of interval-valued functions based on a DAG such that
/// all the functions of the DAG are considered.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionVectorDag : public IntervalFunctionVectorRep {
public:
   /// Constructor
   /// @param dag a dag whose functions are added in this in the same order
   IntervalFunctionVectorDag(SharedDag dag);

   /// Constructor that creates a dag
   /// @param lt list of terms inserted in the dag
   IntervalFunctionVectorDag(const std::initializer_list<Term>& lt);

   /// Constructor that creates a dag
   /// @param lt list of terms inserted in the dag
   /// @param li list of images (bounds) of those terms
   IntervalFunctionVectorDag(const std::initializer_list<Term>& lt,
                             const std::initializer_list<Interval>& li);

   /// Default copy constructor
   IntervalFunctionVectorDag(const IntervalFunctionVectorDag&) = default;

   /// No assignment
   IntervalFunctionVectorDag&
   operator=(const IntervalFunctionVectorDag&) = delete;

   /// Default destructor
   ~IntervalFunctionVectorDag() = default;

   /// @return the dag enclosed in this
   SharedDag dag() const;

   ///@{
   Scope scope() const override;
   size_t nbVars() const override;
   size_t nbFuns() const override;
   IntervalFunction fun(size_t i) const override;
   void eval(const IntervalBox& B, IntervalVector& val) override;
   void pointEval(const RealPoint& pt, IntervalVector& val) override;
   void diff(const IntervalBox& B, IntervalMatrix& J) override;
   void evalDiff(const IntervalBox& B, IntervalVector& val,
                 IntervalMatrix& J) override;
   void violation(const IntervalBox& B, IntervalVector& val,
                  RealVector& viol) override;
   ///@}

private:
   SharedDag dag_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a vector of interval-valued functions based on a a list.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionVectorList : public IntervalFunctionVectorRep {
public:
   /// Constructor of an empty function vector
   IntervalFunctionVectorList();

   /// Constructor
   /// @param lf list of functions
   IntervalFunctionVectorList(const std::initializer_list<IntervalFunction>& lf);

   /// Default copy constructor
   IntervalFunctionVectorList(const IntervalFunctionVectorList&) = default;

   /// No assignment
   IntervalFunctionVectorList&
   operator=(const IntervalFunctionVectorList&) = delete;

   /// Default destructor
   ~IntervalFunctionVectorList() = default;

   /// Inserts a function at the end
   /// @param f function inserted
   void addFun(IntervalFunction f);

   ///@{
   Scope scope() const override;
   size_t nbVars() const override;
   size_t nbFuns() const override;
   IntervalFunction fun(size_t i) const override;
   void eval(const IntervalBox& B, IntervalVector& val) override;
   void pointEval(const RealPoint& pt, IntervalVector& val) override;
   void diff(const IntervalBox& B, IntervalMatrix& J) override;
   void evalDiff(const IntervalBox& B, IntervalVector& val,
                 IntervalMatrix& J) override;
   void violation(const IntervalBox& B, IntervalVector& val,
                  RealVector& viol) override;
   ///@}

private:
   std::vector<IntervalFunction> vf_;
   Scope scope_;
};

} // namespace

#endif
