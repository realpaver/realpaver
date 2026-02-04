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
 * @file   AffineTightener.hpp
 * @brief  Tightnener of affine relaxations
 * @author Laurent Granvilliers
 * @date   29 Jan 2026
 */

#ifndef REALPAVER_AFFINE_TIGHTENER_HPP
#define REALPAVER_AFFINE_TIGHTENER_HPP

#include "realpaver/Dag.hpp"

namespace realpaver {

class AffineTightener {
public:
   /// Creates a tightener for the i-th function of a DAG
   AffineTightener(SharedDag dag, size_t i);

   /// No copy
   AffineTightener(const AffineTightener &) = delete;

   /// No assignment
   AffineTightener &operator=(const AffineTightener &) = delete;

   /// Default destructor
   ~AffineTightener() = default;

   /// Returns the dag
   SharedDag dag() const;

private:
   SharedDag dag_;
   size_t idx_;
};

} // namespace realpaver

#endif
