///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_ENV_HPP
#define REALPAVER_ENV_HPP

#include "realpaver/Param.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a base class of environments for solvers.
///////////////////////////////////////////////////////////////////////////////
class Env {
public:
   /// Constructor
   Env();

   /// Destructor
   virtual ~Env();

   /// Copy constructor
   /// @param e environment copied in this
   Env(const Env& e);

   /// No assignment
   Env& operator=(const Env&) = delete;

   /// Sets the object that manages the parameter settings
   /// @param prm new object
   void setParam(const Param& prm);

   /// @return the object that manages the parameter settings
   Param* getParam() const;

   /// Sets a flag that informs on the termination of a solving process
   /// @param b true if the solving process is aborted due a time limit
   void setTimeLimit(bool b = true);

   /// @return true if a solving process is aborted due a time limit
   bool usedTimeLimit() const;

   /// Sets a flag that informs on the termination of a solving process
   /// @param b true if the solving process is aborted due a node limit
   void setNodeLimit(bool b = true);
   
   /// @return true if a solving process is aborted due a node limit
   bool usedNodeLimit() const;

private:
   Param* param_;    // management of parameters
   bool tlim_;       // time limit reached
   bool nlim_;       // node limit reached
};

} // namespace

#endif
