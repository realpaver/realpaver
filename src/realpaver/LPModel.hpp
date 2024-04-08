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
 * @file   LPModel.hpp
 * @brief  Classes for representing linear programs
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#ifndef REALPAVER_LPMODEL_HPP
#define REALPAVER_LPMODEL_HPP

#include <iostream>
#include <memory>
#include <vector>
#include "realpaver/Common.hpp"
#include "realpaver/IntervalMatrix.hpp"
#include "realpaver/IntervalVector.hpp"

namespace realpaver {

/**
 * @brief Representation of a variable in a LP model.
 * 
 * A variable has an index (positive integer), a lower bound, an upper bound,
 * a name and a type (continuous or integer). It also stores the value of
 * this variable in an optimal solution.
 */
class LinVarRep {
public:
   /**
    * @brief Constructor.
    * 
    *  @param index      the variable index
    *  @param lb         the lower bound
    *  @param ub         the upper bound
    *  @param continuous true for a contibuous variable, false otherwise
    *  @param name       the name of this variable
    */
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
   void setLB(double lb);

   /// Sets the upper bound of this
   void setUB(double ub);

   /// Sets the domain of this
   void setDomain(const Interval& x);

   /// Sets the name of this
   void setName(const std::string& name);

   /// Sets the value of this in an optimal solution
   void setObjVal(double val);

   /**
    * @brief Sets the multiplier.
    * 
    * Sets the multiplier of the bound constraint corresponding to this variable
    * in an optimal solution
    */
   void setMultiplier(double val);

   /// @return true if this is a continuous variable
   bool isContinuous() const;

   /// Returns true if this is an integral variable
   bool isInteger() const;

   /// Returns the index of this
   int getIndex() const;

   /// Returns the lower bound of this
   double getLB() const;

   /// Returns the upper bound of this
   double getUB() const;

   /// Returns the domain of this
   Interval getDomain() const;
   
   /// Returns the name of this
   std::string getName() const;

   /// Returns the value of this in an optimal solution
   double getObjVal() const;

   /// Returns the multiplier of the bound constraint in an optimal solution
   double getMultiplier() const;

   /**
    * @brief Fessibility test.
    * 
    * Returns true if the solution of the primal problem is feasible after an
    * an optimization, false otherwise. Interval arithmetic is used to provide
    * a proof certificate.
    */
   bool isPrimalSolutionFeasible() const;

private:
   int index_;
   double lb_, ub_, objval_, mult_;
   std::string name_;
   bool continuous_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Variable in a LP model.
 * 
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class LinVar {
public:
   /// Default constructor
   LinVar();

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
   void setLB(double lb);

   /// Sets the upper bound of this
   void setUB(double ub);

   /// Sets the domain of this
   void setDomain(const Interval& x);

   /// Sets the name of this
   void setName(const std::string& name);

   /// Sets the value of this in an optimal solution
   void setObjVal(double val) const;

   /// Sets the multiplier of the bound constraint in an optimal solution
   void setMultiplier(double val);

   /// Returns true if this is a continuous variable
   bool isContinuous() const;

   /// Returns true if this is an integral variable
   bool isInteger() const;

   /// Returns the index of this
   int getIndex() const;

   /// Returns the lower bound of this
   double getLB() const;

   /// Returns the upper bound of this
   double getUB() const;

   /// Returns the domain of this
   Interval getDomain() const;
   
   /// Returns the name of this
   std::string getName() const;  

   /// Returns the value of this in an optimal solution
   double getObjVal() const;

   /// Returns the multiplier of the bound constraint in an optimal solution
   double getMultiplier() const;

   /**
    * @brief Feasibility test.
    * Returns true if the solution of the primal problem is feasible after an
    * an optimization, false otherwise. Interval arithmetic is used to provide
    * a proof certificate.
    */
   bool isPrimalSolutionFeasible() const;

   friend class LPModel;

private:
   std::shared_ptr<LinVarRep> rep_;

   // Only the friend class can build a variable
   LinVar(int index, double lb, double ub, bool continuous,
          const std::string& name);
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Representation of a linear expression in a LP model.
 * 
 * A linear expression is a sum of terms ai*xi where ai is a real number
 * and xi is a linear variable.
 */
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

   /**
    * @brief Adds a new term a*v in this.
    * 
    * 
    *  The new term is inserted at the end, after the other terms that were
    *  previously inserted.
    *  Assumes that the variable v is not already present in this.
    */
   void addTerm(double a, LinVar v);

   /// Returns the number of terms in this
   int getNbTerms() const;

   /// Returns the coefficient of the i-th term of this
   double getCoef(int i) const;

   /// Returns the index of the variable of the i-th term of this
   int getIndexVar(int i) const;

   /// Returns a pointer to the array of coefficients of the terms of this
   double* getCoefs() const;

   /// Returns a pointer to the array of variable indexes of the terms of this
   int* getIndexVars() const;

   /// Output on a stream
   void print(std::ostream& os) const;

private:
   std::vector<double> coefs_;
   std::vector<int> vars_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Linear expression in a LP model.
 * 
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class LinExpr {
public:
   /// Creates an empty expression
   LinExpr();

   /// Creates an expression given a sum of terms
   LinExpr(const std::initializer_list<double>& lc,
           const std::initializer_list<LinVar>& lv);

   /// Default copy constructor
   LinExpr(const LinExpr&) = default;

   /// Default assignment operator
   LinExpr& operator=(const LinExpr&) = default;

   /// Default destructor
   ~LinExpr() = default;

   /**
    * @brief Adds a new term a*v in this.
    * 
    * 
    *  The new term is inserted at the end, after the other terms that were
    *  previously inserted.
    *  Assumes that the variable v is not already present in this.
    */
   LinExpr& addTerm(double a, LinVar v);

   /// Returns the number of terms in this
   int getNbTerms() const;

   /// Returns the coefficient of the i-th term of this
   double getCoef(int i) const;

   /// Returns the index of the variable of the i-th term of this
   int getIndexVar(int i) const;

   /// Returns a pointer to the array of coefficients of the terms of this
   double* getCoefs() const;

   /// Returns a pointer to the array of variable indexes of the terms of this
   int* getIndexVars() const;

   /// Output on a stream
   friend std::ostream& operator<<(std::ostream& os, const LinExpr& e);

private:
   std::shared_ptr<LinExprRep> rep_;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const LinExpr& e);

/*----------------------------------------------------------------------------*/

/**
 * @brief Representation of a linear constraint in a LP model.
 * 
 * A linear constraint represents a double inequality constraint of the form
 * lb <= e <= ub where lb is a lower bound, e is a linear expression, and
 * ub is an upper bound.
 */
class LinCtrRep {
public:
   /// Creates a double inequality constraint lb <= e <= ub
   LinCtrRep(double lb, LinExpr e, double ub);

   /// Creates an inequality constraint lb <= e
   LinCtrRep(double lb, LinExpr e);

   /// Creates an inequality constraint e <= ub
   LinCtrRep(LinExpr e, double ub);

   /// No copy
   LinCtrRep(const LinCtrRep&) = delete;

   /// No assignment
   LinCtrRep& operator=(const LinCtrRep&) = delete;

   /// Default destructor
   ~LinCtrRep() = default;

   /// Sets the lower bound of this
   void setLB(double lb);

   /// Sets the upper bound of this
   void setUB(double ub);

   /**
    * @brief Sets the multiplier.
    * 
    * Sets the multiplier, i.e. the value of the corresponding dual variabl
    * of this in an optimal solution.
    */
   void setMultiplier(double val);

   /// Returns the expression of this
   LinExpr getExpr() const;

   /// Returns the lower bound of this
   double getLB() const;

   /// Returns the upper bound of this
   double getUB() const;

   /// Returns the multiplier of this in an optimal solution
   double getMultiplier() const;

   /// Returns true is this has the form e <= ub
   bool isLessEqual() const;

   /// Returns true is this has the form lb <= e (or e >= lb)
   bool isGreaterEqual() const;

   /// Returns true is this has the form e = lb = ub
   bool isEqual() const;

   /// Returns true is this has the form lb <= e <= ub with lb != ub
   bool isRange() const;

private:
   LinExpr expr_;
   double lb_, ub_, mult_;
};

/**
 * @brief Linear constraint in a LP model.
 * 
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class LinCtr {
public:
   /// Creates a double inequality constraint lb <= e <= ub
   LinCtr(double lb, LinExpr e, double ub);

   /// Creates an inequality constraint lb <= e
   LinCtr(double lb, LinExpr e);

   /// Creates an inequality constraint e <= ub
   LinCtr(LinExpr e, double ub);

   /// Default copy constructor
   LinCtr(const LinCtr&) = default;

   /// Default assignment operator
   LinCtr& operator=(const LinCtr&) = default;

   /// Default destructor
   ~LinCtr() = default;

   /// Sets the lower bound of this
   void setLB(double lb);

   /// Sets the upper bound of this
   void setUB(double ub);

   /**
    * @brief Sets the multiplier.
    * 
    * Sets the multiplier, i.e. the value of the corresponding dual variable
    * of this in an optimal solution
    */
   void setMultiplier(double val);

   /// Returns the expression of this
   LinExpr getExpr() const;

   /// Returns the lower bound of this
   double getLB() const;

   /// Returns the upper bound of this
   double getUB() const;

   /// Returns the multiplier of this in an optimal solution
   double getMultiplier() const;

   /// Returns true is this has the form e <= ub
   bool isLessEqual() const;

   /// Returns true is this has the form lb <= e (or e >= lb)
   bool isGreaterEqual() const;

   /// Returns true is this has the form e = lb = ub
   bool isEqual() const;

   /// Returns true is this has the form lb <= e <= ub with lb != ub
   bool isRange() const;

private:
   std::shared_ptr<LinCtrRep> rep_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Linear program also called LP model.
 * 
 *
 * A LP model has the form
 * > min  cT x,<br>
 * > s.t. L <= Ax <= U, l <= x <= u.
 *
 * Any LP solver must be defined as a sub-class overriding the optimization
 * method.
 *
 * After an optimization stage, it is possible to get the status as a value
 * of the enum class OptimizationStatus:
 * - Optimal if optimality proven;
 * - Infeasible if infeasibility proven;
 * - StopOnTimeLimit if a time limit is reached;
 * - StopOnIterLimit if a maximum number of iterations is reached;
 * - Other if other status and not optimal.
 *
 * If the status is Optimal then the optimal value of the objective function
 * is assigned in this model and the values of the variables in the solution
 * are assigned in the variables.
 *
 * It manages the following parameters used to control the optimization
 * process:
 * - a time limit in seconds;
 * - a maximum number of iterations.
 */
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

   /// Creates a new linear variable in this given two bounds and a name
   LinVar makeVar(double lb, double ub, const std::string& name="");

   /// Inserts a new constraint in this
   void addCtr(LinCtr c);

   /// Inserts a new constraint lb <= e <= ub in this
   void addCtr(double lb, LinExpr e, double ub);

   /// Inserts a new constraint lb <= e in this
   void addCtr(double lb, LinExpr e);

   /// Inserts a new constraint e <= ub in this
   void addCtr(LinExpr e, double ub);

   /// Sets the objective function in this
   void setObj(LinExpr obj, bool minimize = true);

   /// Sets the optimization sense of this
   void setMinimization();

   /// Sets the optimization sense of this
   void setMaximization();

   /**
    * @brief Optimization method.
    * 
    * Returns true if an optimal solution is found.
    * The default implementation does nothing.
    */
   virtual bool optimize();

   /**
    * @brief Optimization method after a change of the objective function.
    *
    * Returns true if an optimal solution is found
    *
    * Assume that a LP model with objective function F and constraints C
    * has been solved and that we want to solve a new LP model with
    * objective function G and constraints C. Then it suffices to change
    * F by G in the model and to call reoptimize(). It saves much effort
    * since the constraint part is in a solved form.
    *
    * The default implementation does nothing.
    */
   virtual bool reoptimize();

   /// Gets the value of a variable in an optimal solution after an optimization
   double getObjVal(LinVar v) const;

   /// Gets the optimum after an optimization
   double getObjVal() const;

   /**
    * @brief Gets a safe bound of the optimum after a successfull optimization.
    * 
    * Returns a lower bound of the optimum for a minimization problem
    * or an upper bound for a maximization problem
    */
   double getSafeObjVal() const;

   /// Gets a certificate of infeasibility after a non successfull optimization
   bool isSafeInfeasible() const;

   /// Output on a stream
   friend std::ostream& operator<<(std::ostream& os, const LPModel& model);

   /// Returns the number of linear variables in this
   int getNbLinVars() const;

   /// Returns the i-th variable in this
   LinVar getLinVar(int i) const;  

   /// Returns the number of linear constraints in this
   int getNbLinCtrs() const;

   /// Returns the i-th constraint in this
   LinCtr getLinCtr(int i) const;

   /// Returns the expression of the objective function of this
   LinExpr getObjExpr() const;

   /// Returns true if this is a minimization problem
   bool isMinimization() const;

   /// Returns true is this is a maximization problem
   bool isMaximization() const;

   /// Sets a time limit in seconds of the optimization method
   /// @param s a time limit in seconds
   void setMaxSeconds(double s);

   /// Returns the time limit in seconds of the optimization method
   double getMaxSeconds() const;

   /// Sets a limit of iterations of the optimization method
   void setMaxIter(size_t n);

   /// Returns the maximum number of iterations of the optimization method
   size_t getMaxIter() const;

   /// Returns the status of the last optimization stage
   OptimizationStatus getStatus() const;

   /**
    * @brief Feasibility test.
    * 
    * Returns true if the solution of the primal problem is feasible after an
    * an optimization, false otherwise
    *
    * Interval arithmetic is used to provide a proof certificate.
    */
   bool isPrimalSolutionFeasible() const;

protected:
   /// Sets the value of the objective function in an optimal solution
   void setObjVal(double val);

   /// Sets the optimization status
   void setStatus(OptimizationStatus status);

private:
   std::vector<LinVar> vars_;
   std::vector<LinCtr> ctrs_;
   LinExpr obj_;
   bool minimization_;
   double objval_;
   double maxseconds_;
   size_t maxiter_;
   OptimizationStatus status_;

   // display
   void printLinExpr(std::ostream& os, LinExpr e) const;
   void printVars(std::ostream& os) const;
   void printCtrs(std::ostream& os) const;
   void printObj(std::ostream& os) const;

   // safe bounds
   IntervalVector safeX() const;
   IntervalVector safeB() const;
   IntervalVector safeC() const;
   IntervalVector safeL() const;
   IntervalMatrix safeAT() const;
   IntervalVector safeR() const;
   double safeBound() const;
};

} // namespace

#endif
