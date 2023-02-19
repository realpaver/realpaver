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
        val_()
{
   ASSERT(dag_ != nullptr, "Null pointer used to create an interval function");
   ASSERT(i < dag_->nbFuns(), "Bad index used to create an interval function");
}

IntervalFunction::IntervalFunction(Term t, const Interval& img)
      : dag_(nullptr),
        index_(0),
        val_()
{
   dag_ = std::make_shared<Dag>();
   index_ = dag_->insert(t, img);
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

Interval IntervalFunction::getImage() const
{
   return dag_->fun(index_)->getImage();
}

Interval IntervalFunction::getValue() const
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

double IntervalFunction::violation(const IntervalRegion& reg)
{
   DagFun* f = dag_->fun(index_);
   val_ = f->intervalEval(reg);
   return f->intervalViolation();
}

double IntervalFunction::violation(const IntervalRegion& reg, double lo,
                                   double up)
{
   Interval img(lo, up);
   ASSERT(!img.isEmpty(), "Empty image for an interval function");

   DagFun* f = dag_->fun(index_);
   Interval tmp = f->getImage();
   f->setImage(img);

   val_ = f->intervalEval(reg);
   double v = f->intervalViolation();

   f->setImage(tmp);
   return v;
}

void IntervalFunction::diff(const IntervalRegion& reg, IntervalVector& grad)
{
   ASSERT(nbVars() == grad.size(), "Bad size of gradient");

   DagFun* f = dag_->fun(index_);
   val_ = f->intervalEval(reg);
   f->intervalDiff(grad);
}

} // namespace
