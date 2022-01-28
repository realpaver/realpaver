// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_TERM_DERIVER_HPP
#define REALPAVER_TERM_DERIVER_HPP

#include "realpaver/term.hpp"

namespace realpaver {

/*****************************************************************************
 * Visitor of terms that generates partial derivatives.
 *****************************************************************************/
class TermDeriver : public TermVisitor {
public:
   // constructors given a variable or an identifier of a variable
   TermDeriver(const Variable& v);
   TermDeriver(size_t id);

   // returns the term created by this visitor
   Term get() const;

   // Override
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

private:
   size_t id_;    // variable id
   Term dt_;      // partial derivative
};

inline Term TermDeriver::get() const
{
   return dt_;
}

} // namespace

#endif
