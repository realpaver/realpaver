///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_SELECTOR_HPP
#define REALPAVER_SELECTOR_HPP

#include <vector>
#include "realpaver/IntervalFunction.hpp"
#include "realpaver/SearchNode.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of variables used by splitting strategies.
///////////////////////////////////////////////////////////////////////////////
class Selector {
public:
   /// Creates a selector on a set of variables
   ///@param s a scope
   Selector(Scope s);

   /// Virtual destructor
   virtual ~Selector();

   /// Default copy constructor
   Selector(const Selector&) = default;

   /// No assignment
   Selector& operator=(const Selector&) = delete;

   /// @return the scope of this
   Scope scope() const;

   /// Selection method
   /// @param node a search node
   /// @return a pair <b, v> such that no variable is selected if b = false,
   ///         otherwise v is the selected variable
   ///
   /// The default implementation calls selecVar on the region of the node.
   virtual std::pair<bool, Variable> selectVar(const SearchNode& node);

   /// Selection method
   /// @param reg variable domains
   /// @return a pair <b, v> such that no variable is selected if b = false,
   ///         otherwise v is the selected variable
   ///
   /// The default implementation returns <false, >
   virtual std::pair<bool, Variable> selectVar(const IntervalRegion& reg);

protected:
   Scope scope_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable with largest domain.
///////////////////////////////////////////////////////////////////////////////
class SelectorMaxDom : public Selector {
public:
   /// Creates a selector on a set of variables
   ///@param s a scope
   SelectorMaxDom(Scope s);

   /// Destructor
   ~SelectorMaxDom() = default;

   /// Default copy constructor
   SelectorMaxDom(const SelectorMaxDom&) = default;

   /// No assignment
   SelectorMaxDom& operator=(const SelectorMaxDom&) = delete;

   /// Overrides (Selector)
   std::pair<bool, Variable> selectVar(const IntervalRegion& reg);
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable with maximum smear.
///////////////////////////////////////////////////////////////////////////////
class SelectorMaxSmear : public Selector {
public:
   /// Creates a selector on a set of variables
   ///@param f a function
   ///@param s a scope
   SelectorMaxSmear(DiffIntervalFunction* f, Scope s);

   /// Destructor
   ~SelectorMaxSmear() = default;

   /// Default copy constructor
   SelectorMaxSmear(const SelectorMaxSmear&) = default;

   /// No assignment
   SelectorMaxSmear& operator=(const SelectorMaxSmear&) = delete;

   /// Overrides (Selector)
   std::pair<bool, Variable> selectVar(const IntervalRegion& reg);

private:
   DiffIntervalFunction* f_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable following a round-robin strategy.
///////////////////////////////////////////////////////////////////////////////
class SelectorRoundRobin : public Selector {
public:
   /// Creates a selector on a set of variables
   ///@param s a scope
   SelectorRoundRobin(Scope s);

   /// Destructor
   ~SelectorRoundRobin() = default;

   /// Default copy constructor
   SelectorRoundRobin(const SelectorRoundRobin&) = default;

   /// No assignment
   SelectorRoundRobin& operator=(const SelectorRoundRobin&) = delete;

   /// Overrides (Selector)
   std::pair<bool, Variable> selectVar(const SearchNode& node);
};

} // namespace

#endif
