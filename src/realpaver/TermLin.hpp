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
 * @file   TermLin.hpp
 * @brief  Linear expression
 * @author Laurent Granvilliers
 * @date   2024-6-22
 */

#ifndef REALPAVER_TERM_LIN_HPP
#define REALPAVER_TERM_LIN_HPP

#include <list>
#include <set>
#include "realpaver/Term.hpp"

namespace realpaver {


/// Linear expression
class TermLin {
public:
   /// Creates 0
   TermLin();

   /**
    * @brief Creates a linear term representing t
    *
    * Throws an exception if t is not linear
    */
   TermLin(const Term& t);

   /// Default copy constructor
   TermLin(const TermLin&) = default;

   /// No copy
   TermLin& operator=(const TermLin&) = default;

   /// Default destructor
   ~TermLin() = default;

   /// Returns the hash code
   size_t hashCode() const;

   /// Returns true if this is reduced to a constant
   bool isConstant() const;

   /// Adds a constant in this
   void addConstant(const Interval& a);

   /// Adds a linear term of the form a*v in this
   void addLin(const Interval& a, const Variable& v);

   /// Returns the term represented by this
   Term toTerm() const;

   /// Creates and returns the scope of this
   Scope makeScope() const;

   /// Returns the value of the constant term
   Interval getCst() const;

   /// Returns the number of linear terms
   size_t nbTerms() const;

   /// Returns the coefficient of the i-th linear term
   Interval coef(size_t i) const;

   /// Returns the variable of the i-th linear term
   Variable var(size_t i) const;

private:
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

   Interval cst_;                      // constant
   std::set<Lin, CompLin> sl_;         // linear terms

   friend std::ostream& operator<<(std::ostream& os, const TermLin& t);
};

/// Output on a stream
/// @param os a stream
/// @param t linear term written
/// @return a reference to the given stream
std::ostream& operator<<(std::ostream& os, const TermLin& t);

/*----------------------------------------------------------------------------*/

/// Visitor used to transform (or not) a term into a linear term
class TermLinCreator : public TermVisitor {
public:
   /// Constructor given a pointer to the quadratic term modified by a visit
   TermLinCreator(TermLin* lt, const Interval& factor = Interval::one());

   /// Returns true if this has visited a term representing a linear term
   bool visitSuccessfull() const;

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
   void apply(const TermCosh* t) override;
   void apply(const TermSinh* t) override;
   void apply(const TermTanh* t) override;

private:
   TermLin* lt_;
   Interval factor_;
   bool success_;
};

} // namespace

#endif
