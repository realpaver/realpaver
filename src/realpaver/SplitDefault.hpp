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
 * @file   SplitDefault.hpp
 * @brief  Default splitting strategies
 * @author Laurent Granvilliers
 * @date   29 April 2026
 */

#ifndef REALPAVER_SPLIT_DEFAULT_HPP
#define REALPAVER_SPLIT_DEFAULT_HPP

#include "realpaver/Selector.hpp"
#include "realpaver/Split.hpp"

namespace realpaver {

class SplitDefault : public Split {
public:
   /// Constructor
   SplitDefault(Scope scop, std::unique_ptr<DomainSlicerMap> smap,
                std::unique_ptr<Selector> slc, std::unique_ptr<Selector> sld);

   /// Default destructor
   ~SplitDefault() = default;

   /// No copy
   SplitDefault(const SplitDefault &) = delete;

   /// No assignment
   SplitDefault &operator=(const SplitDefault &) = delete;

   /// Returns the length of a full cycle
   int cycleLength() const;

   /// Assigns the length of a full cycle
   void setCycleLength(int len);

   /// Returns the length of a cycle using the Round-Robin strategy
   int cycleLengthRR() const;

   /// Assigns the length of a cycle using the Round-Robin strategy
   void setCycleLengthRR(int len);

   /// Selects discrete variables before other variables if ok = true
   void setDiscreteFirst(bool ok = true);

   void applyImpl(SharedNode node, NodeContext &context) override;

private:
   std::unique_ptr<Selector> slc_; // selector of continuous variables
   std::unique_ptr<Selector> sld_; // selector of discrete variables

   int cycleLength_;    // length of cycle (round-robin and others)
   int cycleLengthRR_;  // length of round-robin cycle
   bool discreteFirst_; // true: selects discrete variables first
   int nbRR_;
   int nb_;

   std::pair<bool, Variable> selectRR(SharedNode node, NodeContext &context);
};

} // namespace realpaver

#endif
