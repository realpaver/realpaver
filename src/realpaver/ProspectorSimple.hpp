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
 * @file   ProspectorSimple.hpp
 * @brief  Prospector testing one point
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#ifndef REALPAVER_PROSPECTOR_SIMPLE_HPP
#define REALPAVER_PROSPECTOR_SIMPLE_HPP

#include "realpaver/BoxPicker.hpp"
#include "realpaver/Prospector.hpp"
#include "realpaver/ProverOptim.hpp"

namespace realpaver {

/**
 * @brief Prospector making a simple test.
 *
 * A point is chosen in the given box and a prover is used to verify the feasibility
 * property.
 */
class ProspectorSimple : public Prospector {
public:
   /// Constructor
   ProspectorSimple(std::unique_ptr<BoxPicker> picker,
                    std::unique_ptr<ProverOptim> prover);

   bool exploreImpl(Node &node, RealPoint &pt) override;

private:
   std::unique_ptr<BoxPicker> picker_;
};

} // namespace realpaver

#endif
