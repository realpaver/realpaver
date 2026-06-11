/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2026 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   LinearizerInnerAffine.hpp
 * @brief  Linearization of nonlinear problems based on affine forms
 * @author Raphaël Chenouard
 * @date   11 Feb 2026
 */

#ifndef REALPAVER_LINEARIZER_INNER_AFFINE_HPP
#define REALPAVER_LINEARIZER_INNER_AFFINE_HPP

#include "realpaver/LinearizerInner.hpp"

namespace realpaver {

/**
 * @brief Linearizer that generates affine forms.
 *
 * The elementary functions can be approximated by means of the Minrange
 * method or the Chebyshev method.
 */
class LinearizerInnerAffine : public LinearizerInner {
public:
   /// Creates a linearizer of a DAG
   LinearizerInnerAffine(SharedDag dag, bool minrange = true);

   /// No copy
   LinearizerInnerAffine(const LinearizerInnerAffine &) = delete;

   /// No assignment
   LinearizerInnerAffine &operator=(const LinearizerInnerAffine &) = delete;

   /// Default destructor
   ~LinearizerInnerAffine() = default;

   /// Uses the Minrange approximation (true) or Chebyshev (false)
   void useMinrange(bool minrange = true);

   bool makeLPImpl(LPModel &lpm, const IntervalBox &B, const Bitset &active) override;

private:
   bool minrange_; // true: Minrange, false: Chebyshev
};

} // namespace realpaver

#endif
