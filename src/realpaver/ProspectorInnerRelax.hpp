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

#ifndef REALPAVER_PROSPECTOR_INNER_RELAX_HPP
#define REALPAVER_PROSPECTOR_INNER_RELAX_HPP

#include "realpaver/LinearizerInner.hpp"
#include "realpaver/Prospector.hpp"
#include <memory>

namespace realpaver {

/**
 * @brief Prospector making a simple test.
 *
 * A point is chosen in the given box and a prover is used to verify the feasibility
 * property.
 */
class ProspectorInnerRelax : public Prospector {
public:
   /// Constructor
   ProspectorInnerRelax(const Variable &vobj, const Term &obj,
                        std::unique_ptr<LinearizerInner> linearizer,
                        std::unique_ptr<Prover> prover);

   bool exploreImpl(Node &node, RealPoint &pt) override;

   void setRelaxTol(double tol);

private:
   Variable vobj_;
   Term obj_;
   std::unique_ptr<LinearizerInner> linearizer_;
};

} // namespace realpaver

#endif
