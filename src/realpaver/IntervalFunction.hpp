///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_FUNCTION_HPP
#define REALPAVER_INTERVAL_FUNCTION_HPP

#include "realpaver/Dag.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of the hierarchy of representations of interval
/// functions.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionRep {
public:
   /// Default constructor
   IntervalFunctionRep() = default;

   /// Default copy constructor
   IntervalFunctionRep(const IntervalFunctionRep&) = default;

   /// No assignment
   IntervalFunctionRep& operator=(const IntervalFunctionRep&) = delete;

   /// Virtual destructor
   virtual ~IntervalFunctionRep();

   /// Assigns the image of this
   void setImage(const Interval& img);

   /// @return the image of this
   Interval getImage() const;

   /// @return the scope of this, i.e. the set of variables
   virtual Scope scope() const = 0;

   /// @return the number of arguments of this
   virtual size_t nbVars() const = 0;

   /// Evaluates this
   /// @param reg domains of variables
   /// @return value of this at reg
   virtual Interval eval(const IntervalRegion& reg) = 0;

   /// Evaluates this
   /// @param  pt values of variables
   /// @return value of this at pt
   virtual Interval pointEval(const RealPoint& pt) = 0;

   /// Differentiates this
   /// @param reg domains of variables
   /// @param grad output vector such that grad[i] if the derivative of this
   /// at reg with respect to the i-th variable of its scope
   virtual void diff(const IntervalRegion& reg, IntervalVector& grad) = 0;

   /// Evaluates and differentiates this
   /// @param reg domains of variables
   /// @param val result of evaluation of this at reg
   /// @param grad output vector such that grad[i] if the derivative of this
   /// at reg with respect to the i-th variable of its scope
   virtual void evalDiff(const IntervalRegion& reg, Interval& val,
                         IntervalVector& grad) = 0;

   /// Evaluates this and calculates the violation of the underlying constraint
   /// @param reg domains of variables
   /// @param val evaluation of this at reg
   /// @param viol 0.0 if the constraint is satisfied, a positive real number
   ///        otherwise equal to the width of the gap between the image of the
   ///        function and the result of its evaluation at reg
   ///
   /// Given [lo, up] the image of this in the DagFun object, the underlying
   /// constraint is defined by lo <= f(x) <= up.
   virtual void violation(const IntervalRegion& reg, Interval& val,
                          double& viol) = 0;

   /// Evaluates this and calculates the violation of the underlying constraint
   /// @param reg domains of variables
   /// @param lo left bound for this
   /// @param up right bound for this
   /// @param val evaluation of this at reg
   /// @param viol 0.0 if the constraint is satisfied, a positive real number
   ///        otherwise equal to the width of the gap between the image of the
   ///        function and the result of its evaluation at reg
   ///
   /// The underlying constraint is defined by lo <= f(x) <= up.
   virtual void violation(const IntervalRegion& reg, double lo,
                          double up, Interval& val, double& viol) = 0;

private:
   Interval img_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is the main class of interval functions.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunction {
public:
   /// Constructor given an existing dag
   /// @param dag expression graph
   /// @param i index of function in the DAG
   IntervalFunction(SharedDag dag, size_t i);

   /// Constructor that creates a DAG from a term
   /// @param t a trerm
   /// @param img the image of t
   IntervalFunction(Term t, const Interval& img = Interval::universe());

   /// Default destructor
   ~IntervalFunction() = default;

   /// Default copy constructor
   IntervalFunction(const IntervalFunction&) = default;

   /// No assignment
   IntervalFunction& operator=(const IntervalFunction&) = delete;

   /// Assigns the image of this
   void setImage(const Interval& img);

   /// @return the image of this
   Interval getImage() const;

   /// @return the scope of this, i.e. the set of variables
   Scope scope() const;

   /// @return the number of arguments of this
   size_t nbVars() const;

   /// Evaluates this
   /// @param reg domains of variables
   /// @return value of this at reg
   Interval eval(const IntervalRegion& reg);

   /// Evaluates this
   /// @param  pt values of variables
   /// @return value of this at pt
   Interval pointEval(const RealPoint& pt);

   /// Differentiates this
   /// @param reg domains of variables
   /// @param grad output vector such that grad[i] if the derivative of this
   /// at reg with respect to the i-th variable of its scope
   void diff(const IntervalRegion& reg, IntervalVector& grad);

   /// Evaluates and differentiates this
   /// @param reg domains of variables
   /// @param val result of evaluation of this at reg
   /// @param grad output vector such that grad[i] if the derivative of this
   /// at pt with respect to the i-th variable of its scope
   void evalDiff(const IntervalRegion& reg, Interval& val,
                 IntervalVector& grad);

   /// Calculates the violation of the underlying constraint
   /// @param reg domains of variables
   /// @param val evaluation of this at reg
   /// @param viol 0.0 if the constraint is satisfied, a positive real number
   ///        otherwise equal to the width of the gap between the image of the
   ///        function and the result of its evaluation at reg
   ///
   /// Given [lo, up] the image of this in the DagFun object, the underlying
   /// constraint is defined by lo <= f(x) <= up.
   void violation(const IntervalRegion& reg, Interval& val, double& viol);

   /// Calculates the violation of the underlying constraint
   /// @param reg domains of variables
   /// @param lo left bound for this
   /// @param up right bound for this
   /// @return 0.0 if the constraint is satisfied, a positive real number
   ///         otherwise equal to the width of the gap between the image of the
   ///         function and the result of its evaluation at reg
   ///
   /// The underlying constraint is defined by lo <= f(x) <= up.
   void violation(const IntervalRegion& reg, double lo, double up,
                  Interval& val, double& viol);

   /// type of shared pointer to a representation
   typedef std::shared_ptr<IntervalFunctionRep> SharedRep;

   /// Constructor
   /// @param rep representation of this
   IntervalFunction(SharedRep rep);

   /// @return the representation of this
   SharedRep rep() const;

private:
   SharedRep rep_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is an interval-valued function in a DAG.
///
/// This class is an adapter of the DagFun class.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionDag : public IntervalFunctionRep {
public:
   /// Constructor given an existing dag
   /// @param dag expression graph
   /// @param i index of function in the DAG
   IntervalFunctionDag(SharedDag dag, size_t i);

   /// Constructor that creates a DAG from a term
   /// @param t a trerm
   /// @param img the image of t
   IntervalFunctionDag(Term t, const Interval& img = Interval::universe());

   /// Default destructor
   ~IntervalFunctionDag() = default;

   /// Copy constructor
   IntervalFunctionDag(const IntervalFunctionDag&) = default;

   /// No asssignment
   IntervalFunctionDag& operator=(const IntervalFunctionDag&) = delete;

   /// @return the dag
   SharedDag dag() const;

   /// @return the function index in the dag
   size_t index() const;

   ///@{
   Scope scope() const override;
   size_t nbVars() const override;
   Interval eval(const IntervalRegion& reg) override;
   Interval pointEval(const RealPoint& pt) override;
   void diff(const IntervalRegion& reg, IntervalVector& grad) override;
   void evalDiff(const IntervalRegion& reg, Interval& val,
                 IntervalVector& grad) override;
   void violation(const IntervalRegion& reg, Interval& val,
                  double& viol) override;
   void violation(const IntervalRegion& reg, double lo, double up,
                    Interval& val, double& viol) override;
   ///@}

private:
   SharedDag dag_;         // DAG
   size_t index_;          // index of function
};

} // namespace

#endif
