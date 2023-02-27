///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalFunction.hpp"

namespace realpaver {

IntervalFunctionRep::~IntervalFunctionRep()
{}

void IntervalFunctionRep::setImage(const Interval& img)
{
   img_ = img;
}

Interval IntervalFunctionRep::getImage() const
{
   return img_;
}

///////////////////////////////////////////////////////////////////////////////

IntervalFunction::IntervalFunction(SharedDag dag, size_t i)
      : rep_(std::make_shared<IntervalFunctionDag>(dag, i))
{}

IntervalFunction::IntervalFunction(Term t, const Interval& img)
      : rep_(std::make_shared<IntervalFunctionDag>(t, img))
{}

IntervalFunction::IntervalFunction(SharedRep rep)
      : rep_(rep)
{
   ASSERT(rep != nullptr,
          "Creation of an interval function from a null pointer");
}

IntervalFunction::SharedRep IntervalFunction::rep() const
{
   return rep_;
}

void IntervalFunction::setImage(const Interval& img)
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

Interval IntervalFunction::eval(const IntervalRegion& reg)
{
   return rep_->eval(reg);
}

Interval IntervalFunction::pointEval(const RealPoint& pt)
{
   return rep_->pointEval(pt);
}

void IntervalFunction::diff(const IntervalRegion& reg, IntervalVector& grad)
{
   return rep_->diff(reg, grad);
}

void IntervalFunction::evalDiff(const IntervalRegion& reg, Interval& val,
                                IntervalVector& grad)
{
   return rep_->evalDiff(reg, val, grad);   
}

void IntervalFunction::violation(const IntervalRegion& reg, Interval& val,
                                 double& viol)
{
   rep_->violation(reg, val, viol);
}

void IntervalFunction::violation(const IntervalRegion& reg, double lo,
                                 double up, Interval& val, double& viol)
{
   rep_->violation(reg, lo, up, val, viol);
}

///////////////////////////////////////////////////////////////////////////////

IntervalFunctionDag::IntervalFunctionDag(SharedDag dag, size_t i)
      : IntervalFunctionRep(),
        dag_(dag),
        index_(i)
{
   ASSERT(dag_ != nullptr, "Null pointer used to create an interval function");
   ASSERT(i < dag_->nbFuns(), "Bad index used to create an interval function");
}

IntervalFunctionDag::IntervalFunctionDag(Term t, const Interval& img)
      : dag_(nullptr),
        index_(0)
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

Interval IntervalFunctionDag::eval(const IntervalRegion& reg)
{
   return dag_->fun(index_)->intervalEval(reg);
}

Interval IntervalFunctionDag::pointEval(const RealPoint& pt)
{
   return dag_->fun(index_)->intervalEval(pt);
}

void IntervalFunctionDag::violation(const IntervalRegion& reg, Interval& val,
                                    double& viol)
{
   DagFun* f = dag_->fun(index_);
   val = f->intervalEval(reg);
   viol = f->intervalViolation();
}

void IntervalFunctionDag::violation(const IntervalRegion& reg, double lo,
                                    double up, Interval& val, double& viol)
{
   Interval img(lo, up);
   ASSERT(!img.isEmpty(), "Empty image for an interval function");

   DagFun* f = dag_->fun(index_);
   Interval tmp = f->getImage();
   f->setImage(img);

   val = f->intervalEval(reg);
   viol = f->intervalViolation();

   f->setImage(tmp);
}

void IntervalFunctionDag::diff(const IntervalRegion& reg, IntervalVector& grad)
{
   Interval val;
   evalDiff(reg, val, grad);
}

void IntervalFunctionDag::evalDiff(const IntervalRegion& reg, Interval& val,
                                   IntervalVector& grad)
{
   ASSERT(nbVars() == grad.size(), "Bad size of gradient");

   DagFun* f = dag_->fun(index_);
   val = f->intervalEval(reg);
   if (val.isEmpty())
      grad.setEmpty();

   else
      f->intervalDiff(grad);
}

} // namespace
