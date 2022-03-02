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
#include "realpaver/IntervalVector.hpp"
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
std::ostream& operator<<(std::ostream& os, const OpSymbol& s);

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
   /// @param X a vector of variable domains
   /// @return the interval evaluation of this at X
   virtual Interval eval(const IntervalVector& X) const = 0;

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

protected:
   typedef std::shared_ptr<TermRep> SharedRep;
   friend class Term;

   size_t hcode_;
   bool constant_;

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
   Term(const double& a = 0.0);
   Term(const Interval& x);
   Term(const Variable& v);
   ///@}

   /// @return the hash code of this
   size_t hashCode() const;

   /// Output on a stream
   /// @param os an output stream
   void print(std::ostream& os) const;

   /// @return the interval evaluation of a constant term
   Interval evalConst() const;

   /// Interval evaluation
   /// @param X a vector of variable domains
   /// @return the interval evaluation of this at X
   Interval eval(const IntervalVector& X) const;

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
   Term& operator+=(const Term& other);
   Term& operator-=(const Term& other);
   Term& operator*=(const Term& other);
   Term& operator/=(const Term& other);
   ///@}

private:
   typedef TermRep::SharedRep SharedRep;
   SharedRep rep_;   // shared representation

public:
   /// Creates a term
   /// @param rep the representation of this
   Term(const SharedRep& rep);

   /// @return the representation of this
   SharedRep rep() const;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Term& t);

///@{
/// Creation of terms
Term operator+(const Term& l, const Term& r);
Term operator-(const Term& l, const Term& r);
Term operator*(const Term& l, const Term& r);
Term operator/(const Term& l, const Term& r);
Term mini(const Term& l, const Term& r);
Term maxi(const Term& l, const Term& r);
Term operator-(const Term& t);
Term abs(const Term& t);
Term sgn(const Term& t);
Term sqr(const Term& t);
Term sqrt(const Term& t);
Term pow(const Term& t, int n);
Term pow(const Term& t, const double& d);
Term exp(const Term& t);
Term log(const Term& t);
Term cos(const Term& t);
Term sin(const Term& t);
Term tan(const Term& t);
///@}

///////////////////////////////////////////////////////////////////////////////
/// This is a constant node.
///////////////////////////////////////////////////////////////////////////////
class TermConst : public TermRep {
public:
   /// Constructor
   TermConst(const Interval& x);

   ///@{
   /// Overrides
   void print(std::ostream& os) const;
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isZero() const;
   bool isOne() const;
   bool isMinusOne() const;
   bool dependsOn(const Variable& v) const;
   bool isLinear() const;
   void makeScope(Scope& s) const;
   ///@}

   /// @returns the value enclosed
   const Interval& getVal() const;

private:
   Interval x_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a variable node.
///////////////////////////////////////////////////////////////////////////////
class TermVar : public TermRep {
public:
   /// Constructor
   TermVar(const Variable& v);

   ///@{
   /// Overrides
   void print(std::ostream& os) const;
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool dependsOn(const Variable& v) const;
   bool isLinear() const;
   void makeScope(Scope& s) const;
   bool isVar() const;
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
   /// Overrides
   virtual void print(std::ostream& os) const;
   bool dependsOn(const Variable& v) const;
   virtual bool isLinear() const;
   void makeScope(Scope& s) const;
   ///@}

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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void print(std::ostream& os) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isLinear() const;
   bool isAdd() const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void print(std::ostream& os) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isLinear() const;
   bool isSub() const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void print(std::ostream& os) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isLinear() const;
   bool isMul() const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void print(std::ostream& os) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isDiv() const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isLinear() const;
   bool isUsb() const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void print(std::ostream& os) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Overrides
   Interval evalConst() const;
   Interval eval(const IntervalVector& X) const;
   void acceptVisitor(TermVisitor& vis) const;
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
   /// Visit methods
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

} // namespace

#endif
