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
 * @date   2024-4-11
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
   double lb_, ub_;
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

   /// Returns the expression of this
   LinExpr getExpr() const;

   /// Returns the lower bound of this
   double getLB() const;

   /// Returns the upper bound of this
   double getUB() const;

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
   double lb_, ub_;
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

   /// Returns the expression of this
   LinExpr getExpr() const;

   /// Returns the lower bound of this
   double getLB() const;

   /// Returns the upper bound of this
   double getUB() const;

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

/// Certificate returned by solvers of optimization problems
enum class LPStatus {
   Optimal,                   ///< optimum found by the solver
   Infeasible,                ///< infeasible
   Unbounded,                 ///< unbounded
   InfeasibleOrUnbounded,     ///< infeasible or unbounded
   StopOnIterLimit,           ///< iteration limit exceeded
   StopOnTimeLimit,           ///< time limit exceeded
   Other                      ///< other status
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const LPStatus& status);

/*----------------------------------------------------------------------------*/

enum class LPSense {
   Min,  ///< minimize
   Max   ///< maximize
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const LPSense& sense);

/*----------------------------------------------------------------------------*/

/**
 * @brief Linear program also called LP model.
 *
 * A LP model has the form
 * > min  cT x,<br>
 * > s.t. lhs <= Ax <= rhs, lb <= x <= ub.
 *
 * Any LP solver must be defined as a sub-class using a third-party software,
 * e.g. Clp, Highs. The are two optimization methods:
 * - optimize() calls the Simplex algorithm;
 * - reoptimize() is useful after a change of the cost function.
 *
 * These optimization methods return a status. If the status is Optimal it is
 * possible to retrieve several informations:
 * - costSolution(): the value of the cost function;
 * - primalSolution(): the values of the primal (decision) variables;
 * - dualSolution(): the values of the dual variables.
 *
 * The following methods can be used to certify the results using tests
 * from the following article: A. Neumaier and O. Scherbina. Safe bounds
 * in linear and mixed-integer linear programming. Math. Program., Ser. A,
 * 99:283-296, 2004.
 *
 * The method certifiedCostSolution() must be called only when the
 * optimization method returned the status Optimal. It provides a certified
 * value of the cost function, i.e. a valid lower / upper bound for a
 * min / max problem.
 *
 * The method isCertifiedInfeasible() must be called only when the
 * optimization method returned the status Infeasible. It returns true
 * if the problem is proved infeasible, false if no proof is done.
 *
 * The following parameters are used to control the optimization process:
 * - a time limit in seconds;
 * - a maximum number of iterations;
 * - a feasibility tolerance.
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

   /// Inserts a new constraint in this and returns its index
   size_t addCtr(LinCtr c);

   /// Inserts a new constraint lb <= e <= ub in this and returns its index
   size_t addCtr(double lb, LinExpr e, double ub);

   /// Inserts a new constraint lb <= e in this and returns its index
   size_t addCtr(double lb, LinExpr e);

   /// Inserts a new constraint e <= ub in this and returns its index
   size_t addCtr(LinExpr e, double ub);

   /// Sets the cost function in this
   void setCost(LinExpr cost);

   /// Assigns the optimization sense (min or max)
   void setSense(LPSense sense);

   /// Returns the optimization sense (min or max)
   LPSense getSense() const;

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
   LinExpr getCost() const;

   /// Sets a time limit in seconds of the optimization method
   /// @param s a time limit in seconds
   void setMaxSeconds(double s);

   /// Returns the time limit in seconds of the optimization method
   double getMaxSeconds() const;

   /// Sets a limit of iterations of the optimization method
   void setMaxIter(size_t n);

   /// Returns the maximum number of iterations of the optimization method
   size_t getMaxIter() const;

   /// Returns the feasibility tolerance
   double getFeasilityTol() const;

   /// Assigns the feasibility tolerance to tol
   void setFeasilityTol(double tol);

   /**
    * @brief Certification method.
    *
    * Returns a certified lower bound for a minimization problem and
    * a certified upper bound for a maximisation problem. To be called
    * only if the last optimization stage returned the status Optimal.
    */
   double certifiedCostSolution() const;

   /*
    * @brief Certification method.
    *
    * Returns true if this problem is certainly infeasible, false if no
    * proof is done. To be called only if the last optimization stage
    * returned the status Infeasible.
    */
   bool isCertifiedInfeasible() const;

   /**
    * @brief Optimization method.
    *
    * Returns true if an optimal solution is found.
    * The default implementation does nothing.
    */
   virtual LPStatus optimize();

   /**
    * @brief Optimization method.
    *
    * Assumes that we want to solve a sequence of LPs with different
    * cost functions. The first one must be solved by calling optimize().
    * If an optimal solution is found then the next LPs can be solved
    * in two phases: change the cost function with setCost() and then
    * call reoptimize(). This way, only the second phase of the Simplex
    * algorithm may be performed.
    *
    * The default implementation calls optimize().
    */
   virtual LPStatus reoptimize();

   /// Returns the value of the cost function in the solution
   virtual double costSolution() const = 0;

   /// Returns the values of the decision variables in the solution
   virtual RealVector primalSolution() const = 0;

   /**
    * @brief Returns the dual values in the solution.
    *
    * The returned vector has size m+n. The first m values are the
    * reduced costs of the variables and the next n values are the
    * shadow prices of the constraints.
    */
   virtual RealVector dualSolution() const = 0;

   /**
    * @brief Infeasibility test.
    *
    * Tests is an inbfeasible ray can be obtained when the problem
    * is infeasible. Returns true and assigns ray in case of success,
    * returns false otherwise.
    *
    * The vector ray is resized if it is necessary.
    */
   virtual bool infeasibleRay(RealVector& ray) const = 0;

   /**
    * @brief Returns the matrix of constraint coefficients.
    *
    * This matrix has m columns (variables) and m+n rows.
    * The first m rows represent the Identity matrix representing the bound
    * constraints and the next n rows contain the coefficients of the other
    * constraints.
    */
   IntervalMatrix matrix() const;

   /// Returns the vector of coefficients of the cost function
   IntervalVector cost() const;

   /// Returns the vector of variable bounds
   IntervalVector varBounds() const;

   /// Returns the vector of constraint bounds
   IntervalVector ctrBounds() const;

   /**
    * @brief Returns the vector of bounds.
    *
    * This vector has size m+n; The first m values are the variable
    * bounds and the next n values are the constraint bounds.
    */
   IntervalVector bounds() const;

private:
   std::vector<LinVar> vars_;    // vector of variables
   std::vector<LinCtr> ctrs_;    // vector of constraints
   LinExpr cost_;                // cost function
   LPSense sense_;               // optimization sense (min or max)
   double maxseconds_;           // time limit
   size_t maxiter_;              // iteration limit
   double tol_;                  // feasibility tolerance

   // display
   void printLinExpr(std::ostream& os, LinExpr e) const;
   void printVars(std::ostream& os) const;
   void printCtrs(std::ostream& os) const;
   void printCost(std::ostream& os) const;
};

} // namespace

#endif
