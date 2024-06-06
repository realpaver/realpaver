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
 * @file   TermQuadratic.hpp
 * @brief  Quadratic expression
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_TERM_QUADRATIC_HPP
#define REALPAVER_TERM_QUADRATIC_HPP

#include <list>
#include <set>
#include "realpaver/Term.hpp"

namespace realpaver {

/// Quadratic expression
class TermQuadratic {
public:
   /// Constructor of a 0 term
   TermQuadratic();

   /// Constructor from a term
   TermQuadratic(const Term& t);

   /// Default copy constructor
   TermQuadratic(const TermQuadratic&) = default;

   /// No copy
   TermQuadratic& operator=(const TermQuadratic&) = default;

   /// Default destructor
   ~TermQuadratic() = default;

   /// Adds a constant in this
   void addConstant(const Interval& a);

   /// Adds a square term of the form a*v^2 in this
   void addSquare(const Interval& a, const Variable& v);

   /// Adds a bilinear  term of the form a*v1*v2 in this
   void addBilin(const Interval& a, const Variable& v1, const Variable& v2);

   /// Adds a linear term of the form a*v in this
   void addLin(const Interval& a, const Variable& v);

   /// Returns the term represented by this
   Term toTerm() const;

   /// Creates and return sthe scope of this
   Scope makeScope() const;

   /**
    * @brief Factorization method.
    * 
    * Return a term resulting from the factorization of this such that the
    * variables are ordered by a decreasing number of occurrences
    *
    * Given x the variable occurring the more in this the first step
    * generates the equivalent expression x*f + g such that x does not occur
    * in g, then g is factorized following the same process, and so on
    */
   Term factorize() const;

private:
   // square term
   struct Square {
      Interval coef;
      Variable v;
   };

   // comparator of square terms
   struct CompSquare {
      bool operator()(const Square& s1, const Square& s2) const
      {
         return s1.v.id() < s2.v.id();
      }
   };

   // linear term
   struct Lin {
      Interval coef;
      Variable v;
   };

   // comparator of linear term
   struct CompLin {
      bool operator()(const Lin& l1, const Lin& l2) const
      {
         return l1.v.id() < l2.v.id();
      }
   };

   // bilinear term
   struct Bilin {
      Interval coef;
      Variable v1;
      Variable v2;
   };

   // comparator of bilinear term
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

   friend std::ostream& operator<<(std::ostream& os, const TermQuadratic& t);

   void sortByOcc(std::list<Variable>& lv) const;
};

/// Output on a stream
/// @param os a stream
/// @param t quadratic term written
/// @return a reference to the given stream
std::ostream& operator<<(std::ostream& os, const TermQuadratic& t);

/*----------------------------------------------------------------------------*/

/// Visitor used to transform (or not) a term into a quadratic term
class TermQuadraticCreator : public TermVisitor {
public:
   /// Constructor given a pointer to the quadratic term modified by a visit
   TermQuadraticCreator(TermQuadratic* qt);

   /// Returns true if this has visited a term representing a quadratic term
   bool visitSuccessfull() const;

   /// Returns the quadratic term in this
   TermQuadratic* getTermQuadratic() const;

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
   TermQuadratic* qt_;
   bool plus_;
   bool success_;

   bool makeProd(const TermCst* tc, const TermMul* tm);
   bool makeProd(const TermCst* tc, const TermSqr* ts);
   bool makeProd(const TermVar* tv, const TermMul* tm);
};

} // namespace

#endif
