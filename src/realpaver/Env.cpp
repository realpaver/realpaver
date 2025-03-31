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

#include "realpaver/Env.hpp"
#include "realpaver/AssertDebug.hpp"

namespace realpaver {

Env::Env()
    : param_(nullptr)
    , tlim_(false)
    , nlim_(false)
{
   param_ = new Param();
}

Env::~Env()
{
   if (param_ != nullptr)
      delete param_;
}

Env::Env(const Env &e)
{
   param_ = new Param(*e.param_);
}

void Env::setParam(const Param &prm)
{
   if (param_ != nullptr)
      delete param_;
   param_ = new Param(prm);
}

Param *Env::getParam() const
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

} // namespace realpaver
