///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/RealFunction.hpp"

namespace realpaver {

RealFunctionRep::~RealFunctionRep()
{}

void RealFunctionRep::setImage(const Interval& img)
{
   img_ = img;
}

Interval RealFunctionRep::getImage() const
{
   return img_;
}

///////////////////////////////////////////////////////////////////////////////

RealFunction::RealFunction(SharedDag dag, size_t i)
      : rep_(std::make_shared<RealFunctionDag>(dag, i))
{}

RealFunction::RealFunction(Term t, const Interval& img)
      : rep_(std::make_shared<RealFunctionDag>(t, img))
{}

RealFunction::RealFunction(SharedRep rep)
      : rep_(rep)
{
   ASSERT(rep != nullptr,
          "Creation of an interval function from a null pointer");
}

RealFunction::SharedRep RealFunction::rep() const
{
   return rep_;
}

void RealFunction::setImage(const Interval& img)
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

double RealFunction::eval(const RealPoint& pt)
{
   return rep_->eval(pt);
}

void RealFunction::diff(const RealPoint& pt, RealVector& grad)
{
   return rep_->diff(pt, grad);
}

void RealFunction::evalDiff(const RealPoint& pt, double& val, RealVector& grad)
{
   return rep_->evalDiff(pt, val, grad);
}

void RealFunction::violation(const RealPoint& pt, double& val, double& viol)
{
   rep_->violation(pt, val, viol);
}

void RealFunction::violation(const RealPoint& pt, double lo, double up, double& val,
                             double& viol)
{
   rep_->violation(pt, lo, up, val, viol);
}

///////////////////////////////////////////////////////////////////////////////

RealFunctionDag::RealFunctionDag(SharedDag dag, size_t i)
      : RealFunctionRep(),
        dag_(dag),
        index_(i)
{
   ASSERT(dag_ != nullptr, "Null pointer used to create a real function");
   ASSERT(i < dag_->nbFuns(), "Bad index used to create a real function");
}

RealFunctionDag::RealFunctionDag(Term t, const Interval& img)
      : dag_(nullptr),
        index_(0)
{
   dag_ = std::make_shared<Dag>();
   index_ = dag_->insert(t, img);
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

double RealFunctionDag::eval(const RealPoint& pt)
{
   return dag_->fun(index_)->realEval(pt);
}

void RealFunctionDag::diff(const RealPoint& pt, RealVector& grad)
{
   double val;
   evalDiff(pt, val, grad);
}

void RealFunctionDag::evalDiff(const RealPoint& pt, double& val,
                               RealVector& grad)
{
   ASSERT(nbVars() == grad.size(), "Bad size of gradient");

   DagFun* f = dag_->fun(index_);
   val = f->realEval(pt);
   f->realDiff(grad);
}

void RealFunctionDag::violation(const RealPoint& pt, double& val, double& viol)
{
   DagFun* f = dag_->fun(index_);
   val = f->realEval(pt);
   viol = f->realViolation();   
}

void RealFunctionDag::violation(const RealPoint& pt, double lo, double up,
                                double& val, double& viol)
{
   Interval img(lo, up);
   ASSERT(!img.isEmpty(), "Empty image for a real function");

   DagFun* f = dag_->fun(index_);
   Interval tmp = f->getImage();
   f->setImage(img);

   val = f->realEval(pt);
   viol = f->realViolation();

   f->setImage(tmp);
}

} // namespace
