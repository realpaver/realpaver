///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_ENV_HPP
#define REALPAVER_NCSP_ENV_HPP

#include "realpaver/Env.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an environment for NCSP solvers.
///////////////////////////////////////////////////////////////////////////////
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

   /// Sets a flag that informs on the termination of a solving process
   /// @param b true if the solving process is aborted due a solution limit
   void setSolutionLimit(bool b = true);

   /// @return true if a solving process is aborted due a solution limit
   bool usedSolutionLimit() const;

   /// Sets a flag that informs on the solving process
   /// @param b true if the a branch of the search tree is cut due a depth limit
   void setDepthLimit(bool b = true);

   /// @return true if the a branch of the search tree is cut due a depth limit
   bool usedDepthLimit() const;

   /// @return true if no limit has been activated during the solving process
   ///         that is a necessary condition for a complete search
   bool usedNoLimit() const;

private:
   bool slim_;    // solution limit reached
   bool dlim_;    // depth limit reached in a search tree
};

} // namespace

#endif
