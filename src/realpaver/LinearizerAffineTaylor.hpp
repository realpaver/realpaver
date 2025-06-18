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
 * @file   LinearizerAffineTaylor.hpp
 * @brief  Linearization of nonlinear problems based on affine and Taylor forms
 * @author Laurent Granvilliers
 * @date   11 Fev 2025
 */

#ifndef REALPAVER_LINEARIZER_AFFINE_TAYLOR_HPP
#define REALPAVER_LINEARIZER_AFFINE_TAYLOR_HPP

#include "realpaver/LinearizerAffine.hpp"
#include "realpaver/LinearizerTaylor.hpp"

namespace realpaver {

/// Linearizer that generates affine forms and Taylor forms.
class LinearizerAffineTaylor : public Linearizer {
public:
   /// Creates a linearizer of a DAG
   LinearizerAffineTaylor(std::unique_ptr<LinearizerAffine> affine,
                          std::unique_ptr<LinearizerTaylor> taylor);

   /// No copy
   LinearizerAffineTaylor(const LinearizerAffineTaylor &) = delete;

   /// No assignment
   LinearizerAffineTaylor &operator=(const LinearizerAffineTaylor &) = delete;

   /// Default destructor
   ~LinearizerAffineTaylor() = default;

   bool makeLPImpl(LPModel &lpm, const IntervalBox &B, const Bitset &active) override;

private:
   std::unique_ptr<LinearizerAffine> affine_;
   std::unique_ptr<LinearizerTaylor> taylor_;
};

} // namespace realpaver

#endif
