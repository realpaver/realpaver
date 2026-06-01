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
 * @brief  Prospector using a local solver
 * @author Raphaël Chenouard
 * @date   16 Oct 2025
 */

#ifndef REALPAVER_PROSPECTOR_LOCALSOLVER_HPP
#define REALPAVER_PROSPECTOR_LOCALSOLVER_HPP

#include "realpaver/DomainBoxPicker.hpp"
#include "realpaver/NLPModel.hpp"
#include "realpaver/Prospector.hpp"

namespace realpaver {

/**
 * @brief Prospector using a local solver.
 *
 * A point is searched using a local solver in the given box and a prover is used to
 * verify the feasibility property.
 */
class ProspectorLocalSolver : public Prospector {
public:
   /// Constructor
   ProspectorLocalSolver(std::unique_ptr<DomainBoxPicker> picker,
                         std::unique_ptr<Prover> prover,
                         std::unique_ptr<NLPModel> localSolver);

   bool exploreImpl(Node &node, RealPoint &pt) override;

private:
   std::unique_ptr<DomainBoxPicker> picker_;
   std::unique_ptr<NLPModel> localSolver_;
};

} // namespace realpaver

#endif
