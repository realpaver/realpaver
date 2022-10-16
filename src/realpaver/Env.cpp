///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/Env.hpp"

namespace realpaver {

Env::Env() : param_(nullptr), tlim_(false), nlim_(false)
{
   param_ = new Param();
}

Env::~Env()
{
   if (param_ != nullptr) delete param_;   
}

Env::Env(const Env& e)
{
   param_ = new Param(*e.param_);
}

void Env::setParam(const Param& prm)
{
   if (param_ != nullptr) delete param_;
   param_ = new Param(prm);
}

Param* Env::getParam() const
{
   return param_;
}

void Env::setTimeLimit(bool b)
{
   tlim_ = b;
}

bool Env::usedTimeLimit() const
{
   return tlim_;
}

void Env::setNodeLimit(bool b)
{
   nlim_ = b;
}

bool Env::usedNodeLimit() const
{
   return nlim_;
}

void Env::setPresolved(bool b)
{
   presolved_ = b;
}

bool Env::isPresolved() const
{
   return presolved_;
}

void Env::setConstraintViolated(bool b)
{
   cviol_ = b;
}

bool Env::hasConstraintViolated() const
{
   return cviol_;
}

} // namespace
