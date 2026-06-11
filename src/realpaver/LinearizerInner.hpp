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
 * @file   LinearizerInner.hpp
 * @brief  Inner linearization of nonlinear problems
 * @author Raphaël Chenouard
 * @date   11 Jun 2026
 */

#ifndef REALPAVER_LINEARIZER_INNER_HPP
#define REALPAVER_LINEARIZER_INNER_HPP

#include "realpaver/Dag.hpp"
#include "realpaver/LPModel.hpp"
#include "realpaver/Linearizer.hpp"

namespace realpaver {

/**
 * @brief Base class of inner linearizers of nonlinear problems.
 *
 * A linearizer is used to linearize constraints from a DAG. Its scope corresponds
 * to the DAG's scope. A resulting linear model must involve all the variables from
 * the scope, even if some variable does not occur in active constraints.
 */
class LinearizerInner : public Linearizer {
public:
   /// Creates a linearizer for a DAG
   LinearizerInner(SharedDagExtended dag);
};

} // namespace realpaver

#endif
