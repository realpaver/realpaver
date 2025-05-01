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
 * @file   SelectorHybridSSR.hpp
 * @brief  Variable selection strategy SSR+LF
 * @author Laurent Granvilliers
 * @date   30 Apr 2025
 */

#ifndef REALPAVER_SELECTOR_HYBRID_SSR_HPP
#define REALPAVER_SELECTOR_HYBRID_SSR_HPP

#include "realpaver/SelectorLF.hpp"
#include "realpaver/SelectorSSR.hpp"

namespace realpaver {

/**
 *
 * TODO
 *
 */
class SelectorHybridSSR : public Selector {
public:
   /**
    * @brief Constructor.
    *
    * TODO
    */
   SelectorHybridSSR(Scope scop, IntervalFunctionVector F);

   /// Returns the frequency of application of the SSR strategy
   double getFrequency() const;

   /// Assigns the frequency of application of the SSR strategy
   void setFrequency(double f);

   bool apply(const DomainBox &box) override;

private:
   SelectorSSR ssr_; // SSR strategy
   SelectorLF lf_;   // LF strategy
   int nb_;          // counter of applications
   int nbssr_;       // counter of applications of SSR
   double f_;        // frequency of SSR with respect to LF
};

} // namespace realpaver

#endif
