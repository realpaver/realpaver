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
 * @file   RealFunction.hpp
 * @brief  Real functions
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/RealFunction.hpp"

namespace realpaver {

RealFunctionRep::~RealFunctionRep()
{
}

/*----------------------------------------------------------------------------*/

RealFunction::RealFunction(SharedDagFun f)
    : rep_(std::make_shared<RealFunctionDag>(f))
{
}

RealFunction::RealFunction(SharedDag dag, size_t i)
    : rep_(std::make_shared<RealFunctionDag>(dag->fun(i)))
{
}

RealFunction::RealFunction(Term t, const Interval &img)
    : rep_(std::make_shared<RealFunctionDag>(t, img))
{
}

RealFunction::RealFunction(SharedRep rep)
    : rep_(rep)
{
   REALPAVER_ASSERT(rep != nullptr,
                    "Creation of an interval function from a null pointer");
}

RealFunction::SharedRep RealFunction::rep() const
{
   return rep_;
}

Interval RealFunction::getImage() const
{
   return rep_->getImage();
}

Scope RealFunction::scope() const
{
   return rep_->scope();
}

size_t RealFunction::nbVars() const
{
   return rep_->nbVars();
}

double RealFunction::eval(const RealPoint &pt)
{
   return rep_->eval(pt);
}

void RealFunction::diff(const RealPoint &pt, RealVector &G)
{
   return rep_->diff(pt, G);
}

/*----------------------------------------------------------------------------*/

RealFunctionDag::RealFunctionDag(SharedDagFun f)
    : RealFunctionRep()
    , f_(f)
{
}

RealFunctionDag::RealFunctionDag(Term t, const Interval &img)
    : f_(nullptr)
{
   SharedDag dag = std::make_shared<Dag>();
   size_t i = dag->insert(t, img);
   f_ = dag->fun(i);
}

Interval RealFunctionDag::getImage() const
{
   return f_->getImage();
}

Scope RealFunctionDag::scope() const
{
   return f_->scope();
}

size_t RealFunctionDag::nbVars() const
{
   return f_->nbVars();
}

double RealFunctionDag::eval(const RealPoint &pt)
{
   return f_->rEval(pt);
}

void RealFunctionDag::diff(const RealPoint &pt, RealVector &G)
{
   return f_->rDiff(pt, G);
}

} // namespace realpaver
