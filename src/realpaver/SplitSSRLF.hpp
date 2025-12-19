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
 * @file   SplitSSRLF.hpp
 * @brief  Splitting strategy hybridizing SSR and LF
 * @author Laurent Granvilliers
 * @date   10 June 2025
 */

#ifndef REALPAVER_SPLIT_SSRLF_HPP
#define REALPAVER_SPLIT_SSRLF_HPP

#include "realpaver/SelectorSSRLF.hpp"
#include "realpaver/Split.hpp"

namespace realpaver {

/// Splitting strategy hybridizing SSR and LF
class SplitSSRLF : public Split {
public:
   /// Constructor
   SplitSSRLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap,
              IntervalFunctionVector F);

   /// Default destructor
   ~SplitSSRLF() = default;

   /// No copy
   SplitSSRLF(const SplitSSRLF &) = delete;

   /// No assignment
   SplitSSRLF &operator=(const SplitSSRLF &) = delete;

   /// Returns the frequency of application of the SSR strategy
   double getFrequency() const;

   /// Assigns the frequency of application of the SSR strategy
   void setFrequency(double f);

   void applyImpl(SharedNode node, NodeContext &context) override;

private:
   SelectorSSRLF sel_; // hybrid selector
};

} // namespace realpaver

#endif
