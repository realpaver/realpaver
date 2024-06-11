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
 * @file   Term.hpp
 * @brief  Terms (nonlinear expressions)
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_TERM_HPP
#define REALPAVER_TERM_HPP

#include <set>
#include <unordered_map>
#include <vector>
#include "realpaver/DomainBox.hpp"
#include "realpaver/IntervalBox.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

class TermVisitor;

/// Enumeration of node symbols
enum class NodeSymbol {
   Cst,     ///< constant
   Var,     ///< variable
   Add,     ///< addition
   Sub,     ///< subtraction
   Mul,     ///< multiplication
   Div,     ///< division
   Min,     ///< minimum
   Max,     ///< maximum
   Usb,     ///< unary subtraction
   Abs,     ///< absolute value
   Sgn,     ///< sign
   Sqr,     ///< square
   Sqrt,    ///< square root
   Pow,     ///< power
   Exp,     ///< exponential
   Log,     ///< logarithm
   Cos,     ///< cosine
   Sin,     ///< sine
   Tan,     ///< tangent
   Lin,     ///< linear
   Cosh,    ///< hyperbolic cosine
   Sinh,    ///< hyperbolic sine
   Tanh     ///< hyperbolic tangent
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, NodeSymbol op);

/*----------------------------------------------------------------------------*/

/// Enumeration of priority levels of expression nodes
enum class NodePriority {
   Low,        ///< constants, variables
   AddSub,     ///< add, sub
   MulDiv,     ///< mul, div
   High        ///< others
};

/*----------------------------------------------------------------------------*/

/// Base class of the hierarchy of term nodes
class TermRep {
public:
   /// Constructor
   TermRep(NodeSymbol symb, NodePriority p);

   /// Virtual destructor
   virtual ~TermRep();

   /// Returns the node symbol
   NodeSymbol symbol() const;

   /// Returns the priority of this
   NodePriority priority() const;

   /// Returns the hash code of this
   size_t hashCode() const;

   /// Output on a stream
   virtual void print(std::ostream& os) const = 0;

   /// Returns the interval evaluation of a constant term
   virtual Interval evalConst() const = 0;

   /**
    * @brief Interval evaluation on B.
    * 
    * The result is assigned in the interval value enclosed.
    */
   virtual void eval(const IntervalBox& B) = 0;

   /**
    * @brief Interval evaluation on box.
    * 
    * The result is assigned in the interval value enclosed.
    */
   virtual void eval(const DomainBox& box) = 0;

   /**
    * @brief Contraction method.
    * 
    * Contracts B and returns a certificate of proof
    */
   virtual Proof contract(IntervalBox& B) = 0;

   /**
    * @brief Contraction method.
    * 
    * Contracts box and returns a certificate of proof
    */
   virtual Proof contract(DomainBox& box) = 0;

   /// Visitor pattern
   virtual void acceptVisitor(TermVisitor& vis) const = 0;

   /// Returns true if the root node of this has type TermCst (a number)
   virtual bool isNumber() const;

   /// Returns true if this has one node that is equal to 0
   virtual bool isZero() const;

   /// Returns true if this has one node that is equal to 1
   virtual bool isOne() const;

   /// Returns true if this has one node that is equal to -1
   virtual bool isMinusOne() const;

   /// @return true if the root node of this has type TermVar
   virtual bool isVar() const;

   /// Returns true if the root node of this has type TermAdd
   virtual bool isAdd() const;

   /// Returns true if the root node of this has type TermSub
   virtual bool isSub() const;

   /// Returns true if the root node of this has type TermMul
   virtual bool isMul() const;

   /// Returns true if the root node of this has type TermDiv
   virtual bool isDiv() const;

   /// Returns true if the root node of this has type TermUsb
   virtual bool isUsb() const;

   /// Returns true if the root node of this has type TermLin
   virtual bool isLin() const;

   /// Returns true if this is a linear expression
   virtual bool isLinear() const = 0;

   /// Returns true if this is an integer term
   virtual bool isInteger() const = 0;

   /// Return true if this has no variable (test done in O(1))
   bool isConstant() const;

   /// Returns true if v occurs in this
   virtual bool dependsOn(const Variable& v) const = 0;

   /// Makes the scope of this
   virtual void makeScope(Scope& scop) const = 0;

   /**
    * @brief Cloning.
    * 
    * Returns a new representation such that the root of this is cloned and
    * its sub-terms are shared
    */
   virtual TermRep* cloneRoot() const = 0;

   /// Returns a clone of this (deep copy)
   virtual TermRep* clone() const = 0;

   /// Returns the interval value enclosed
   Interval ival() const;

   /// Assigns the interval value enclosed
   void setIval(const Interval& x);

protected:
   using SharedRep = std::shared_ptr<TermRep>;
   friend class Term;

   NodeSymbol symb_; // symbol
   size_t hcode_;    // hash code
   bool constant_;   // true if this is constant
   Interval ival_;   // used for evaluation

private:
   NodePriority priority_;   // priority
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Main class of terms.
 *
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class Term {
public:
   /// Type of shared representation
   using SharedRep = std::shared_ptr<TermRep>;

   /// @name Constructors
   ///@{

   /// Constructor of a constant term
   Term(double a = 0.0);

   /// Constructor of a constant term
   Term(const Interval& x);

   /// Constructor of a variable term
   Term(Variable v);

   /// Constructor from a representation
   Term(const SharedRep& rep);
   ///@}

   /// Returns the hash code of this
   size_t hashCode() const;

   /// Output on a stream
   void print(std::ostream& os) const;

   /// Returns the interval evaluation of a constant term
   Interval evalConst() const;

   /// Returns the interval evaluation of this on B
   Interval eval(const IntervalBox& B) const;

   /// Returns the interval evaluation of this on box
   Interval eval(const DomainBox& box) const;

   /**
    * @brief Reduction of domains using the HC4 Revise contractor.
    * 
    * @param B domains of variables
    * @param img image or bounds of this considered as a function
    * @return a certificate of proof
    *
    * This algorithm first evaluates the nodes from the leaves to the root
    * (forward phase) and then calculates the projections from the root to
    * the leaves (backward phase).
    */
   Proof contract(IntervalBox& B, const Interval& img);

   /**
    * @brief Reduction of domains using the HC4 Revise contractor.
    * 
    * @param box domains of variables
    * @param img image or bounds of this considered as a function
    * @return a certificate of proof
    *
    * This algorithm first evaluates the nodes from the leaves to the root
    * (forward phase) and then calculates the projections from the root to
    * the leaves (backward phase).
    */
   Proof contract(DomainBox& box, const Interval& img);

   /**
    * @brief Forward phase of the HC4 Revise contractor.
    * 
    * @param B domains of variables
    * @return the interval evaluation of this on B
    */
   Interval hc4ReviseForward(const IntervalBox& B) const;

   /**
    * @brief Forward phase of the HC4 Revise contractor.
    * 
    * @param box domains of variables
    * @return the interval evaluation of this on B
    */
   Interval hc4ReviseForward(const DomainBox& box) const;

   /**
    * @brief Backward phase of the HC4 Revise contractor.
    * 
    * @param B domains of variables
    * @param img image or bounds of this considered as a function
    * @return a certificate of proof
    *
    * Assumes that the forward phase has been executed using hc4ReviseForward.
    */
   Proof hc4ReviseBackward(IntervalBox& B, const Interval& img);

   /**
    * @brief Backward phase of the HC4 Revise contractor.
    * 
    * @param box domains of variables
    * @param img image or bounds of this considered as a function
    * @return a certificate of proof
    *
    * Assumes that the forward phase has been executed using hc4ReviseForward.
    */
   Proof hc4ReviseBackward(DomainBox& box, const Interval& img);

   /// Visitor pattern
   void acceptVisitor(TermVisitor& vis) const;   

   /// Returns true if the root node of this has type TermCst (a number)
   bool isNumber() const;

   /// Returns true if this has one node that is equal to 0
   bool isZero() const;

   /// Returns true if this has one node that is equal to 1
   bool isOne() const;

   /// Returns true if this has one node that is equal to -1
   bool isMinusOne() const;

   /// Returns true if the root node of this has type TermVar
   bool isVar() const;

   /// Returns true if the root node of this has type TermAdd
   bool isAdd() const;

   /// Returns true if the root node of this has type TermSub
   bool isSub() const;

   /// Returns true if the root node of this has type TermMul
   bool isMul() const;

   /// Returns true if the root node of this has type TermDiv
   bool isDiv() const;

   /// Returns true if the root node of this has type TermUsb
   bool isUsb() const;

      /// Returns true if the root node of this has type TermLin
   bool isLin() const;

   /// Returns true if this is a linear expression
   bool isLinear() const;

   /**
    * @brief Returns true if this is an integer term.
    *
    * An integer term has only integer variables, integer constants, and
    * arithmetic operations
    */
   bool isInteger() const;

   /// Returns true if this has no variable (test done in O(1))
   bool isConstant() const;

   /// Returns true if this is a sum of squares
   bool isSumOfSquares() const;

   /// Returns true if v occurs in this
   bool dependsOn(const Variable& v) const;

   /// Inserts the variables of this in a scope
   void makeScope(Scope& scop) const;

   /// Returns creates and returns the scope of this
   Scope scope() const;

   /// Addition with assignment
   Term& operator+=(Term other);

   /// Subtraction with assignment
   Term& operator-=(Term other);

   /// Multiplication with assignment
   Term& operator*=(Term other);

   /// Division with assignment
   Term& operator/=(Term other);
 
   /// Returns the representation of this
   SharedRep rep() const;

   /**
    * @brief Cloning.
    * 
    * Returns a new representation such that the root of this is cloned and
    * its sub-terms are shared
    */
   TermRep* cloneRoot() const;

   /// Returns a clone of this (deep copy)
   Term clone() const;

   /**
    * @brief Returns the simplification status.
    * 
    * Returns true if terms are automatically simplified when they are created
    * and false otherwise
    */
   static bool simplification();

   /**
    * @brief Assigns the simplification status.
    * 
    * @param simplify true in order to simplify terms when they are created,
    *        false otherwise
    * @return the previous status before its assignment
    */
   static bool simplification(bool simplify);

   /**
    * @brief Gets the display status of constants.
    * 
    * Returns true if the constants are displayed as intervals, false
    * if the midpoints are displayed
    */
   static bool idisplay();

   /// Assigns the display status of constants
   static bool idisplay(bool ok);

private:
   SharedRep rep_;         // shared representation

   static bool simplify_;  // true if terms are automatically simplified
   static bool idisplay_;  // true if constants are displayed as intervals
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Term& t);

/// @name Creation of terms
///@{

/// Returns l+r
Term operator+(Term l, Term r);

/// Returns l-r
Term operator-(Term l, Term r);

/// Returns l*r
Term operator*(Term l, Term r);

/// Returns l/r
Term operator/(Term l, Term r);

/// Returns MIN(l, r)
Term MIN(Term l, Term r);

/// Returns MAX(l, r)
Term MAX(Term l, Term r);

/// Returns -t
Term operator-(Term t);

/// Returns abs(t)
Term abs(Term t);

/// Returns sign(t)
Term sgn(Term t);

/// Returns sqr(t)
Term sqr(Term t);

/// Returns sqrt(t)
Term sqrt(Term t);

/// Returns t^n
Term pow(Term t, int n);

/// Returns t^d
Term pow(Term t, double d);

/// Returns t^x
Term pow(Term t, const Interval& x);

/// Returns t^e
Term pow(Term t, Term e);

/// Returns exp(t)
Term exp(Term t);

/// Returns log(t)
Term log(Term t);

/// Returns cos(t)
Term cos(Term t);

/// Returns sin(t)
Term sin(Term t);

/// Returns tan(t)
Term tan(Term t);

/// Returns cosh(t)
Term cosh(Term t);

/// Returns sinh(t)
Term sinh(Term t);

/// Returns tanh(t)
Term tanh(Term t);
///@}

/*----------------------------------------------------------------------------*/

/// Constant node
class TermCst : public TermRep {
public:
   /// Constructor
   TermCst(const Interval& x);

   void print(std::ostream& os) const override;
   Interval evalConst() const override;
   void eval(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   void eval(const DomainBox& box) override;
   Proof contract(DomainBox& box) override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isNumber() const override;
   bool isZero() const override;
   bool isOne() const override;
   bool isMinusOne() const override;
   bool dependsOn(const Variable& v) const override;
   bool isLinear() const override;
   bool isInteger() const override;
   void makeScope(Scope& scop) const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;

   /// Returns the value enclosed
   Interval getVal() const;

private:
   Interval x_;
};

/*----------------------------------------------------------------------------*/

/// Variable node
class TermVar : public TermRep {
public:
   /// Constructor
   /// @param v the variable enclosed
   TermVar(Variable v);

   void print(std::ostream& os) const override;
   Interval evalConst() const override;
   void eval(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   void eval(const DomainBox& box) override;
   Proof contract(DomainBox& box) override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool dependsOn(const Variable& v) const override;
   bool isLinear() const override;
   bool isInteger() const override;
   void makeScope(Scope& scop) const override;
   bool isVar() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;

   /// Returns the variable enclosed
   Variable var() const;

private:
   Variable v_;
};

/*----------------------------------------------------------------------------*/

/// Base class of operation nodes
class TermOp : public TermRep {
public:
   /// Constructor
   TermOp(const SharedRep& t, NodeSymbol symb, NodePriority p);

   /// Constructor
   TermOp(const SharedRep& l, const SharedRep& r, NodeSymbol op, NodePriority p);

   /// Virtual destructor
   virtual ~TermOp();

   /// Returns the number of sub-terms
   size_t arity() const;

   /// Returns the i-th sub-term of this
   SharedRep subTerm(size_t i) const;

   /// Returns the left-hand term for a binary operation
   SharedRep left() const;

   /// Returns the right-hand term for a binary operation
   SharedRep right() const;

   /// Returns the sub-term for a unary operation
   SharedRep child() const;

   /// Inserts a term at the end of the list of sub-terms of this
   void insert(const SharedRep& t);

   bool isAdd() const override;
   bool isSub() const override;
   bool isMul() const override;
   bool isDiv() const override;
   bool isUsb() const override;
   bool isInteger() const override;
   void eval(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   void eval(const DomainBox& box) override;
   Proof contract(DomainBox& box) override;
   virtual void print(std::ostream& os) const override;
   bool dependsOn(const Variable& v) const override;
   virtual bool isLinear() const override;
   void makeScope(Scope& scop) const override;

protected:
   /// Interval evaluation of the root node
   virtual void evalRoot() = 0;

   /// Contraction with respect to the root node
   virtual void contractRoot() = 0;

   /// Returns true if the term rooted by this is an integer term
   virtual bool isIntegerRoot() const;

private:
   std::vector<SharedRep> v_;    // sub-terms
};

/*----------------------------------------------------------------------------*/

/// Binary addition
class TermAdd : public TermOp {
public:
   /// Constructor
   TermAdd(const SharedRep& l, const SharedRep& r);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isLinear() const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Binary subtraction
class TermSub : public TermOp {
public:
   /// Constructor
   TermSub(const SharedRep& l, const SharedRep& r);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isLinear() const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Binary multiplication
class TermMul : public TermOp {
public:
   /// Constructor
   TermMul(const SharedRep& l, const SharedRep& r);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isLinear() const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Binary division
class TermDiv : public TermOp {
public:
   /// Constructor
   TermDiv(const SharedRep& l, const SharedRep& r);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Binary minimum
class TermMin : public TermOp {
public:
   /// Constructor
   TermMin(const SharedRep& l, const SharedRep& r);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Binary maximum
class TermMax : public TermOp {
public:
   /// Constructor
   TermMax(const SharedRep& l, const SharedRep& r);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Unary subtraction
class TermUsb : public TermOp {
public:
   /// Constructor
   TermUsb(const SharedRep& t);

   void print(std::ostream& os) const override;
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isLinear() const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Absolute value
class TermAbs : public TermOp {
public:
   /// Constructor
   TermAbs(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Sign of a term
class TermSgn : public TermOp {
public:
   /// Constructor
   TermSgn(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Square of a term
class TermSqr : public TermOp {
public:
   /// Constructor
   TermSqr(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Square root of a term
class TermSqrt : public TermOp {
public:
   /// Constructor
   TermSqrt(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Power of a term
class TermPow : public TermOp {
public:
   /// Constructor
   TermPow(const SharedRep& t, int n);

   /// Returns the exponent
   int exponent() const;

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isIntegerRoot() const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;

private:
   int n_;
};

/*----------------------------------------------------------------------------*/

/// Exponential of a term
class TermExp : public TermOp {
public:
   /// Constructor
   TermExp(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Logarithm of a term
class TermLog : public TermOp {
public:
   /// Constructor
   TermLog(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Cosine of a term
class TermCos : public TermOp {
public:
   /// Constructor
   TermCos(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Sine of a term
class TermSin : public TermOp {
public:
   /// Constructor
   TermSin(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Tangent of a term
class TermTan : public TermOp {
public:
   /// Constructor
   TermTan(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Linear expression
class TermLin : public TermRep {
public:
   /// Constructor
   TermLin();

   /// Adds a constant to this
   void addConstant(const Interval& val);

   /// Adds the opposite of a constant to this
   void subConstant(const Interval& val);

   /// Adds a product of x*v to this
   void addTerm(const Interval& x, Variable v);

   /// Adds the opposite of x*v to this
   /// @param x constant
   /// @param v variable
   void subTerm(const Interval& x, Variable v);

   /// Adds a linear term to this
   void addTermLin(const TermLin& t);

   /// Adds the opposite of a linear term to this
   void subTermLin(const TermLin& t);

   void print(std::ostream& os) const override;
   Interval evalConst() const override;
   void eval(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   void eval(const DomainBox& box) override;
   Proof contract(DomainBox& box) override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isLin() const override;
   bool isLinear() const override;
   bool isInteger() const override;
   bool dependsOn(const Variable& v) const override;
   void makeScope(Scope& scop) const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;

   /// Returns the constant value of this
   Interval getConstantValue() const;

   /// Returns the number of sub-terms (variables) in this
   size_t getNbSub() const;

   /// Returns the coefficient of the i-th linear sub-term in this
   Interval getCoefSub(size_t i) const;

   /// Returns the variable of the i-th linear sub-term in this
   Variable getVarSub(size_t i) const;

   /// Returns true if this is reduced to a variable
   bool isVariable() const;

   /// Changes the sign of the constant and each coefficient in this
   void toOpposite();

private:
   struct Item {
      Interval coef;    // coefficient
      Variable var;     // variable term
      Interval ival;    // used for the evaluation
   };

   struct CompItem {
      bool operator()(const Item& i1, const Item& i2) const
      {
         return i1.var.id() < i2.var.id();
      }
   };

   void makeHashCode();

public:
   typedef std::set<Item, CompItem>::const_iterator const_iterator;
   const_iterator begin() const;
   const_iterator end() const;
   const_iterator find(const Variable& v) const;

   Interval getCoefSub(const_iterator it) const;
   Variable getVarSub(const_iterator it) const;

private:
   Interval cst_;                      // constant value
   std::set<Item, CompItem> terms_;    // set of linear terms
};

/*----------------------------------------------------------------------------*/

/// Hyperbolic cosine of a term
class TermCosh : public TermOp {
public:
   /// Constructor
   TermCosh(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Hyperbolic sine of a term
class TermSinh : public TermOp {
public:
   /// Constructor
   TermSinh(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Hyperbolic tangent of a term
class TermTanh : public TermOp {
public:
   /// Constructor
   TermTanh(const SharedRep& t);

   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   TermRep* clone() const override;
};

/*----------------------------------------------------------------------------*/

/// Base class of visitors of terms
class TermVisitor {
public:
   /// Virtual destructor
   virtual ~TermVisitor();

   virtual void apply(const TermCst* t);
   virtual void apply(const TermVar* t);
   virtual void apply(const TermAdd* t);
   virtual void apply(const TermSub* t);
   virtual void apply(const TermMul* t);
   virtual void apply(const TermDiv* t);
   virtual void apply(const TermMin* t);
   virtual void apply(const TermMax* t);
   virtual void apply(const TermUsb* t);
   virtual void apply(const TermAbs* t);
   virtual void apply(const TermSgn* t);
   virtual void apply(const TermSqr* t);
   virtual void apply(const TermSqrt* t);
   virtual void apply(const TermPow* t);
   virtual void apply(const TermExp* t);
   virtual void apply(const TermLog* t);
   virtual void apply(const TermCos* t);
   virtual void apply(const TermSin* t);
   virtual void apply(const TermTan* t);
   virtual void apply(const TermLin* t);
   virtual void apply(const TermCosh* t);
   virtual void apply(const TermSinh* t);
   virtual void apply(const TermTanh* t);
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Visitor used to transform (or not) a term into a sum of squares.
 *
 * If the visited term has the form t1^2 + t2^2 + ... + tk^2 then the terms
 * t1^2, t2^2, ..., tk^2 are stored in a vector.
 */
class SumOfSquaresCreator : public TermVisitor {
public:
   /// Constructor
   SumOfSquaresCreator();

   /// Returns true if this has visited a term representing a sum of squares
   bool sumOfSquaresVisited() const;

   /// Returns the number of squares after a visit
   size_t nbSquares() const;

   /// Gets the i-th square in this after a visit
   Term getSquare(size_t i) const;

   void apply(const TermCst* t) override;
   void apply(const TermVar* t) override;
   void apply(const TermAdd* t) override;
   void apply(const TermSub* t) override;
   void apply(const TermMul* t) override;
   void apply(const TermDiv* t) override;
   void apply(const TermMin* t) override;
   void apply(const TermMax* t) override;
   void apply(const TermUsb* t) override;
   void apply(const TermAbs* t) override;
   void apply(const TermSgn* t) override;
   void apply(const TermSqr* t) override;
   void apply(const TermSqrt* t) override;
   void apply(const TermPow* t) override;
   void apply(const TermExp* t) override;
   void apply(const TermLog* t) override;
   void apply(const TermCos* t) override;
   void apply(const TermSin* t) override;
   void apply(const TermTan* t) override;
   void apply(const TermLin* t) override;
   void apply(const TermCosh* t) override;
   void apply(const TermSinh* t) override;
   void apply(const TermTanh* t) override;

private:
   bool sos_;              // true if this has visited a sum of squares
   std::vector<Term> v_;   // vector of squares
};

} // namespace

#endif
