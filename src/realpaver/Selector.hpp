/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   Selector.hpp
 * @brief  Selector of variables
 * @author Laurent Granvilliers
 * @date   25 Apr 2025
 */

#ifndef REALPAVER_SELECTOR_HPP
#define REALPAVER_SELECTOR_HPP

#include "realpaver/Node.hpp"
#include "realpaver/NodeContext.hpp"

namespace realpaver {

/**
 * @brief Selector of a variable in a node.
 *
 * A selector selects a variable in a node. A variable can be selected only
 * if its domain is large enough, i.e. it is splitable.
 */
class Selector {
public:
   /// Creates a selector
   Selector();

   /// Destructor
   virtual ~Selector();

   /// Default copy constructor
   Selector(const Selector &) = default;

   /// No assignment
   Selector &operator=(const Selector &) = delete;

   /**
    * @brief Selects a variable of the scope in a node.
    *
    * Returns true if a variable is selected. In this case, getVar() returns this
    * variable. Returns false if no variable can be selected.
    */
   virtual bool apply(SharedNode node, NodeContext &context) = 0;

   /// Returns the selected variable
   Variable getSelectedVar() const;

protected:
   // Assigns the selected variable
   void setSelectedVar(Variable v);

private:
   Variable v_; // selected variable if any
};

} // namespace realpaver

#endif
