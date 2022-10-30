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
        val_(dag->nbFuns()),
        jac_(dag->nbFuns(), dag->nbVars())
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

const RealVector& RealFunctionVector::values() const
{
   return val_;
}

const RealMatrix& RealFunctionVector::jacobian() const
{
   return jac_;
}

void RealFunctionVector::eval(const RealPoint& pt)
{
   dag_->realEval(pt, val_);
}

void RealFunctionVector::diff(const RealPoint& pt)
{
   dag_->realEval(pt, val_);
   dag_->realDiff(jac_);
}

} // namespace
