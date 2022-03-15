///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_TERM_DERIVER_HPP
#define REALPAVER_TERM_DERIVER_HPP

#include "realpaver/Term.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor of terms that generates partial derivatives.
///////////////////////////////////////////////////////////////////////////////
class TermDeriver : public TermVisitor {
public:
   /// Creates a deriver
   /// @param v a variable
   TermDeriver(Variable v);

   /// Creates a deriver
   /// @param id an identifier of variable
   TermDeriver(size_t id);

   /// @return the partial derivative after a visit
   Term getDerivative() const;

   ///@{
   /// Overrides TermVisitor methods
   void apply(const TermConst* t);
   void apply(const TermVar* t);
   void apply(const TermAdd* t);
   void apply(const TermSub* t);
   void apply(const TermMul* t);
   void apply(const TermDiv* t);
   void apply(const TermMin* t);
   void apply(const TermMax* t);
   void apply(const TermUsb* t);
   void apply(const TermAbs* t);
   void apply(const TermSgn* t);
   void apply(const TermSqr* t);
   void apply(const TermSqrt* t);
   void apply(const TermPow* t);
   void apply(const TermExp* t);
   void apply(const TermLog* t);
   void apply(const TermCos* t);
   void apply(const TermSin* t);
   void apply(const TermTan* t);
   ///@}

private:
   size_t id_;    // variable id
   Term dt_;      // partial derivative
};

} // namespace

#endif
