///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_TERM_HPP
#define REALPAVER_TERM_HPP

#include <unordered_map>
#include <vector>
#include "realpaver/IntervalRegion.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

class TermVisitor;   // forward declaration

///////////////////////////////////////////////////////////////////////////////
/// This is an enumeration of operation symbols.
///////////////////////////////////////////////////////////////////////////////
enum class OpSymbol {
   Add, Sub, Mul, Div, Min, Max, Usb, Abs, Sgn, Sqr, Sqrt, Pow,
   Exp, Log, Cos, Sin, Tan
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, OpSymbol op);

///////////////////////////////////////////////////////////////////////////////
/// This is an enumeration of priority levels of expression nodes.
///////////////////////////////////////////////////////////////////////////////
enum class OpPriority {
   Low,        // constants, variables
   AddSub,     // add, sub
   MulDiv,     // mul, div
   High        // others
};

///////////////////////////////////////////////////////////////////////////////
/// This the base class of the hierarchy of term nodes.
///////////////////////////////////////////////////////////////////////////////
class TermRep {
public:
   /// Creates a term node
   /// @param p priority of this node
   TermRep(OpPriority p);

   /// Virtual destructor
   virtual ~TermRep();

   /// @return the priority of this
   OpPriority priority() const;

   /// @return the hash code of this
   size_t hashCode() const;

   /// Output on a stream
   /// @param os an output stream
   virtual void print(std::ostream& os) const = 0;

   /// @return the interval evaluation of a constant term
   virtual Interval evalConst() const = 0;

   /// Interval evaluation
   /// @param reg domains of variables
   ///
   /// The result is assigned in the interval value enclosed.
   virtual void eval(const IntervalRegion& reg) = 0;

   /// Contraction of domains
   /// @param reg domains of variables
   /// @return a certificate of proof
   virtual Proof contract(IntervalRegion& reg) = 0;

   /// Visitor pattern
   /// @param vis a visitor
   virtual void acceptVisitor(TermVisitor& vis) const = 0;

   ///@{
   /// Tests and operations for numbers used for the simplication of terms
   virtual bool isNumber() const;
   virtual bool isZero() const;
   virtual bool isOne() const;
   virtual bool isMinusOne() const;
   ///@}

   ///@{
   /// Tests
   virtual bool isLinear() const = 0;
   bool isConstant() const;
   virtual bool isVar() const;
   virtual bool isAdd() const;
   virtual bool isSub() const;
   virtual bool isMul() const;
   virtual bool isDiv() const;
   virtual bool isUsb() const;
   ///@}

   /// Dependency test
   /// @param v a variable
   /// @return true if v occurs in this
   virtual bool dependsOn(const Variable& v) const = 0;

   /// Makes the scope of this
   /// @param s the set of variables occurring in this
   virtual void makeScope(Scope& s) const = 0;

   /// @return a new representation such that the root of this is cloned and
   ///         its sub-terms are shared
   virtual TermRep* cloneRoot() const = 0;

   /// @return the interval value enclosed
   Interval ival() const;

   /// Assigns the interval value enclosed
   /// @param x new interval value
   void setIval(const Interval& x);

protected:
   typedef std::shared_ptr<TermRep> SharedRep;
   friend class Term;

   size_t hcode_;
   bool constant_;
   Interval ival_;

private:
   OpPriority priority_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is the main class of terms.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class Term {
public:
   ///@{
   /// Constructors
   Term(double a = 0.0);
   Term(const Interval& x);
   Term(Variable v);
   ///@}

   /// @return the hash code of this
   size_t hashCode() const;

   /// Output on a stream
   /// @param os an output stream
   void print(std::ostream& os) const;

   /// @return the interval evaluation of a constant term
   Interval evalConst() const;

   /// Interval evaluation
   /// @param reg domains of variables
   /// @return the interval evaluation of this at reg
   Interval eval(const IntervalRegion& reg) const;

   /// Reduction of domains using the HC4 Revise contractor
   /// @param reg domains of variables
   /// @param img image or bounds of this considered as a function
   /// @return a certificate of proof
   ///
   /// This algorithm first evaluates the nodes from the leaves to the root
   /// (forward phase) and then calculates the projections from the root to
   /// the leaves (backward phase).
   Proof contract(IntervalRegion& reg, const Interval& img);

   /// Forward phase of the HC4 Revise contractor
   /// @param reg domains of variables
   /// @return the interval evaluation of this at reg
   Interval hc4ReviseForward(const IntervalRegion& reg) const;

   /// Backward phase of the HC4 Revise contractor
   /// @param reg domains of variables
   /// @param img image or bounds of this considered as a function
   /// @return a certificate of proof
   ///
   /// Assumes that the forward phase has been executed using hc4ReviseForward.
   Proof hc4ReviseBackward(IntervalRegion& reg, const Interval& img);

   /// Visitor pattern
   /// @param vis a visitor
   void acceptVisitor(TermVisitor& vis) const;   

   ///@{
   /// Tests and operations for numbers used for the simplication of terms
   bool isNumber() const;
   bool isZero() const;
   bool isOne() const;
   bool isMinusOne() const;
   bool isUsb() const;
   ///@}

   ///@{
   /// Tests
   bool isSumOfSquares() const;
   bool isLinear() const;
   bool isConstant() const;
   bool isVar() const;
   bool isAdd() const;
   bool isSub() const;
   bool isMul() const;
   bool isDiv() const;
   ///@}

   /// Dependency test
   /// @param v a variable
   /// @return true if v occurs in this
   bool dependsOn(const Variable& v) const;

   /// Makes the scope of this
   /// @param s the set of variables occurring in this
   void makeScope(Scope& s) const;

   ///@{
   /// Arithmetic operations with assignment
   Term& operator+=(Term other);
   Term& operator-=(Term other);
   Term& operator*=(Term other);
   Term& operator/=(Term other);
   ///@}

   /// Type of shared representation
   typedef TermRep::SharedRep SharedRep;

   /// Creates a term
   /// @param rep the representation of this
   Term(const SharedRep& rep);

   /// @return the representation of this
   SharedRep rep() const;

   /// @return a new representation such that the root of this is cloned and
   ///         its sub-terms are shared
   TermRep* cloneRoot() const;

   /// @return true if terms are automatically simplified when they are created
   ///         and false otherwise
   static bool simplification();

   /// Assigns the simplification status of terms
   /// @param simplify true in order to simplify terms when they are created,
   ///        false otherwise
   /// @return the previous status before its assignment
   static bool simplification(bool simplify);

   /// Gets the display status of constants
   /// @return true if the constants are displayed as intervals, false
   ///         if the midpoints are displayed
   static bool idisplay();

   /// Assigns the display status of constants
   /// @param ok true in order to display the constants as intervals
   /// @return the previous status before its assignment
   static bool idisplay(bool ok);

private:
   SharedRep rep_;         // shared representation

   static bool simplify_;  // true if terms are automatically simplified
   static bool idisplay_;  // true if constants are displayed as intervals
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, Term t);

///@{
/// Creation of terms
Term operator+(Term l, Term r);
Term operator-(Term l, Term r);
Term operator*(Term l, Term r);
Term operator/(Term l, Term r);
Term MIN(Term l, Term r);
Term MAX(Term l, Term r);
Term operator-(Term t);
Term abs(Term t);
Term sgn(Term t);
Term sqr(Term t);
Term sqrt(Term t);
Term pow(Term t, int n);
Term pow(Term t, double d);
Term pow(Term t, const Interval& x);
Term exp(Term t);
Term log(Term t);
Term cos(Term t);
Term sin(Term t);
Term tan(Term t);
///@}

///////////////////////////////////////////////////////////////////////////////
/// This is a constant node.
///////////////////////////////////////////////////////////////////////////////
class TermConst : public TermRep {
public:
   /// Constructor
   TermConst(const Interval& x);

   ///@{
   void print(std::ostream& os) const override;
   Interval evalConst() const override;
   void eval(const IntervalRegion& reg) override;
   Proof contract(IntervalRegion& reg) override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isZero() const override;
   bool isOne() const override;
   bool isMinusOne() const override;
   bool dependsOn(const Variable& v) const override;
   bool isLinear() const override;
   void makeScope(Scope& s) const override;
   TermRep* cloneRoot() const override;
   ///@}

   /// @returns the value enclosed
   Interval getVal() const;

private:
   Interval x_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a variable node.
///////////////////////////////////////////////////////////////////////////////
class TermVar : public TermRep {
public:
   /// Constructor
   /// @param v the variable enclosed
   TermVar(Variable v);

   ///@{
   void print(std::ostream& os) const override;
   Interval evalConst() const override;
   void eval(const IntervalRegion& reg) override;
   Proof contract(IntervalRegion& reg) override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool dependsOn(const Variable& v) const override;
   bool isLinear() const override;
   void makeScope(Scope& s) const override;
   bool isVar() const override;
   TermRep* cloneRoot() const override;
   ///@}

   /// @return the variable enclosed
   Variable var() const;

private:
   Variable v_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of operation nodes.
///////////////////////////////////////////////////////////////////////////////
class TermOp : public TermRep {
public:
   ///@{
   /// Constructors
   TermOp(const SharedRep& t, OpSymbol op, OpPriority p);
   TermOp(const SharedRep& l, const SharedRep& r, OpSymbol op, OpPriority p);
   ///@}

   /// Virtual destructor
   virtual ~TermOp();

   /// @return the number of sub-terms
   size_t arity() const;

   /// @return the operation symbol
   OpSymbol opSymbol() const;

   /// Access to a sub-term
   /// @param i index of a sub-term between 0 and arity()-1
   /// @return the i-th sub-term of this
   SharedRep subTerm(size_t i) const;

   /// @return the left-hand term for a binary operation
   ///
   ///  equivalent to subTerm(0)
   SharedRep left() const;

   /// @return the right-hand term for a binary operation
   ///
   ///  equivalent to subTerm(1)
   SharedRep right() const;

   /// @return the sub-term for a unary operation
   ///
   ///  equivalent to subTerm(0)
   SharedRep child() const;

   /// Inserts a term at the end of the list of sub-terms of this
   /// @param t a sub-term
   void insert(const SharedRep& t);

   ///@{
   void eval(const IntervalRegion& reg) override;
   Proof contract(IntervalRegion& reg) override;
   virtual void print(std::ostream& os) const override;
   bool dependsOn(const Variable& v) const override;
   virtual bool isLinear() const override;
   void makeScope(Scope& s) const override;
   ///@}

protected:
   virtual void evalRoot() = 0;
   virtual void contractRoot() = 0;

private:
   std::vector<SharedRep> v_;    // sub-terms
   OpSymbol op_;                 // operation symbol
};

///////////////////////////////////////////////////////////////////////////////
/// This is a binary addition.
///////////////////////////////////////////////////////////////////////////////
class TermAdd : public TermOp {
public:
   /// Creates a term with form l + r
   /// @param l left-hand term
   /// @param r right-hand term
   TermAdd(const SharedRep& l, const SharedRep& r);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isLinear() const override;
   bool isAdd() const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a binary subtraction.
///////////////////////////////////////////////////////////////////////////////
class TermSub : public TermOp {
public:
   /// Creates a term with form l - r
   /// @param l left-hand term
   /// @param r right-hand term
   TermSub(const SharedRep& l, const SharedRep& r);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isLinear() const override;
   bool isSub() const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a binary multiplication.
///////////////////////////////////////////////////////////////////////////////
class TermMul : public TermOp {
public:
   /// Creates a term with form l * r
   /// @param l left-hand term
   /// @param r right-hand term
   TermMul(const SharedRep& l, const SharedRep& r);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isLinear() const override;
   bool isMul() const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a binary division.
///////////////////////////////////////////////////////////////////////////////
class TermDiv : public TermOp {
public:
   /// Creates a term with form l / r
   /// @param l left-hand term
   /// @param r right-hand term
   TermDiv(const SharedRep& l, const SharedRep& r);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isDiv() const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a binary minimum.
///////////////////////////////////////////////////////////////////////////////
class TermMin : public TermOp {
public:
   /// Creates a term with form min(l, r)
   /// @param l left-hand term
   /// @param r right-hand term
   TermMin(const SharedRep& l, const SharedRep& r);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a binary maximum.
///////////////////////////////////////////////////////////////////////////////
class TermMax : public TermOp {
public:
   /// Creates a term with form max(l, r)
   /// @param l left-hand term
   /// @param r right-hand term
   TermMax(const SharedRep& l, const SharedRep& r);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a unary subtraction.
///////////////////////////////////////////////////////////////////////////////
class TermUsb : public TermOp {
public:
   /// Creates a term with form -t
   /// @param t sub-term
   TermUsb(const SharedRep& t);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   bool isLinear() const override;
   bool isUsb() const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is an absolute value.
///////////////////////////////////////////////////////////////////////////////
class TermAbs : public TermOp {
public:
   /// Creates a term with form |t|
   /// @param t sub-term
   TermAbs(const SharedRep& t);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is the sign of a term.
///////////////////////////////////////////////////////////////////////////////
class TermSgn : public TermOp {
public:
   /// Creates a term with form sgn(t)
   /// @param t sub-term
   TermSgn(const SharedRep& t);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is the square of a term.
///////////////////////////////////////////////////////////////////////////////
class TermSqr : public TermOp {
public:
   /// Creates a term with form t^2
   /// @param t sub-term
   TermSqr(const SharedRep& t);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is the square root of a term.
///////////////////////////////////////////////////////////////////////////////
class TermSqrt : public TermOp {
public:
   /// Creates a term with form sqrt(t)
   /// @param t sub-term
   TermSqrt(const SharedRep& t);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is the power of a term.
///////////////////////////////////////////////////////////////////////////////
class TermPow : public TermOp {
public:
   /// Creates a term with form t^n
   /// @param t sub-term
   /// @param n exponent
   TermPow(const SharedRep& t, int n);

   /// @return the exponent
   int exponent() const;

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void print(std::ostream& os) const override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}

private:
   int n_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is the exponential of a term.
///////////////////////////////////////////////////////////////////////////////
class TermExp : public TermOp {
public:
   /// Creates a term with form exp(t)
   /// @param t sub-term
   TermExp(const SharedRep& t);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is the logarithm of a term.
///////////////////////////////////////////////////////////////////////////////
class TermLog : public TermOp {
public:
   /// Creates a term with form log(t)
   /// @param t sub-term
   TermLog(const SharedRep& t);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is the cosine of a term.
///////////////////////////////////////////////////////////////////////////////
class TermCos : public TermOp {
public:
   /// Creates a term with form cos(t)
   /// @param t sub-term
   TermCos(const SharedRep& t);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is the sine of a term.
///////////////////////////////////////////////////////////////////////////////
class TermSin : public TermOp {
public:
   /// Creates a term with form sin(t)
   /// @param t sub-term
   TermSin(const SharedRep& t);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is the tangent of a term.
///////////////////////////////////////////////////////////////////////////////
class TermTan : public TermOp {
public:
   /// Creates a term with form tan(t)
   /// @param t sub-term
   TermTan(const SharedRep& t);

   ///@{
   Interval evalConst() const override;
   void evalRoot() override;
   void contractRoot() override;
   void acceptVisitor(TermVisitor& vis) const override;
   TermRep* cloneRoot() const override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of visitors of terms.
///////////////////////////////////////////////////////////////////////////////
class TermVisitor {
public:
   /// Virtul destructor
   virtual ~TermVisitor();

   ///@{
   virtual void apply(const TermConst* t);
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
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor used to transform (or not) a term into a sum of squares.
///
/// If the visited term has the form t1^2 + t2^2 + ... + tk^2 then the terms
/// t1^2, t2^2, ..., tk^2 are stored in a vector.
///////////////////////////////////////////////////////////////////////////////
class SumOfSquaresCreator : public TermVisitor {
public:
   /// Constructor
   SumOfSquaresCreator();

   /// @return true if this has visited a term representing a sum of squares
   bool sumOfSquaresVisited() const;

   /// @return the number of squares after a visit
   size_t nbSquares() const;

   /// Gets a square in this after a visit
   /// @param i an index between 0 and nbSquares()-1
   /// @return the i-th square in this
   Term getSquare(size_t i) const;

   ///@{
   virtual void apply(const TermConst* t) override;
   virtual void apply(const TermVar* t) override;
   virtual void apply(const TermAdd* t) override;
   virtual void apply(const TermSub* t) override;
   virtual void apply(const TermMul* t) override;
   virtual void apply(const TermDiv* t) override;
   virtual void apply(const TermMin* t) override;
   virtual void apply(const TermMax* t) override;
   virtual void apply(const TermUsb* t) override;
   virtual void apply(const TermAbs* t) override;
   virtual void apply(const TermSgn* t) override;
   virtual void apply(const TermSqr* t) override;
   virtual void apply(const TermSqrt* t) override;
   virtual void apply(const TermPow* t) override;
   virtual void apply(const TermExp* t) override;
   virtual void apply(const TermLog* t) override;
   virtual void apply(const TermCos* t) override;
   virtual void apply(const TermSin* t) override;
   virtual void apply(const TermTan* t) override;
   ///@}

private:
   bool sos_;              // true if this has visited a sum of squares
   std::vector<Term> v_;   // vector of squares
};

} // namespace

#endif
