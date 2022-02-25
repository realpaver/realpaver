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
#include "realpaver/IntervalVector.hpp"
#include "realpaver/Scope.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of variables used by splitting strategies.
///////////////////////////////////////////////////////////////////////////////
class Selector {
public:
   /// Creates a selector on a set of variables
   ///@param s a scope
   Selector(const Scope& s);

   /// Virtual destructor
   virtual ~Selector();

   /// Default copy constructor
   Selector(const Selector&) = default;

   /// No assignment
   Selector& operator=(const Selector&) = delete;

   /// @return the scope of this
   Scope scope() const;

   /// Selection method
   /// @param X a region
   /// @return a pair <b, v> such that no variable is selected if b = false,
   ///         otherwise v is the selected variable
   virtual std::pair<bool, Variable> selectVar(const IntervalVector& X) = 0;

private:
   Scope scope_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a selector of the variable with largest domain.
///////////////////////////////////////////////////////////////////////////////
class SelectorMaxDom : public Selector {
public:
   /// Creates a selector on a set of variables
   ///@param s a scope
   SelectorMaxDom(const Scope& s);

   /// Destructor
   ~SelectorMaxDom() = default;

   /// Default copy constructor
   SelectorMaxDom(const SelectorMaxDom&) = default;

   /// No assignment
   SelectorMaxDom& operator=(const SelectorMaxDom&) = delete;

   /// Overrides
   /// @param X a region
   /// @return a pair <b, v> such that no variable is selected if b = false,
   ///         otherwise v is the selected variable
   std::pair<bool, Variable> selectVar(const IntervalVector& X);
};

} // namespace

#endif
