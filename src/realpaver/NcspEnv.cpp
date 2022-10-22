///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspEnv.hpp"

namespace realpaver {

NcspEnv::NcspEnv() : Env(), slim_(false), dlim_(false)
{}

void NcspEnv::setSolutionLimit(bool b)
{
   slim_ = b;
}

bool NcspEnv::usedSolutionLimit() const
{
   return slim_;
}

void NcspEnv::setDepthLimit(bool b)
{
   dlim_ = true;
}

bool NcspEnv::usedDepthLimit() const
{
   return dlim_;
}

bool NcspEnv::usedNoLimit() const
{
   return !(usedTimeLimit() || usedNodeLimit() || usedSolutionLimit() ||
            usedDepthLimit());
}

} // namespace
