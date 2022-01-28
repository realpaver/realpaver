// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_TERM_HPP
#define REALPAVER_TERM_HPP

#include <unordered_map>
#include <vector>
#include "realpaver/box.hpp"
#include "realpaver/scope.hpp"

namespace realpaver {

class TermVisitor;   // forward declaration

/*****************************************************************************
 * Operation symbols.
 *****************************************************************************/
enum class OpSymbol {
   Add, Sub, Mul, Div, Min, Max, Usb, Abs, Sgn, Sqr, Sqrt, Pow,
   Exp, Log, Cos, Sin, Tan
};

std::ostream& operator<<(std::ostream& os, const OpSymbol& s);

/*****************************************************************************
 * Priority levels of expression nodes.
 *****************************************************************************/
enum class OpPriority {
   Low,        // constants, variables
   AddSub,     // add, sub
   MulDiv,     // mul, div
   High        // others
};

/*****************************************************************************
 * Base class of the hierarchy of expression nodes.
 *****************************************************************************/
class TermRep {
public:
   // constructor
   TermRep(OpPriority p);

   // destructor
   virtual ~TermRep();

   // priority
   OpPriority priority() const;

   // hash code
   size_t hashCode() const;

   // output on a stream
   virtual void print(std::ostream& os) const = 0;

   // interval evaluation of a constant term
   virtual Interval evalConst() const = 0;

   // interval evaluation of a term
   virtual Interval eval(const Box& B) const = 0;

   // visitor pattern
   virtual void acceptVisitor(TermVisitor& vis) const = 0;

   // tests and operations for numbers used for the simplication of terms
   virtual bool isNumber() const;
   virtual bool isZero() const;
   virtual bool isOne() const;
   virtual bool isMinusOne() const;

   // other tests
   virtual bool isLinear() const = 0;
   bool isConstant() const;
   virtual bool isAdd() const;
   virtual bool isSub() const;
   virtual bool isMul() const;
   virtual bool isDiv() const;

   // returns true if this term depends on v
   virtual bool dependsOn(const Variable& v) const = 0;

   // makes the scope of this term
   virtual void makeScope(Scope& s) const = 0;

protected:
   typedef std::shared_ptr<TermRep> SharedRep;
   friend class Term;

   size_t hcode_;
   bool constant_;

private:
   OpPriority priority_;
};

inline OpPriority TermRep::priority() const
{
   return priority_;
}

inline size_t TermRep::hashCode() const
{
   return hcode_;
}

inline bool TermRep::isConstant() const
{
   return constant_;
}

/*****************************************************************************
 * Main class of expressions.
 *****************************************************************************/
class Term {
public:
   // constructors
   Term(const double& a = 0.0);
   Term(const Interval& x);
   Term(const Variable& v);

   // hash code
   size_t hashCode() const;

   // output on a stream
   void print(std::ostream& os) const;

   // interval evaluation of a constant term
   Interval evalConst() const;

   // interval evaluation of a term
   Interval eval(const Box& B) const;

   // visitor pattern
   void acceptVisitor(TermVisitor& vis) const;   

   // tests and operations for numbers
   bool isNumber() const;
   bool isZero() const;
   bool isOne() const;
   bool isMinusOne() const;

   // other tests
   bool isLinear() const;
   bool isConstant() const;
   bool isAdd() const;
   bool isSub() const;
   bool isMul() const;
   bool isDiv() const;

   // returns true if this term depends on v
   bool dependsOn(const Variable& v) const;

   // makes the scope of this term
   void makeScope(Scope& s) const;

   // arithmetic operations with assignment
   Term& operator+=(const Term& other);
   Term& operator-=(const Term& other);
   Term& operator*=(const Term& other);
   Term& operator/=(const Term& other);

private:
   typedef TermRep::SharedRep SharedRep;
   SharedRep rep_;   // shared representation

public:
   // constructor
   Term(const SharedRep& rep);

   // returns the representation of this term
   SharedRep rep() const;
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const Term& t);

// creation of terms
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

inline size_t Term::hashCode() const
{
   return rep_->hashCode();
}

inline void Term::print(std::ostream& os) const
{
   rep_->print(os);
}

inline Interval Term::evalConst() const
{
   return rep_->evalConst();
}

inline Interval Term::eval(const Box& B) const
{
   return rep_->eval(B);
}

inline void Term::acceptVisitor(TermVisitor& vis) const
{
   rep_->acceptVisitor(vis);
}

inline bool Term::dependsOn(const Variable& v) const
{
   return rep_->dependsOn(v);
}

inline void Term::makeScope(Scope& s) const
{
   rep_->makeScope(s);
}

inline Term::SharedRep Term::rep() const
{
   return rep_;
}

inline bool Term::isZero() const
{
   return rep_->isZero();
}

inline bool Term::isOne() const
{
   return rep_->isOne();
}

inline bool Term::isMinusOne() const
{
   return rep_->isMinusOne();
}

inline bool Term::isNumber() const
{
   return rep_->isNumber();
}

inline bool Term::isConstant() const
{
   return rep_->isConstant();
}

inline bool Term::isLinear() const
{
   return rep_->isLinear();
}

inline bool Term::isAdd() const
{
   return rep_->isAdd();
}

inline bool Term::isSub() const
{
   return rep_->isSub();
}

inline bool Term::isMul() const
{
   return rep_->isMul();
}

inline bool Term::isDiv() const
{
   return rep_->isDiv();
}

/*****************************************************************************
 * Class of constant nodes.
 *****************************************************************************/
class TermConst : public TermRep {
public:
   // constructor
   TermConst(const Interval& x);

   // Override
   void print(std::ostream& os) const;
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isZero() const;
   bool isOne() const;
   bool isMinusOne() const;
   bool dependsOn(const Variable& v) const;
   bool isLinear() const;
   void makeScope(Scope& s) const;

   // returns the value enclosed
   const Interval& getVal() const;

private:
   Interval x_;
};

inline const Interval& TermConst::getVal() const
{
   return x_;
}

/*****************************************************************************
 * Class of variable nodes.
 *****************************************************************************/
class TermVar : public TermRep {
public:
   // constructor
   TermVar(const Variable& v);

   // Override
   void print(std::ostream& os) const;
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool dependsOn(const Variable& v) const;
   bool isLinear() const;
   void makeScope(Scope& s) const;

   // returns the variable enclosed
   Variable var() const;

private:
   Variable v_;
};

inline Variable TermVar::var() const
{
   return v_;
}

/*****************************************************************************
 * Base class of operation nodes.
 *****************************************************************************/
class TermOp : public TermRep {
public:
   // constructors
   TermOp(const SharedRep& t, OpSymbol op, OpPriority p);
   TermOp(const SharedRep& l, const SharedRep& r, OpSymbol op, OpPriority p);

   // destructor
   virtual ~TermOp();

   // number of sub-terms
   size_t arity() const;

   // operation symbol
   OpSymbol opSymbol() const;

   // gets the i-th sub-term
   SharedRep subTerm(size_t i) const;

   // equivalent to subTerm(0) for a binary operation
   SharedRep left() const;

   // equivalent to subTerm(1) for a binary operation
   SharedRep right() const;

   // equivalent to subTerm(0) for unary operation
   SharedRep sub() const;

   // inserts a term at the end of the list of sub-terms
   void insert(const SharedRep& t);

   // Override
   virtual void print(std::ostream& os) const;
   bool dependsOn(const Variable& v) const;
   virtual bool isLinear() const;
   void makeScope(Scope& s) const;

private:
   std::vector<SharedRep> v_;    // sub-terms
   OpSymbol op_;                 // operation symbol
};

inline size_t TermOp::arity() const
{
   return v_.size();
}

inline OpSymbol TermOp::opSymbol() const
{
   return op_;
}

inline TermRep::SharedRep TermOp::subTerm(size_t i) const
{
   ASSERT(i < arity(), "access out of range to a term operand");

   return v_[i];
}

inline TermRep::SharedRep TermOp::left() const
{
   ASSERT(arity() == 2, "access out of range to a term operand");

   return v_[0];
}

inline TermRep::SharedRep TermOp::right() const
{
   ASSERT(arity() == 2, "access out of range to a term operand");

   return v_[1];
}

inline TermRep::SharedRep TermOp::sub() const
{
   ASSERT(arity() == 1, "access out of range to a term operand");

   return v_[0];
}

/*****************************************************************************
 * Class of nodes: binary addition.
 *****************************************************************************/
class TermAdd : public TermOp {
public:
   TermAdd(const SharedRep& l, const SharedRep& r);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void print(std::ostream& os) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isLinear() const;
   bool isAdd() const;
};

/*****************************************************************************
 * Class of nodes: subtraction.
 *****************************************************************************/
class TermSub : public TermOp {
public:
   TermSub(const SharedRep& l, const SharedRep& r);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void print(std::ostream& os) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isLinear() const;
   bool isSub() const;
};

/*****************************************************************************
 * Class of nodes: binary multiplication.
 *****************************************************************************/
class TermMul : public TermOp {
public:
   TermMul(const SharedRep& l, const SharedRep& r);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void print(std::ostream& os) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isLinear() const;
   bool isMul() const;
};

/*****************************************************************************
 * Class of nodes: division.
 *****************************************************************************/
class TermDiv : public TermOp {
public:
   TermDiv(const SharedRep& l, const SharedRep& r);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void print(std::ostream& os) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isDiv() const;
};

/*****************************************************************************
 * Class of nodes: binary minimum.
 *****************************************************************************/
class TermMin : public TermOp {
public:
   TermMin(const SharedRep& l, const SharedRep& r);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

/*****************************************************************************
 * Class of nodes: binary maximum.
 *****************************************************************************/
class TermMax : public TermOp {
public:
   TermMax(const SharedRep& l, const SharedRep& r);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

/*****************************************************************************
 * Class of nodes: unary subtraction.
 *****************************************************************************/
class TermUsb : public TermOp {
public:
   // constructor
   TermUsb(const SharedRep& t);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
   bool isLinear() const;
};

/*****************************************************************************
 * Class of nodes: absolute value.
 *****************************************************************************/
class TermAbs : public TermOp {
public:
   TermAbs(const SharedRep& t);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

/*****************************************************************************
 * Class of nodes: sign function.
 *****************************************************************************/
class TermSgn : public TermOp {
public:
   TermSgn(const SharedRep& t);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

/*****************************************************************************
 * Class of nodes: square.
 *****************************************************************************/
class TermSqr : public TermOp {
public:
   TermSqr(const SharedRep& t);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

/*****************************************************************************
 * Class of nodes: square root.
 *****************************************************************************/
class TermSqrt : public TermOp {
public:
   TermSqrt(const SharedRep& t);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

/*****************************************************************************
 * Class of nodes: power function with an integral exponent.
 *****************************************************************************/
class TermPow : public TermOp {
public:
   TermPow(const SharedRep& t, int n);

   int exponent() const;

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void print(std::ostream& os) const;
   void acceptVisitor(TermVisitor& vis) const;

private:
   int n_;
};

inline int TermPow::exponent() const
{
   return n_;
}

/*****************************************************************************
 * Class of nodes: exponential function.
 *****************************************************************************/
class TermExp : public TermOp {
public:
   TermExp(const SharedRep& t);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

 /*****************************************************************************
 * Class of nodes: logarithm function.
 *****************************************************************************/
class TermLog : public TermOp {
public:
   TermLog(const SharedRep& t);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

/*****************************************************************************
 * Class of nodes: cosine function.
 *****************************************************************************/
class TermCos : public TermOp {
public:
   TermCos(const SharedRep& t);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

/*****************************************************************************
 * Class of nodes: sine function.
 *****************************************************************************/
class TermSin : public TermOp {
public:
   TermSin(const SharedRep& t);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

/*****************************************************************************
 * Class of nodes: tangent function.
 *****************************************************************************/
class TermTan : public TermOp {
public:
   TermTan(const SharedRep& t);

   // Override
   Interval evalConst() const;
   Interval eval(const Box& B) const;
   void acceptVisitor(TermVisitor& vis) const;
};

/*****************************************************************************
 * Base class of term visitors.
 *****************************************************************************/
class TermVisitor {
public:
   // destructor
   virtual ~TermVisitor();

   // visit methods
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
};

} // namespace

#endif
