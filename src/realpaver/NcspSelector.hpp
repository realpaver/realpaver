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
//#include "realpaver/IntervalFunctionVector.hpp"
#include "realpaver/NcspNode.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of variables used by splitting strategies.
///////////////////////////////////////////////////////////////////////////////
class NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param s a scope
   NcspSelector(Scope s);

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
   Scope scope_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable following a round-robin strategy.
///////////////////////////////////////////////////////////////////////////////
class NcspSelectorRR : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param s a scope
   NcspSelectorRR(Scope s);

   /// Destructor
   ~NcspSelectorRR() = default;

   /// Default copy constructor
   NcspSelectorRR(const NcspSelectorRR&) = default;

   /// No assignment
   NcspSelectorRR& operator=(const NcspSelectorRR&) = delete;

   ///@{
   std::pair<bool, Variable> selectVar(NcspNode& node) override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable having the largest domain.
/// Largest-First
///////////////////////////////////////////////////////////////////////////////
class NcspSelectorLF : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param s a scope
   NcspSelectorLF(Scope s);

   /// Destructor
   ~NcspSelectorLF() = default;

   /// Default copy constructor
   NcspSelectorLF(const NcspSelectorLF&) = default;

   /// No assignment
   NcspSelectorLF& operator=(const NcspSelectorLF&) = delete;

   ///@{
   std::pair<bool, Variable> selectVar(NcspNode& node) override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable having the smallest domain.
/// Smallest-First
///////////////////////////////////////////////////////////////////////////////
class NcspSelectorSF : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param s a scope
   NcspSelectorSF(Scope s);

   /// Destructor
   ~NcspSelectorSF() = default;

   /// Default copy constructor
   NcspSelectorSF(const NcspSelectorSF&) = default;

   /// No assignment
   NcspSelectorSF& operator=(const NcspSelectorSF&) = delete;

   ///@{
   std::pair<bool, Variable> selectVar(NcspNode& node) override;
   ///@}
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
   /// @param s a scope
   NcspSelectorMixedSLF(Scope s);

   /// Destructor
   ~NcspSelectorMixedSLF() = default;

   /// Default copy constructor
   NcspSelectorMixedSLF(const NcspSelectorMixedSLF&) = default;

   /// No assignment
   NcspSelectorMixedSLF& operator=(const NcspSelectorMixedSLF&) = delete;

   ///@{
   std::pair<bool, Variable> selectVar(NcspNode& node) override;
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable with maximum smear.
///////////////////////////////////////////////////////////////////////////////
/*
class MaxSmearSelector : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param f a function
   /// @param s a scope
   MaxSmearSelector(IntervalFunctionVector F, Scope s);

   /// Destructor
   ~MaxSmearSelector() = default;

   /// Default copy constructor
   MaxSmearSelector(const MaxSmearSelector&) = default;

   /// No assignment
   MaxSmearSelector& operator=(const MaxSmearSelector&) = delete;

   ///@{
   std::pair<bool, Variable> selectVar(SearchNode& node) override;
   ///@}

private:
   IntervalFunctionVector F_;
};
*/

} // namespace

#endif
