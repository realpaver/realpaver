// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_TERM_FIXER_HPP
#define REALPAVER_TERM_FIXER_HPP

#include "realpaver_term.hpp"

namespace realpaver {

/*****************************************************************************
 * Visitors of terms creating new terms by substitutions of variables.
 * 
 * Let t be a term, let vvm be a map Variable -> Variable, and let vim be a map
 * Variable -> Interval.
 * For every entry (v, x) in vvm, every occurrence of v in t is replaced by x.
 * For every entry (v, d) in vim, every occurrence of v in t is replaced by d.
 *****************************************************************************/
class TermFixer : public TermVisitor {
public:
   // map type Variable -> Variable
   typedef std::unordered_map<Variable, Variable, VariableHasher> VVMap;

   // map type Variable -> Interval
   typedef std::unordered_map<Variable, Interval, VariableHasher> VIMap;

   // constructor given the two maps
   TermFixer(VVMap* vvm, VIMap* vim);

   // returns the new term after a visit
   Term getTerm() const;

   // visit methods
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
   VVMap* vvm_;
   VIMap* vim_;
   Term t_;
};

inline Term TermFixer::getTerm() const
{
   return t_;
}

} // namespace

#endif
