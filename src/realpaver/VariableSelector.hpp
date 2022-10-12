///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_VARIABLE_SELECTOR_HPP
#define REALPAVER_VARIABLE_SELECTOR_HPP

#include <vector>
#include "realpaver/IntervalFunction.hpp"
#include "realpaver/SearchNode.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of variables used by splitting strategies.
///////////////////////////////////////////////////////////////////////////////
class VariableSelector {
public:
   /// Creates a selector on a set of variables
   ///@param s a scope
   VariableSelector(Scope s);

   /// Virtual destructor
   virtual ~VariableSelector();

   /// Default copy constructor
   VariableSelector(const VariableSelector&) = default;

   /// No assignment
   VariableSelector& operator=(const VariableSelector&) = delete;

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
class MaxDomSelector : public VariableSelector {
public:
   /// Creates a selector on a set of variables
   ///@param s a scope
   MaxDomSelector(Scope s);

   /// Destructor
   ~MaxDomSelector() = default;

   /// Default copy constructor
   MaxDomSelector(const MaxDomSelector&) = default;

   /// No assignment
   MaxDomSelector& operator=(const MaxDomSelector&) = delete;

   /// Overrides (Selector)
   std::pair<bool, Variable> selectVar(const IntervalRegion& reg);
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable with maximum smear.
///////////////////////////////////////////////////////////////////////////////
class MaxSmearSelector : public VariableSelector {
public:
   /// Creates a selector on a set of variables
   ///@param f a function
   ///@param s a scope
   MaxSmearSelector(DiffIntervalFunction* f, Scope s);

   /// Destructor
   ~MaxSmearSelector() = default;

   /// Default copy constructor
   MaxSmearSelector(const MaxSmearSelector&) = default;

   /// No assignment
   MaxSmearSelector& operator=(const MaxSmearSelector&) = delete;

   /// Overrides (Selector)
   std::pair<bool, Variable> selectVar(const IntervalRegion& reg);

private:
   DiffIntervalFunction* f_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable following a round-robin strategy.
///////////////////////////////////////////////////////////////////////////////
class RoundRobinSelector : public VariableSelector {
public:
   /// Creates a selector on a set of variables
   ///@param s a scope
   RoundRobinSelector(Scope s);

   /// Destructor
   ~RoundRobinSelector() = default;

   /// Default copy constructor
   RoundRobinSelector(const RoundRobinSelector&) = default;

   /// No assignment
   RoundRobinSelector& operator=(const RoundRobinSelector&) = delete;

   /// Overrides (Selector)
   std::pair<bool, Variable> selectVar(const SearchNode& node);
};

} // namespace

#endif
