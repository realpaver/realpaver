// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONSTRAINT_HPP
#define REALPAVER_CONSTRAINT_HPP

#include "realpaver/term.hpp"

namespace realpaver {

class ConstraintVisitor;   // forward declaration

/*****************************************************************************
 * Relation symbols.
 *****************************************************************************/
enum class RelSymbol {
   Eq, Le, Lt, Ge, Gt, In
};

std::ostream& operator<<(std::ostream& os, const RelSymbol& s);

/*****************************************************************************
 * Base class of the hierarchy of constraints.
 *****************************************************************************/
class ConstraintRep {
public:
   // constructor
   ConstraintRep();

   // destructor
   virtual ~ConstraintRep();

   // hash code
   size_t hashCode() const;

   // scope
   Scope scope() const;

   // bitset
   const Bitset& bitset() const;

   // returns true if this constraint is variable free
   virtual bool isConstant() const = 0;

   // satisfaction test
   virtual Proof isSat(const Box& B) const = 0;

   // output on a stream
   virtual void print(std::ostream& os) const = 0;

   // visitor pattern
   virtual void acceptVisitor(ConstraintVisitor& vis) const = 0;

   // returns true if this constraint depends on v
   virtual bool dependsOn(const Variable& v) const = 0;

   // tests (default: false)
   virtual bool isEquation() const;
   virtual bool isInequality() const;
   virtual bool isLinear() const;

protected:
   typedef std::shared_ptr<ConstraintRep> SharedRep;
   friend class Constraint;

   // scope assignment
   void setScope(const Scope& s);

private:
   Scope scope_;
   Bitset bs_;

protected:
   size_t hcode_;
};

inline size_t ConstraintRep::hashCode() const
{
   return hcode_;
}

inline const Bitset& ConstraintRep::bitset() const
{
   return bs_;
}

inline Scope ConstraintRep::scope() const
{
   return scope_;
}

/*****************************************************************************
 * Main class of constraints.
 *****************************************************************************/
class Constraint {
public:
   // hash code
   size_t hashCode() const;

   // scope
   Scope scope() const;

   // bitset
   const Bitset& bitset() const;

   // returns true if this constraint is variable free
   bool isConstant() const;

   // satisfaction test
   Proof isSat(const Box& B) const;

   // output on a stream
   void print(std::ostream& os) const;

   // visitor pattern
   void acceptVisitor(ConstraintVisitor& vis) const;

   // returns true if this constraint depends on v
   bool dependsOn(const Variable& v) const;

   // tests
   bool isEquation() const;
   bool isInequality() const;
   bool isLinear() const;

private:
   typedef ConstraintRep::SharedRep SharedRep;
   SharedRep rep_;   // shared representation

public:
   // constructor
   Constraint(const SharedRep& rep);

   // returns the representation of this constraint
   SharedRep rep() const;
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const Constraint& c);

inline Constraint::SharedRep Constraint::rep() const
{
   return rep_;
}

inline size_t Constraint::hashCode() const
{
   return rep_->hashCode();
}

inline Scope Constraint::scope() const
{
   return rep_->scope();
}

inline const Bitset& Constraint::bitset() const
{
   return rep_->bitset();
}

inline bool Constraint::isConstant() const
{
   return rep_->isConstant();
}

inline void Constraint::print(std::ostream& os) const
{
   rep_->print(os);
}

inline void Constraint::acceptVisitor(ConstraintVisitor& vis) const
{
   rep_->acceptVisitor(vis);
}

inline Proof Constraint::isSat(const Box& B) const
{
   return rep_->isSat(B);
}

inline bool Constraint::dependsOn(const Variable& v) const
{
   return rep_->dependsOn(v);
}

inline bool Constraint::isEquation() const
{
   return rep_->isEquation();
}

inline bool Constraint::isInequality() const
{
   return rep_->isInequality();
}

inline bool Constraint::isLinear() const
{
   return rep_->isLinear();
}

/*****************************************************************************
 * Base class of constraints between two terms.
 *****************************************************************************/
class ConstraintBin : public ConstraintRep {
public:
   // constructor
   ConstraintBin(const Term& l, const Term& r, RelSymbol rel);

   // destructor
   virtual ~ConstraintBin();

   // returns the left term
   Term left() const;

   // returns the right term
   Term right() const;

   // returns the relation symbol
   RelSymbol relSymbol() const;

   // Override
   void print(std::ostream& os) const;
   bool isConstant() const;
   bool dependsOn(const Variable& v) const;
   bool isEquation() const;
   bool isInequality() const;
   bool isLinear() const;

private:
   Term l_, r_;
   RelSymbol rel_;
};

inline Term ConstraintBin::left() const
{
   return l_;
}

inline Term ConstraintBin::right() const
{
   return r_;
}

inline RelSymbol ConstraintBin::relSymbol() const
{
   return rel_;
}

inline bool ConstraintBin::isConstant() const
{
   return l_.isConstant() && r_.isConstant();
}

/*****************************************************************************
 * Class of constraints: equation 'l = r'.
 *****************************************************************************/
class ConstraintEq : public ConstraintBin {
public:
   ConstraintEq(const Term& l, const Term& r);
   
   // Override
   void acceptVisitor(ConstraintVisitor& vis) const;
   Proof isSat(const Box& B) const;
};

Constraint operator==(const Term& l, const Term& r);

/*****************************************************************************
 * Class of constraints: inequality constraint 'l <= r'.
 *****************************************************************************/
class ConstraintLe : public ConstraintBin {
public:
   ConstraintLe(const Term& l, const Term& r);
   
   // Override
   void acceptVisitor(ConstraintVisitor& vis) const;
   Proof isSat(const Box& B) const;
};

Constraint operator<=(const Term& l, const Term& r);

/*****************************************************************************
 * Class of constraints: inequality constraint 'l < r'.
 *****************************************************************************/
class ConstraintLt : public ConstraintBin {
public:
   ConstraintLt(const Term& l, const Term& r);
   
   // Override
   void acceptVisitor(ConstraintVisitor& vis) const;
   Proof isSat(const Box& B) const;
};

Constraint operator< (const Term& l, const Term& r);

/*****************************************************************************
 * Class of constraints: inequality constraint 'l >= r'.
 *****************************************************************************/
class ConstraintGe : public ConstraintBin {
public:
   ConstraintGe(const Term& l, const Term& r);
   
   // Override
   void acceptVisitor(ConstraintVisitor& vis) const;
   Proof isSat(const Box& B) const;
};

Constraint operator>=(const Term& l, const Term& r);

/*****************************************************************************
 * Class of constraints: inequality constraint 'l > r'.
 *****************************************************************************/
class ConstraintGt : public ConstraintBin {
public:
   ConstraintGt(const Term& l, const Term& r);

   // Override
   void acceptVisitor(ConstraintVisitor& vis) const;
   Proof isSat(const Box& B) const;
};

Constraint operator>(const Term& l, const Term& r);

/*****************************************************************************
 * Class of constraints: membership constraint 'f in I'.
 *****************************************************************************/
class ConstraintIn : public ConstraintBin {
public:
   // constructor
   ConstraintIn(const Term& t, const Interval& x);

   // returns the interval
   const Interval& image() const;

   // returns the term
   Term term() const;

   // Override
   void acceptVisitor(ConstraintVisitor& vis) const;
   Proof isSat(const Box& B) const;

private:
   Interval x_;
};

Constraint in(const Term& l, const Interval& x);
Constraint in(const Term& l, const double& a, const double& b);

inline const Interval& ConstraintIn::image() const
{
   return x_;
}

inline Term ConstraintIn::term() const
{
   return left();
}

/*****************************************************************************
 * Constraint visitors.
 *****************************************************************************/
class ConstraintVisitor {
public:
   // destructor
   virtual ~ConstraintVisitor();

   // visit methods
   virtual void apply(const ConstraintEq* c);
   virtual void apply(const ConstraintLe* c);
   virtual void apply(const ConstraintLt* c);
   virtual void apply(const ConstraintGe* c);
   virtual void apply(const ConstraintGt* c);
   virtual void apply(const ConstraintIn* c);
};

} // namespace

#endif
