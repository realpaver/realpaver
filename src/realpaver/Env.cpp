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
 * @file   Env.cpp
 * @brief  Base class of environments for solvers
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Env.hpp"

namespace realpaver {

Env::Env()
    : params_(nullptr)
    , tlim_(false)
    , nlim_(false)
{
   params_ = new Params();
}

Env::~Env()
{
   if (params_ != nullptr)
      delete params_;
}

Env::Env(const Env &e)
{
   params_ = new Params(*e.params_);
}

void Env::setParams(const Params &params)
{
   if (params_ != nullptr)
      delete params_;
   params_ = new Params(params);
}

Params *Env::getParams() const
{
   return params_;
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

} // namespace realpaver
