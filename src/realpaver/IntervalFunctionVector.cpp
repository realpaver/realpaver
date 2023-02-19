///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalFunctionVector.hpp"

namespace realpaver {

IntervalFunctionVector::IntervalFunctionVector(SharedDag dag)
      : dag_(dag),
        val_(dag->nbFuns())
{
   ASSERT(dag->nbFuns() > 0,
          "Creation of an interval function vector from an empty Dag");
}

IntervalFunctionVector::IntervalFunctionVector(
      const std::initializer_list<Term>& lt)
      : dag_(nullptr),
        val_(lt.size())
{
   ASSERT(lt.size() > 0,
          "Creation of an interval function vector from an empty list");

   dag_ = std::make_shared<Dag>();
   for (const auto& t : lt)
      dag_->insert(t);
}

IntervalFunctionVector::IntervalFunctionVector(
      const std::initializer_list<Term>& lt,
      const std::initializer_list<Interval>& limg)
      : dag_(nullptr),
        val_(lt.size())
{
   ASSERT(lt.size() > 0,
          "Creation of an interval function vector from an empty list");

   ASSERT(lt.size() == limg.size(),
          "Bad initialiaztion of an interval function vector ");

   auto it = lt.begin();
   auto jt = limg.begin();
   for (; it != lt.end(); ++it, ++jt)
      dag_->insert(*it, *jt);
}

IntervalFunctionVector::~IntervalFunctionVector()
{}

SharedDag IntervalFunctionVector::dag() const
{
   return dag_;
}

Scope IntervalFunctionVector::scope() const
{
   return dag_->scope();
}

size_t IntervalFunctionVector::nbVars() const
{
   return dag_->nbVars();
}

size_t IntervalFunctionVector::nbFuns() const
{
   return dag_->nbFuns();
}

IntervalFunction IntervalFunctionVector::fun(size_t i) const
{
   ASSERT(i < nbFuns(), "Bad access to an interval function in a vector");

   return IntervalFunction(dag_, i);
}

const IntervalVector& IntervalFunctionVector::getValues() const
{
   return val_;
}

void IntervalFunctionVector::eval(const IntervalRegion& reg)
{
   dag_->intervalEval(reg, val_);
}

void IntervalFunctionVector::pointEval(const RealPoint& pt)
{
   dag_->intervalPointEval(pt, val_);
}

void IntervalFunctionVector::diff(const IntervalRegion& reg, IntervalMatrix& J)
{
   dag_->intervalEval(reg, val_);
   dag_->intervalDiff(J);
}

void IntervalFunctionVector::violation(const IntervalRegion& reg,
                                       IntervalVector& viol)
{
   dag_->intervalEval(reg, val_);
   dag_->intervalViolation(viol);
}

} // namespace
