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
 * @file   IntervalFunction.cpp
 * @brief  Interval functions
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalFunction.hpp"

namespace realpaver {

IntervalFunctionRep::~IntervalFunctionRep()
{
}

void IntervalFunctionRep::setImage(const Interval &img)
{
   img_ = img;
}

Interval IntervalFunctionRep::getImage() const
{
   return img_;
}

/*----------------------------------------------------------------------------*/

IntervalFunction::IntervalFunction(SharedDag dag, size_t i)
    : rep_(std::make_shared<IntervalFunctionDag>(dag, i))
{
}

IntervalFunction::IntervalFunction(Term t, const Interval &img)
    : rep_(std::make_shared<IntervalFunctionDag>(t, img))
{
}

IntervalFunction::IntervalFunction(SharedRep rep)
    : rep_(rep)
{
   ASSERT(rep != nullptr, "Creation of an interval function from a null pointer");
}

IntervalFunction::SharedRep IntervalFunction::rep() const
{
   return rep_;
}

void IntervalFunction::setImage(const Interval &img)
{
   rep_->setImage(img);
}

Interval IntervalFunction::getImage() const
{
   return rep_->getImage();
}

Scope IntervalFunction::scope() const
{
   return rep_->scope();
}

size_t IntervalFunction::nbVars() const
{
   return rep_->nbVars();
}

Interval IntervalFunction::eval(const IntervalBox &B)
{
   return rep_->eval(B);
}

void IntervalFunction::diff(const IntervalBox &B, IntervalVector &G)
{
   return rep_->diff(B, G);
}

void IntervalFunction::diffHansen(const IntervalBox &B, const RealPoint &c,
                                  IntervalVector &G)
{
   rep_->diffHansen(B, c, G);
}

/*----------------------------------------------------------------------------*/

IntervalFunctionDag::IntervalFunctionDag(SharedDag dag, size_t i)
    : IntervalFunctionRep()
    , dag_(dag)
    , index_(i)
{
   ASSERT(dag_ != nullptr, "Null pointer used to create an interval function");
   ASSERT(i < dag_->nbFuns(), "Bad index used to create an interval function");
}

IntervalFunctionDag::IntervalFunctionDag(Term t, const Interval &img)
    : dag_(nullptr)
    , index_(0)
{
   dag_ = std::make_shared<Dag>();
   index_ = dag_->insert(t, img);
}

SharedDag IntervalFunctionDag::dag() const
{
   return dag_;
}

size_t IntervalFunctionDag::index() const
{
   return index_;
}

Scope IntervalFunctionDag::scope() const
{
   return dag_->fun(index_)->scope();
}

size_t IntervalFunctionDag::nbVars() const
{
   return dag_->fun(index_)->nbVars();
}

Interval IntervalFunctionDag::eval(const IntervalBox &B)
{
   return dag_->fun(index_)->iEval(B);
}

void IntervalFunctionDag::diff(const IntervalBox &B, IntervalVector &G)
{
   DagFun *f = dag_->fun(index_);
   return f->iDiff(B, G);
}

void IntervalFunctionDag::diffHansen(const IntervalBox &B, const RealPoint &c,
                                     IntervalVector &G)
{
   DagFun *f = dag_->fun(index_);
   return f->iDiffHansen(B, c, G);
}

} // namespace realpaver
