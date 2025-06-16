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
 * @date   2024-4-11
 */

#include "realpaver/RealFunction.hpp"
#include "realpaver/AssertDebug.hpp"

namespace realpaver {

RealFunctionRep::~RealFunctionRep()
{
}

void RealFunctionRep::setImage(const Interval &img)
{
   img_ = img;
}

Interval RealFunctionRep::getImage() const
{
   return img_;
}

/*----------------------------------------------------------------------------*/

RealFunction::RealFunction(SharedDag dag, size_t i)
    : rep_(std::make_shared<RealFunctionDag>(dag, i))
{
}

RealFunction::RealFunction(Term t, const Interval &img)
    : rep_(std::make_shared<RealFunctionDag>(t, img))
{
}

RealFunction::RealFunction(SharedRep rep)
    : rep_(rep)
{
   ASSERT(rep != nullptr, "Creation of an interval function from a null pointer");
}

RealFunction::SharedRep RealFunction::rep() const
{
   return rep_;
}

void RealFunction::setImage(const Interval &img)
{
   rep_->setImage(img);
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

RealFunctionDag::RealFunctionDag(SharedDag dag, size_t i)
    : RealFunctionRep()
    , dag_(dag)
    , index_(i)
{
   ASSERT(dag_ != nullptr, "Null pointer used to create a real function");
   ASSERT(i < dag_->nbFuns(), "Bad index used to create a real function");

   img_ = dag->fun(i)->getImage();
}

RealFunctionDag::RealFunctionDag(Term t, const Interval &img)
    : dag_(nullptr)
    , index_(0)
{
   dag_ = std::make_shared<Dag>();
   index_ = dag_->insert(t, img);
   img_ = img;
}

SharedDag RealFunctionDag::dag() const
{
   return dag_;
}

size_t RealFunctionDag::index() const
{
   return index_;
}

Scope RealFunctionDag::scope() const
{
   return dag_->fun(index_)->scope();
}

size_t RealFunctionDag::nbVars() const
{
   return dag_->fun(index_)->nbVars();
}

double RealFunctionDag::eval(const RealPoint &pt)
{
   return dag_->fun(index_)->rEval(pt);
}

void RealFunctionDag::diff(const RealPoint &pt, RealVector &G)
{
   DagFun *f = dag_->fun(index_);
   return f->rDiff(pt, G);
}

} // namespace realpaver
