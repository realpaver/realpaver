///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/RealFunctionVector.hpp"

namespace realpaver {

RealFunctionVector::RealFunctionVector(SharedDag dag)
      : dag_(dag),
        val_(dag->nbFuns())
{}

RealFunctionVector::~RealFunctionVector()
{}

SharedDag RealFunctionVector::dag() const
{
   return dag_;
}

Scope RealFunctionVector::scope() const
{
   return dag_->scope();
}

size_t RealFunctionVector::nbVars() const
{
   return dag_->nbVars();
}

size_t RealFunctionVector::nbFuns() const
{
   return dag_->nbFuns();
}

RealFunction RealFunctionVector::fun(size_t i) const
{
   ASSERT(i < nbFuns(), "Bad access to a real function in a vector");

   return RealFunction(dag_, i);
}

const RealVector& RealFunctionVector::getValues() const
{
   return val_;
}

void RealFunctionVector::eval(const RealPoint& pt)
{
   dag_->realEval(pt, val_);
}

void RealFunctionVector::diff(const RealPoint& pt, RealMatrix& J)
{
   dag_->realEval(pt, val_);
   dag_->realDiff(J);
}

void RealFunctionVector::violation(const RealPoint& pt, RealVector& viol)
{
   dag_->realEval(pt, val_);
   dag_->realViolation(viol);
}

} // namespace