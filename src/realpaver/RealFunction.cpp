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
        val_()
{
   ASSERT(dag_ != nullptr, "Null pointer used to create a real function");
   ASSERT(i < dag_->nbFuns(), "Bad index used to create a real function");
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

double RealFunction::getValue() const
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
   DagFun* f = dag_->fun(index_);
   val_ = f->realEval(pt);
   return f->realViolation();
}

double RealFunction::violation(const RealPoint& pt, double lo, double up)
{
   Interval img(lo, up);
   ASSERT(!img.isEmpty(), "Empty image for a real function");

   DagFun* f = dag_->fun(index_);
   Interval tmp = f->getImage();
   f->setImage(img);

   val_ = f->realEval(pt);
   double v = f->realViolation();

   f->setImage(tmp);
   return v;
}

void RealFunction::diff(const RealPoint& pt, RealVector& grad)
{
   ASSERT(nbVars() == grad.size(), "Bad size of gradient");

   DagFun* f = dag_->fun(index_);
   val_ = f->realEval(pt);
   f->realDiff(grad);
}

} // namespace
