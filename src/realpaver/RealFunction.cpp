///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/RealFunction.hpp"

namespace realpaver {

RealFunction::RealFunction(SharedDag dag, size_t i)
      : dag_(dag),
        index_(i),
        val_(),
        grad_()
{
   ASSERT(dag_ != nullptr, "Null pointer used to create a real function");
   ASSERT(i < dag_->nbFuns(), "Bad index used to create a real function");

   grad_ = RealVector(dag_->fun(i)->nbVars());
}

RealFunction::~RealFunction()
{}

SharedDag RealFunction::dag() const
{
   return dag_;
}

Scope RealFunction::scope() const
{
   return dag_->fun(index_)->scope();
}

size_t RealFunction::nbVars() const
{
   return dag_->fun(index_)->nbVars();   
}

Interval RealFunction::getImage() const
{
   return dag_->fun(index_)->getImage();   
}

const RealVector& RealFunction::gradient() const
{
   return grad_;
}

double RealFunction::value() const
{
   return val_;
}

double RealFunction::eval(const RealPoint& pt)
{
   val_ = dag_->fun(index_)->realEval(pt);
   return val_;
}

double RealFunction::violation(const RealPoint& pt)
{
   return dag_->fun(index_)->realViolation(pt);
}

double RealFunction::violation(const RealPoint& pt, double lo, double up)
{
   Interval img(lo, up);
   ASSERT(!img.isEmpty(), "Empty image for a real function");

   DagFun* f = dag_->fun(index_);
   Interval tmp = f->getImage();
   f->setImage(img);
   double v = dag_->fun(index_)->realViolation(pt);
   f->setImage(tmp);
   return v;
}

void RealFunction::diff(const RealPoint& pt)
{
   DagFun* f = dag_->fun(index_);
   val_ = f->realEval(pt);
   f->realDiff(grad_);
}

} // namespace
