///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LPMODEL_HPP
#define REALPAVER_LPMODEL_HPP

#include <iostream>
#include <memory>
#include <vector>
#include "realpaver/Common.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is the representation of a variable in a LP model.
///
/// A variable has an index (positive integer), a lower bound, an upper bound,
/// a name and a type (continuous or integer). It also stores the value of
/// this variable in an optimal solution.
///////////////////////////////////////////////////////////////////////////////
class LinVarRep {
public:
   /// Constructor
   /// @param index      the variable index
   /// @param lb         the lower bound
   /// @param ub         the upper bound
   /// @param continuous true for a contibuous variable, false otherwise
   /// @param name       the name of this variable
   LinVarRep(int index, double lb, double ub, bool continuous,
             const std::string& name);

   /// Destructor
   ~LinVarRep();

   /// No copy
   LinVarRep(const LinVarRep&) = delete;

   /// No assignment
   LinVarRep& operator=(const LinVarRep&) = delete;

   /// Defines this as a continuous variable
   void setContinuous();

   /// Defines this as an integer variable
   void setInteger();

   /// Sets the lower bound of this
   /// @param lb the new value of the lower bound
   void setLB(double lb);

   /// Sets the upper bound of this
   /// @param ub the new value of the upper bound
   void setUB(double ub);

   /// Sets the name of this
   /// @param name the new name
   void setName(const std::string& name);

   /// Sets the value of this in an optimal solution
   /// @param value the new value in an optimal solution
   void setObjVal(double val);

   /// @return true if this is a continuous variable
   bool isContinuous() const;

   /// @return true if this is an integral variable
   bool isInteger() const;

   /// @return the index of this
   int getIndex() const;

   /// @return the lower bound of this
   double getLB() const;

   /// @return the upper bound of this
   double getUB() const;
   
   /// @return the name of this
   std::string getName() const;

   /// @return the value of this in an optimal solution
   double getObjVal() const;

private:
   int index_;
   double lb_, ub_, objval_;
   std::string name_;
   bool continuous_;
};

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a variable in a LP model.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class LinVar {
public:
   /// Default constructor
   LinVar() = default;

   /// Default copy constructor
   LinVar(const LinVar&) = default;

   /// Default assignment operator
   LinVar& operator=(const LinVar&) = default;

   /// Default destructor
   ~LinVar() = default;

   /// Defines this as a continuous variable
   void setContinuous();

   /// Defines this as an integer variable
   void setInteger();

   /// Sets the lower bound of this
   /// @param lb the new value of the lower bound
   void setLB(double lb);

   /// Sets the upper bound of this
   /// @param ub the new value of the upper bound
   void setUB(double ub);

   /// Sets the name of this
   /// @param name the new name
   void setName(const std::string& name);

   /// Sets the value of this in an optimal solution
   /// @param value the new value in an optimal solution
   void setObjVal(double val) const;

   /// @return true if this is a continuous variable
   bool isContinuous() const;

   /// @return true if this is an integral variable
   bool isInteger() const;

   /// @return the index of this
   int getIndex() const;

   /// @return the lower bound of this
   double getLB() const;

   /// @return the upper bound of this
   double getUB() const;
   
   /// @return the name of this
   std::string getName() const;  

   /// @return the value of this in an optimal solution
   double getObjVal() const;

   friend class LPModel;

private:
   std::shared_ptr<LinVarRep> rep_;

   LinVar(int index, double lb, double ub, bool continuous,
          const std::string& name);
};

///////////////////////////////////////////////////////////////////////////////
/// @brief This is the representation of a linear expression in a LP model.
///
/// A linear expression is a sum of terms ai*xi where ai is a real number
/// and xi is a linear variable.
///////////////////////////////////////////////////////////////////////////////
class LinExprRep {
public:
   /// Creates an empty expression
   LinExprRep() = default;

   /// Default constructor
   ~LinExprRep() = default;

   /// No copy
   LinExprRep(const LinExprRep&) = delete;

   /// No assignment
   LinExprRep& operator=(const LinExprRep&) = delete;

   /// Adds a new term in this
   /// @param a coefficient of the new term
   /// @param v variable of the new term
   ///
   /// The new term is inserted at the end, after the other terms that were
   /// previously inserted.<br>
   /// Assumes that the variable **v** is not already present in this.
   void addTerm(double a, LinVar v);

   /// @return the number of terms in this
   int getNbTerms() const;

   /// @param i an index between 0 and the number of terms excluded
   /// @return the coefficient of the i-th term of this
   double getCoef(int i) const;

   /// @param i an index between 0 the number of terms excluded
   /// @return the index of the variable of the i-th term of this
   int getIndexVar(int i) const;

   /// @return a pointer to the array of coefficients of the terms of this
   double* getCoefs() const;

   /// @return a pointer to the array of variable indexes of the terms of this
   int* getIndexVars() const;

private:
   std::vector<double> coefs_;
   std::vector<int> vars_;
};

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a linear expression in a LP model.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class LinExpr {
public:
   /// Creates an empty expression
   LinExpr();

   /// Creates an expression given a sum of terms
   /// @param lc the list of coefficients of the terms
   /// @param lv the list of variables occurring in the terms
   LinExpr(const std::initializer_list<double>& lc,
           const std::initializer_list<LinVar>& lv);

   /// Default copy constructor
   LinExpr(const LinExpr&) = default;

   /// Default assignment operator
   LinExpr& operator=(const LinExpr&) = default;

   /// Default destructor
   ~LinExpr() = default;

   /// Adds a new term in this
   /// @param a coefficient of the new term
   /// @param v variable of the new term
   ///
   /// The new term is inserted at the end, after the other terms that were
   /// previously inserted.<br>
   /// Assumes that the variable **v** is not already present in this.
   void addTerm(double a, LinVar v);

   /// @return the number of terms in this
   int getNbTerms() const;

   /// @param i an index between 0 and the number of terms excluded
   /// @return the coefficient of the i-th term of this
   double getCoef(int i) const;

   /// @param i an index between 0 the number of terms excluded
   /// @return the index of the variable of the i-th term of this
   int getIndexVar(int i) const;

   /// @return a pointer to the array of coefficients of the terms of this
   double* getCoefs() const;

   /// @return a pointer to the array of variable indexes of the terms of this
   int* getIndexVars() const;

private:
   std::shared_ptr<LinExprRep> rep_;
};

///////////////////////////////////////////////////////////////////////////////
/// @brief This is the representation of a linear constraint in a LP model.
///
/// A linear constraint represents a double inequality constraint of the form
/// lb <= e <= ub where lb is a lower bound, e is a linear expression, and
/// ub is an upper bound.
///////////////////////////////////////////////////////////////////////////////
class LinCtrRep {
public:
   /// Creates a double inequality constraint
   /// @param lb lower bound of this
   /// @param e  expression of this
   /// @param ub upper bound of this
   LinCtrRep(double lb, LinExpr e, double ub);

   /// Creates an inequality constraint
   /// @param lb lower bound of this
   /// @param e  expression of this
   ///
   /// The upper bound is assigned to plus infinity.
   LinCtrRep(double lb, LinExpr e);

   /// Creates an inequality constraint
   /// @param e  expression of this
   /// @param ub upper bound of this
   ///
   /// The lower bound is assigned to minus infinity.
   LinCtrRep(LinExpr e, double ub);

   /// No copy
   LinCtrRep(const LinCtrRep&) = delete;

   /// No assignment
   LinCtrRep& operator=(const LinCtrRep&) = delete;

   /// Default destructor
   ~LinCtrRep() = default;

   /// Sets the lower bound of this
   /// @param lb the new lower bound of this
   void setLB(double lb);

   /// Sets the upper bound of this
   /// @param ub the new upper bound of this
   void setUB(double ub);

   /// @return the expression of this
   LinExpr getExpr() const;

   /// @return the lower bound of this
   double getLB() const;

   /// @return the upper bound of this
   double getUB() const;

   /// @return true is this has the form e <= ub
   bool isLessEqual() const;

   /// @return true is this has the form lb <= e (or e >= lb)
   bool isGreaterEqual() const;

   /// @return true is this has the form e = lb = ub
   bool isEqual() const;

   /// @return true is this has the form lb <= e <= ub with lb != ub
   bool isRange() const;
  
private:
   LinExpr expr_;
   double lb_, ub_;
};

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a linear constraint in a LP model.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class LinCtr {
public:
   /// Creates a double inequality constraint
   /// @param lb lower bound of this
   /// @param e  expression of this
   /// @param ub upper bound of this
   LinCtr(double lb, LinExpr e, double ub);

   /// Creates an inequality constraint
   /// @param lb lower bound of this
   /// @param e  expression of this
   ///
   /// The upper bound is assigned to plus infinity.
   LinCtr(double lb, LinExpr e);

   /// Creates an inequality constraint
   /// @param e  expression of this
   /// @param ub upper bound of this
   ///
   /// The lower bound is assigned to minus infinity.
   LinCtr(LinExpr e, double ub);

   /// Default copy constructor
   LinCtr(const LinCtr&) = default;

   /// Default assignment operator
   LinCtr& operator=(const LinCtr&) = default;

   /// Default destructor
   ~LinCtr() = default;

   /// Sets the lower bound of this
   /// @param lb the new lower bound of this
   void setLB(double lb);

   /// Sets the upper bound of this
   /// @param ub the new upper bound of this
   void setUB(double ub);

   /// @return the expression of this
   LinExpr getExpr() const;

   /// @return the lower bound of this
   double getLB() const;

   /// @return the upper bound of this
   double getUB() const;
  
   /// @return true is this has the form e <= ub
   bool isLessEqual() const;

   /// @return true is this has the form lb <= e (or e >= lb)
   bool isGreaterEqual() const;

   /// @return true is this has the form e = lb = ub
   bool isEqual() const;

   /// @return true is this has the form lb <= e <= ub with lb != ub
   bool isRange() const;

private:
   std::shared_ptr<LinCtrRep> rep_;
};

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a linear program or LP model.
///
/// A LP model has the form
/// > min  cT x,<br>
/// > s.t. L <= Ax <= U, l <= x <= u.
///
/// Any LP solver must be defined as a sub-class overriding the optimization
/// method.
///
/// After an optimization stage, it is possible to get the status as a value
/// of the enum class OptimizationStatus:
/// - Optimal if optimality proven;
/// - Infeasible if infeasibility proven;
/// - StopOnTimeLimit if a time limit is reached;
/// - StopOnIterLimit if a maximum number of iterations is reached;
/// - Other if other status and not optimal.
///
/// If the status is Optimal then the optimal value of the objective function
/// is assigned in this model and the values of the variables in the solution
/// are assigned in the variables.
///
/// It manages the following parameters used to control the optimization
/// process:
/// - a time limit in seconds;
/// - a maximum number of iterations.
///////////////////////////////////////////////////////////////////////////////
class LPModel {
public:
   /// Creates an empty model
   LPModel();

   /// Virtual destructor
   virtual ~LPModel();

   /// No copy
   LPModel(const LPModel&) = delete;

   /// No assignment
   LPModel& operator=(const LPModel&) = delete;

   /// Creates a new linear variable in this
   /// @param lb lower bound of the new variable
   /// @param ub upper bound of the new variable
   /// @param name name of the new variable
   /// @return the new variable
   LinVar makeVar(double lb, double ub, const std::string& name="");

   /// Inserts a new constraint in this
   /// @param lb lower bound of the new constraint
   /// @param e expression of the new constraint
   /// @param ub upper bound of the new constraint
   void addCtr(double lb, LinExpr e, double ub);

   /// Inserts a new constraint in this
   /// @param lb lower bound of the new constraint
   /// @param e expression of the new constraint
   void addCtr(double lb, LinExpr e);

   /// Inserts a new constraint in this
   /// @param e expression of the new constraint
   /// @param ub upper bound of the new constraint
   void addCtr(LinExpr e, double ub);

   /// Sets the objective function in this
   /// @param obj expression of the objective function
   /// @param minimize true for a minimization problem
   void setObj(LinExpr obj, bool minimize = true);

   /// Sets the optimization sense of this
   void setMinimization();

   /// Sets the optimization sense of this
   void setMaximization();

   /// Optimization method
   /// @return true if an optimal solution is found
   ///
   /// The default implementation does nothing.
   virtual bool optimize();

   /// Gets the value of a variable in an optimal solution
   /// @param v a variable of this
   /// @return the value of v in the last computed optimal solution
   double getObjVal(LinVar v) const;

   /// @return the optimal value of the objective function in the last
   ///         computed optimal solution
   double getObjVal() const;

   /// Output on a stream
   friend std::ostream& operator<<(std::ostream& os, const LPModel& model);

   /// @return the number of linear variables in this
   int getNbLinVars() const;

   /// @param i a variable index between 0 the number of variables excluded
   /// @return the i-th variable in this
   LinVar getLinVar(int i) const;  

   /// @return the number of linear constraints in this
   int getNbLinCtrs() const;

   /// @param i a constraint index between 0 the number of constraints excluded
   /// @return the i-th constraint in this
   LinCtr getLinCtr(int i) const;

   /// @return the expression of the objective function of this
   LinExpr getObjExpr() const;

   /// @return true if this is a minimization problem
   bool isMinimization() const;

   /// @return true is this is a maximization problem
   bool isMaximization() const;

   /// Sets a time limit of the optimization method
   /// @param s a time limit in seconds
   void setMaxSeconds(double s);

   /// @return the time limit of the optimization method
   double getMaxSeconds() const;

   /// Sets a limit of iterations of the optimization method
   /// @param n a maximum number of iterations
   void setMaxIterations(int n);

   /// @return the maximum number of iterations of the optimization method
   int getMaxIterations() const;

   /// @return the status of the last optimization stage
   OptimizationStatus getStatus() const;

protected:
   /// Sets the value of the objective function in an optimal solution
   /// @param val the new value of the objective function
   void setObjVal(double val);

   /// Sets the optimization status
   /// @param status the new status
   void setStatus(OptimizationStatus status);

private:
   std::vector<LinVar> vars_;
   std::vector<LinCtr> ctrs_;
   LinExpr obj_;
   bool minimization_;
   double objval_;
   double maxseconds_;
   int maxiter_;
   OptimizationStatus status_;

   void printLinExpr(std::ostream& os, LinExpr e) const;
   void printVars(std::ostream& os) const;
   void printCtrs(std::ostream& os) const;
   void printObj(std::ostream& os) const;
};

} // namespace

#endif