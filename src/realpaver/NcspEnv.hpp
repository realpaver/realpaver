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
 * @file   NcspEnv.hpp
 * @brief  Environment for NCSP solver
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#ifndef REALPAVER_NCSP_ENV_HPP
#define REALPAVER_NCSP_ENV_HPP

#include "realpaver/Env.hpp"

namespace realpaver {

/// Environment for NCSP solver
class NcspEnv : public Env {
public:
   /// Constructor
   NcspEnv();

   /// Default destructor
   ~NcspEnv() = default;

   /// Default copy constructor
   NcspEnv(const NcspEnv&) = default;

   /// No assignment
   NcspEnv& operator=(const NcspEnv&) = delete;

   /// Determines if the solving process is aborted due a solution limit
   void setSolutionLimit(bool b = true);

   /// Returns true if a solving process is aborted due a solution limit
   bool usedSolutionLimit() const;

   /// Determines if the a branch of the search tree is cut due a depth limit
   void setDepthLimit(bool b = true);

   /// Returns true if the a branch of the search tree is cut due a depth limit
   bool usedDepthLimit() const;

   /// Returns true if no limit has been activated during the solving
   bool usedNoLimit() const;

private:
   bool slim_;    // solution limit reached
   bool dlim_;    // depth limit reached in a search tree
};

} // namespace

#endif
