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
 * @file   SelectorSSRLF.hpp
 * @brief  Variable selection strategy alternating SSR and LF
 * @author Laurent Granvilliers
 * @date   30 Apr 2025
 */

#ifndef REALPAVER_SELECTOR_SSR_LF_HPP
#define REALPAVER_SELECTOR_SSR_LF_HPP

#include "realpaver/SelectorLF.hpp"
#include "realpaver/SelectorSSR.hpp"

namespace realpaver {

/**
 * @brief Variable selection strategy alternating SSR and LF.
 *
 * Let f be a real in [0, 1] that represents a frequency of application of
 * the SSR strategy. If f = 1 then SSR is used. If f = 0 then LF is used. Otherwise SSR
 * is used with frequency f with respect to LF, e.g. f = 0.75 means that SSR is used 3
 * times out of 4.
 */
class SelectorSSRLF : public Selector {
public:
   /**
    * @brief Constructor.
    *
    * The scope is given as input to the LF selector and F is an input of
    * the SSR selector.
    */
   SelectorSSRLF(Scope scop, IntervalFunctionVector F);

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
