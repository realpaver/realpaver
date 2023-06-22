///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_REAL_FUNCTION_HPP
#define REALPAVER_REAL_FUNCTION_HPP

#include "realpaver/Dag.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of the hierarchy of representations of real
/// functions.
///////////////////////////////////////////////////////////////////////////////
class RealFunctionRep {
public:
   /// Default constructor
   RealFunctionRep() = default;

   /// Default copy constructor
   RealFunctionRep(const RealFunctionRep&) = default;

   /// No assignment
   RealFunctionRep& operator=(const RealFunctionRep&) = delete;

   /// Virtual destructor
   virtual ~RealFunctionRep();

   /// Assigns the image of this
   void setImage(const Interval& img);

   /// @return the image of this
   Interval getImage() const;

   /// @return the scope of this, i.e. the set of variables
   virtual Scope scope() const = 0;

   /// @return the number of arguments of this
   virtual size_t nbVars() const = 0;

   /// Evaluates this
   /// @param pt values of variables
   /// @return value of this at pt
   virtual double eval(const RealPoint& pt) = 0;

   /// Differentiates this
   /// @param pt values of variables
   /// @param grad output vector such that grad[i] if the derivative of this
   /// at pt with respect to the i-th variable of its scope
   virtual void diff(const RealPoint& pt, RealVector& grad) = 0;

   /// Evaluates and differentiates this
   /// @param pt values of variables
   /// @param val result of evaluation of this at pt
   /// @param grad output vector such that grad[i] if the derivative of this
   /// at pt with respect to the i-th variable of its scope
   virtual void evalDiff(const RealPoint& pt, double& val,
                         RealVector& grad) = 0;

   /// Evaluates this and calculates the violation of the underlying constraint
   /// @param pt values of variables
   /// @param val evaluation of this at reg
   /// @param viol 0.0 if the constraint is satisfied, a positive real number
   ///        otherwise equal to the width of the gap between the image of the
   ///        function and the result of its evaluation at pt
   ///
   /// Given [lo, up] the image of this in the DagFun object, the underlying
   /// constraint is defined by lo <= f(x) <= up.
   virtual void violation(const RealPoint& pt, double& val, double& viol) = 0;

   /// Evaluates this and calculates the violation of the underlying constraint
   /// @param pt values of variables
   /// @param lo left bound for this
   /// @param up right bound for this
   /// @param val evaluation of this at pt
   /// @param viol 0.0 if the constraint is satisfied, a positive real number
   ///        otherwise equal to the width of the gap between the image of the
   ///        function and the result of its evaluation at pt
   ///
   /// The underlying constraint is defined by lo <= f(x) <= up.
   virtual void violation(const RealPoint& pt, double lo, double up,
                          double& val, double& viol) = 0;

protected:
   Interval img_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is the main class of real functions.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class RealFunction {
public:
   /// Constructor given an existing dag
   /// @param dag expression graph
   /// @param i index of function in the DAG
   RealFunction(SharedDag dag, size_t i);

   /// Constructor that creates a DAG from a term
   /// @param t a trerm
   /// @param img the image of t
   RealFunction(Term t, const Interval& img = Interval::universe());

   /// Default destructor
   ~RealFunction() = default;

   /// Default copy constructor
   RealFunction(const RealFunction&) = default;

   /// No assignment
   RealFunction& operator=(const RealFunction&) = delete;

   /// Assigns the image of this
   void setImage(const Interval& img);

   /// @return the image of this
   Interval getImage() const;

   /// @return the scope of this, i.e. the set of variables
   Scope scope() const;

   /// @return the number of arguments of this
   size_t nbVars() const;

   /// Evaluates this
   /// @param pt values of variables
   /// @return value of this at pt
   double eval(const RealPoint& pt);

   /// Differentiates this
   /// @param pt values of variables
   /// @param grad output vector such that grad[i] if the derivative of this
   /// at pt with respect to the i-th variable of its scope
   void diff(const RealPoint& pt, RealVector& grad);

   /// Evaluates and differentiates this
   /// @param pt values of variables
   /// @param val result of evaluation of this at pt
   /// @param grad output vector such that grad[i] if the derivative of this
   /// at pt with respect to the i-th variable of its scope
   void evalDiff(const RealPoint& pt, double& val, RealVector& grad);

   /// Evaluates this and calculates the violation of the underlying constraint
   /// @param pt values of variables
   /// @param val evaluation of this at reg
   /// @param viol 0.0 if the constraint is satisfied, a positive real number
   ///        otherwise equal to the width of the gap between the image of the
   ///        function and the result of its evaluation at pt
   ///
   /// Given [lo, up] the image of this in the DagFun object, the underlying
   /// constraint is defined by lo <= f(x) <= up.
   void violation(const RealPoint& pt, double& val, double& viol);

   /// Evaluates this and calculates the violation of the underlying constraint
   /// @param pt values of variables
   /// @param lo left bound for this
   /// @param up right bound for this
   /// @param val evaluation of this at pt
   /// @param viol 0.0 if the constraint is satisfied, a positive real number
   ///        otherwise equal to the width of the gap between the image of the
   ///        function and the result of its evaluation at pt
   ///
   /// The underlying constraint is defined by lo <= f(x) <= up.
   void violation(const RealPoint& pt, double lo, double up, double& val,
                  double& viol);

   /// type of shared pointer to a representation
   typedef std::shared_ptr<RealFunctionRep> SharedRep;

   /// Constructor
   /// @param rep representation of this
   RealFunction(SharedRep rep);

   /// @return the representation of this
   SharedRep rep() const;

private:
   SharedRep rep_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a real-valued function in a DAG.
///
/// This class is an adapter of the DagFun class.
///////////////////////////////////////////////////////////////////////////////
class RealFunctionDag : public RealFunctionRep {
public:
   /// Constructor given an existing dag
   /// @param dag expression graph
   /// @param i index of function in the DAG
   RealFunctionDag(SharedDag dag, size_t i);

   /// Constructor that creates a DAG from a term
   /// @param t a trerm
   /// @param img the image of t
   RealFunctionDag(Term t, const Interval& img = Interval::universe());

   /// Default destructor
   ~RealFunctionDag() = default;

   /// Copy constructor
   RealFunctionDag(const RealFunctionDag&) = default;

   /// No asssignment
   RealFunctionDag& operator=(const RealFunctionDag&) = delete;

   /// @return the dag
   SharedDag dag() const;

   /// @return the function index in the dag
   size_t index() const;

   ///@{
   Scope scope() const override;
   size_t nbVars() const override;
   double eval(const RealPoint& pt) override;
   void diff(const RealPoint& pt, RealVector& grad) override;
   void evalDiff(const RealPoint& pt, double& val, RealVector& grad) override;
   void violation(const RealPoint& pt, double& val, double& viol) override;
   void violation(const RealPoint& pt, double lo, double up, double& val,
                  double& viol) override;
   ///@}

private:
   SharedDag dag_;         // DAG
   size_t index_;          // index of function
};

} // namespace

#endif
