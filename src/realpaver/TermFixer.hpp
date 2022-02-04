///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_TERM_FIXER_HPP
#define REALPAVER_TERM_FIXER_HPP

#include "realpaver/term.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor of terms that replaces variables.
///
/// Let t be a term, let vvm be a map Variable -> Variable, and let vim
/// be a map Variable -> Interval.
/// - for every entry (v, x) in vvm, every occurrence of v in t is replaced
///   by x;
/// - for every entry (v, d) in vim, every occurrence of v in t is replaced
/// - by d.
///////////////////////////////////////////////////////////////////////////////
class TermFixer : public TermVisitor {
public:
   /// map type Variable -> Variable
   typedef std::unordered_map<Variable,
                              Variable,
                              VariableHasher> VarVarMapType;

   /// map type Variable -> Interval
   typedef std::unordered_map<Variable,
                              Interval,
                              VariableHasher> VarIntervalMapType;

   /// Creates a fixer given the two maps
   /// @param vvm map Variable -> Variable
   /// @param vim map Variable -> Interval
   TermFixer(VarVarMapType* vvm, VarIntervalMapType* vim);

   /// No copy
   TermFixer(const TermFixer&) = delete;

   /// No assignment
   TermFixer& operator=(const TermFixer&) = delete;

   /// Default destructor
   ~TermFixer() = default;

   /// @return the new term after a visit
   Term getTerm() const;

   /// Visits a term
   /// @param t term visited
   void apply(const TermConst* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermVar* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermAdd* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermSub* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermMul* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermDiv* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermMin* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermMax* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermUsb* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermAbs* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermSgn* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermSqr* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermSqrt* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermPow* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermExp* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermLog* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermCos* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermSin* t);

   /// Visits a term
   /// @param t term visited
   void apply(const TermTan* t);

private:
   VarVarMapType* vvm_;
   VarIntervalMapType* vim_;
   Term t_;
};

} // namespace

#endif
