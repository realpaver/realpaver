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
 * @file   LinearizerAffine.hpp
 * @brief  Linearization of nonlinear problems based on affine forms
 * @author Laurent Granvilliers
 * @date   11 Feb 2025
 */

#ifndef REALPAVER_LINEARIZER_AFFINE_HPP
#define REALPAVER_LINEARIZER_AFFINE_HPP

#include "realpaver/Linearizer.hpp"

namespace realpaver {

/**
 * @brief Linearizer that generates affine forms.
 *
 * The elementary functions can be approximated by means of the Minrange
 * method or the Chebyshev method.
 */
class LinearizerAffine : public Linearizer {
public:
   /// Creates a linearizer of a DAG
   LinearizerAffine(SharedDag dag, bool minrange = true);

   /// No copy
   LinearizerAffine(const LinearizerAffine &) = delete;

   /// No assignment
   LinearizerAffine &operator=(const LinearizerAffine &) = delete;

   /// Default destructor
   ~LinearizerAffine() = default;

   /// Uses the Minrange approximation (true) or Chebyshev (false)
   void useMinrange(bool minrange = true);

   bool makeLPImpl(LPModel &lpm, const IntervalBox &B, const Bitset &active) override;

private:
   bool minrange_; // true: Minrange, false: Chebyshev

   using MapType = std::unordered_map<size_t, size_t>;
   MapType mve_; // map var index -> lin var index of the epsilon variable
};

} // namespace realpaver

#endif
