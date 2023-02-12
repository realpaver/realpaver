///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_QUADRATIC_TERM_HPP
#define REALPAVER_QUADRATIC_TERM_HPP

#include <list>
#include <set>
#include "realpaver/Term.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This represents a quadratic expression.
///////////////////////////////////////////////////////////////////////////////
class QuadraticTerm {
public:
   /// Constructor of a 0 term
   QuadraticTerm();

   /// Constructor
   /// @param t a term to be transformed into a quadratic term
   QuadraticTerm(const Term& t);

   /// Default copy constructor
   QuadraticTerm(const QuadraticTerm&) = default;

   /// No copy
   QuadraticTerm& operator=(const QuadraticTerm&) = default;

   /// Default destructor
   ~QuadraticTerm() = default;

   /// Adds a constant in this
   /// @param a an interval
   void addConstant(const Interval& a);

   /// Adds a square term of the form a*v^2 in this
   /// @param a coefficient
   /// @param v variable
   void addSquare(const Interval& a, const Variable& v);

   /// Adds a bilinear  term of the form a*v1*v2 in this
   /// @param a coefficient
   /// @param v1 first variable
   /// @param v2 second variable
   void addBilin(const Interval& a, const Variable& v1, const Variable& v2);

   /// Adds a linear term of the form a*v in this
   /// @param a coefficient
   /// @param v variable
   void addLin(const Interval& a, const Variable& v);

   /// @return term represented by this
   Term toTerm() const;

   // TODO
   Scope makeScope() const;

   // TODO
   Term factorize() const;

private:
   struct Square {
      Interval coef;
      Variable v;
   };

   struct CompSquare {
      bool operator()(const Square& s1, const Square& s2) const
      {
         return s1.v.id() < s2.v.id();
      }
   };

   struct Lin {
      Interval coef;
      Variable v;
   };

   struct CompLin {
      bool operator()(const Lin& l1, const Lin& l2) const
      {
         return l1.v.id() < l2.v.id();
      }
   };

   struct Bilin {
      Interval coef;
      Variable v1;
      Variable v2;
   };

   struct CompBilin {
      bool operator()(const Bilin& b1, const Bilin& b2) const
      {
         return (b1.v1.id() < b2.v1.id()) ||
                ( (b1.v1.id() == b2.v1.id()) && (b1.v2.id() < b2.v2.id()) );
      }
   };

   Interval cst_;                      // constant
   std::set<Square, CompSquare> sq_;   // square terms
   std::set<Bilin, CompBilin> sb_;     // bilinear terms
   std::set<Lin, CompLin> sl_;         // linear terms

   friend std::ostream& operator<<(std::ostream& os, const QuadraticTerm& t);

   void sortByOcc(std::list<Variable>& lv) const;
};

/// Output on a stream
/// @param os a stream
/// @param t quadratic term written
/// @return a reference to the given stream
std::ostream& operator<<(std::ostream& os, const QuadraticTerm& t);

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor used to transform (or not) a term into a quadratic term.
///////////////////////////////////////////////////////////////////////////////
class QuadraticTermCreator : public TermVisitor {
public:
   /// Constructor
   /// @param qt a pointer to the quadratic term modified by a visit
   QuadraticTermCreator(QuadraticTerm* qt);

   /// @return true if this has visited a term representing a quadratic term
   bool visitSuccessfull() const;

   /// @return the quadratic term in this
   QuadraticTerm* getQuadraticTerm() const;

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
   QuadraticTerm* qt_;
   bool plus_;
   bool success_;

   bool makeProd(const TermConst* tc, const TermMul* tm);
   bool makeProd(const TermConst* tc, const TermSqr* ts);
   bool makeProd(const TermVar* tv, const TermMul* tm);
};

} // namespace

#endif
