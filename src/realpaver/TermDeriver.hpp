///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
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

   /// @return the partial derivative after a visit
   Term getDerivative() const;

   ///@{
   void apply(const TermConst* t) override;
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
   ///@}

private:
   Variable v_;   // variable
   Term dt_;      // partial derivative wrt. this variable
};

} // namespace

#endif
