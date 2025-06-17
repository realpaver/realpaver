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
 * @file   LinearizerRLT.hpp
 * @brief  Linearization of nonlinear problems based on RLT
 * @author Laurent Granvilliers
 * @date   5 May 2025
 */

#ifndef REALPAVER_LINEARIZER_RLT_HPP
#define REALPAVER_LINEARIZER_RLT_HPP

#include "realpaver/Linearizer.hpp"
#include <unordered_map>

namespace realpaver {

/**
 * @brief Linearizer that generates Taylor forms.
 *
 * It generates a relaxation using Taylor forms in two opposite corners.
 * The first corner can be fixed manually or selected randomly.
 */
class LinearizerRLT : public Linearizer {
public:
   /**
    * @brief Creates a linearizer of a DAG.
    */
   LinearizerRLT(SharedDag dag);

   /// No copy
   LinearizerRLT(const LinearizerRLT &) = delete;

   /// No assignment
   LinearizerRLT &operator=(const LinearizerRLT &) = delete;

   /// Default destructor
   ~LinearizerRLT() = default;

   bool makeLPImpl(LPModel &lpm, const IntervalBox &B, const Bitset &active) override;

private:
   size_t nodeToLinVar(DagNode *node) const;
   void createLinVar(LPModel &lpm, DagNode *node);

   std::unordered_map<size_t, size_t> mnv_; // node index -> lin var index
};

} // namespace realpaver

#endif
