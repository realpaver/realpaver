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
/// This is a selector of the variable with largest domain.
///////////////////////////////////////////////////////////////////////////////
/*
class MaxDomSelector : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param s a scope
   MaxDomSelector(Scope s);

   /// Destructor
   ~MaxDomSelector() = default;

   /// Default copy constructor
   MaxDomSelector(const MaxDomSelector&) = default;

   /// No assignment
   MaxDomSelector& operator=(const MaxDomSelector&) = delete;

   ///@{
   std::pair<bool, Variable> selectVar(SearchNode& node) override;
   ///@}
};
*/

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
/// This is a selector of the variable following an hybrid strategy.
///
/// It alternates the max-dom selector and the round-robin selector according
/// to a factor f >= 1.
/// - f = 1: round-robin always used
/// - f = 2: alternates round-robin and max-dom
/// - f = 3: applies round-robin, two times max-dom, ...
/// and so on
///////////////////////////////////////////////////////////////////////////////
/*
class HybridDomRobinSelector : public NcspSelector {
public:
   /// Creates a selector on a set of variables
   /// @param s a scope
   /// @param f inverse of the frequency of use of the round-robin strategy
   HybridDomRobinSelector(Scope s, int f);

   /// Destructor
   ~HybridDomRobinSelector() = default;

   /// Default copy constructor
   HybridDomRobinSelector(const HybridDomRobinSelector&) = default;

   /// No assignment
   HybridDomRobinSelector& operator=(const HybridDomRobinSelector&) = delete;

   ///@{
   std::pair<bool, Variable> selectVar(SearchNode& node) override;
   ///@}

private:
   int f_;
};
*/

} // namespace

#endif
