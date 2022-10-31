///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalFunction.hpp"

namespace realpaver {

IntervalFunction::IntervalFunction(SharedDag dag, size_t i)
      : dag_(dag),
        index_(i),
        val_(),
        grad_()
{
   ASSERT(dag_ != nullptr, "Null pointer used to create an interval function");
   ASSERT(i < dag_->nbFuns(), "Bad index used to create an interval function");

   grad_ = IntervalVector(dag_->fun(i)->nbVars());
}

IntervalFunction::~IntervalFunction()
{}

SharedDag IntervalFunction::dag() const
{
   return dag_;
}

Scope IntervalFunction::scope() const
{
   return dag_->fun(index_)->scope();
}

size_t IntervalFunction::nbVars() const
{
   return dag_->fun(index_)->nbVars();   
}

const IntervalVector& IntervalFunction::gradient() const
{
   return grad_;
}

Interval IntervalFunction::value() const
{
   return val_;
}

Interval IntervalFunction::eval(const IntervalRegion& reg)
{
   val_ = dag_->fun(index_)->intervalEval(reg);
   return val_;
}

Interval IntervalFunction::pointEval(const RealPoint& pt)
{
   val_ = dag_->fun(index_)->intervalEval(pt);
   return val_;
}

void IntervalFunction::diff(const IntervalRegion& reg)
{
   DagFun* f = dag_->fun(index_);
   val_ = f->intervalEval(reg);
   f->intervalDiff(grad_);
}

} // namespace
