///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_SELECTOR_HPP
#define REALPAVER_NCSP_SELECTOR_HPP

#include <vector>
#include "realpaver/IntervalFunctionVector.hpp"
#include "realpaver/NcspNode.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of variables used by splitting strategies.
///////////////////////////////////////////////////////////////////////////////
class NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param scop a scope
   NcspSelector(Scope scop);

   /// Virtual destructor
   virtual ~NcspSelector();

   /// Default copy constructor
   NcspSelector(const NcspSelector&) = default;

   /// No assignment
   NcspSelector& operator=(const NcspSelector&) = delete;

   /// @return the scope of this
   Scope scope() const;

   /// Selection method
   /// @param node a search node
   /// @return a pair <b, v> such that no variable is selected if b = false,
   ///         otherwise v is the selected variable
   virtual std::pair<bool, Variable> selectVar(NcspNode& node) = 0;

protected:
   Scope scop_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable following a round-robin strategy.
///////////////////////////////////////////////////////////////////////////////
class NcspSelectorRR : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param scop a scope
   NcspSelectorRR(Scope scop);

   /// Destructor
   ~NcspSelectorRR() = default;

   /// Default copy constructor
   NcspSelectorRR(const NcspSelectorRR&) = default;

   /// No assignment
   NcspSelectorRR& operator=(const NcspSelectorRR&) = delete;

   std::pair<bool, Variable> selectVar(NcspNode& node) override;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable having the largest domain.
/// Largest-First
///////////////////////////////////////////////////////////////////////////////
class NcspSelectorLF : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param scop a scope
   NcspSelectorLF(Scope scop);

   /// Destructor
   ~NcspSelectorLF() = default;

   /// Default copy constructor
   NcspSelectorLF(const NcspSelectorLF&) = default;

   /// No assignment
   NcspSelectorLF& operator=(const NcspSelectorLF&) = delete;

   std::pair<bool, Variable> selectVar(NcspNode& node) override;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable having the smallest domain.
/// Smallest-First
///////////////////////////////////////////////////////////////////////////////
class NcspSelectorSF : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param scop a scope
   NcspSelectorSF(Scope scop);

   /// Destructor
   ~NcspSelectorSF() = default;

   /// Default copy constructor
   NcspSelectorSF(const NcspSelectorSF&) = default;

   /// No assignment
   NcspSelectorSF& operator=(const NcspSelectorSF&) = delete;

   std::pair<bool, Variable> selectVar(NcspNode& node) override;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector that works as follows. If there is an integer variable
/// whose domain is splitable, then it selects the integer variable having the
/// smallest domain. Otherwise, it selects the real variable having the
/// largest domain.
///////////////////////////////////////////////////////////////////////////////
class NcspSelectorMixedSLF : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param scop a scope
   NcspSelectorMixedSLF(Scope scop);

   /// Destructor
   ~NcspSelectorMixedSLF() = default;

   /// Default copy constructor
   NcspSelectorMixedSLF(const NcspSelectorMixedSLF&) = default;

   /// No assignment
   NcspSelectorMixedSLF& operator=(const NcspSelectorMixedSLF&) = delete;

   std::pair<bool, Variable> selectVar(NcspNode& node) override;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable with the SmearSumRel heuristics.
///
/// Let F(x) be a vector of functions obtained from all the numeric constraints
/// of a problem and let B be a box. We first calculate the real matrix S
/// such that sij is the smear value of xi in fj, which is equal to the product
/// of the width of the domain of xi in B and the magnitude of the interval
/// derivative of fj with respect to xi evaluated in B. Then S is normalized
/// by considering each row to derive the smear relative values 0 <= rij <= 1.0.
/// Then for each column these values are addded.
///
/// For example, let F = (f1, f2) and x = (x1, x2). Let the smear matrix be
///   S = (s11, s12)
///       (s21, s22)
///
/// Then S is normalized as
///   R = (s11 / (s11+s12), s12 / (s11+s12))
///       (s21 / (s21+s22), s22 / (s21+s22))
///
/// Then the sums are calculated by columns to derive
///   smearSumRel(x1) = s11 / (s11+s12) + s21 / (s21+s22)
///   smearSumRel(x2) = s12 / (s11+s12) + s22 / (s21+s22)
///
/// These values are stored in this.
///////////////////////////////////////////////////////////////////////////////
class NcspSelectorSSR : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param f a function
   NcspSelectorSSR(IntervalFunctionVector F);

   /// Destructor
   ~NcspSelectorSSR() = default;

   /// Default copy constructor
   NcspSelectorSSR(const NcspSelectorSSR&) = default;

   /// No assignment
   NcspSelectorSSR& operator=(const NcspSelectorSSR&) = delete;

   std::pair<bool, Variable> selectVar(NcspNode& node) override;

   /// Variable selection in an interval box
   /// @param B an interval box
   /// @return a couple (b, v) such that b=false if no variable is selected,
   ///         b=true if v is selected
   std::pair<bool, Variable> selectVar(const IntervalBox& B);

   /// Gets the smearSumRel value of a variable
   /// @param v a variable
   /// @return the smear sum rel value of v in this
   double getSSR(const Variable& v) const;

   /// Calculates the smearSumRel value of the variables in a box
   /// @param B an interval box
   void calculateSSR(const IntervalBox& B);

private:
   IntervalFunctionVector F_;
   std::vector<double> ssr_;     // vector of smearSumRel values
};

} // namespace

#endif
