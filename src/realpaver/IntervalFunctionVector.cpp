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

IntervalFunctionVectorRep::~IntervalFunctionVectorRep()
{}

///////////////////////////////////////////////////////////////////////////////

/*
IntervalFunctionVectorDag::IntervalFunctionVectorDag(SharedDag dag)
      : dag_(dag),
        val_(dag->nbFuns())
{
   ASSERT(dag->nbFuns() > 0,
          "Creation of an interval function vector from an empty Dag");
}

IntervalFunctionVectorDag::IntervalFunctionVectorDag(
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

IntervalFunctionVectorDag::IntervalFunctionVectorDag(
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

IntervalFunctionVectorDag::~IntervalFunctionVectorDag()
{}

SharedDag IntervalFunctionVectorDag::dag() const
{
   return dag_;
}

Scope IntervalFunctionVectorDag::scope() const
{
   return dag_->scope();
}

size_t IntervalFunctionVectorDag::nbVars() const
{
   return dag_->nbVars();
}

size_t IntervalFunctionVectorDag::nbFuns() const
{
   return dag_->nbFuns();
}

IntervalFunction IntervalFunctionVectorDag::fun(size_t i) const
{
   ASSERT(i < nbFuns(), "Bad access to an interval function in a vector");

   return IntervalFunction(dag_, i);
}

const IntervalVector& IntervalFunctionVectorDag::getValues() const
{
   return val_;
}

void IntervalFunctionVectorDag::eval(const IntervalRegion& reg)
{
   dag_->intervalEval(reg, val_);
}

void IntervalFunctionVectorDag::pointEval(const RealPoint& pt)
{
   dag_->intervalPointEval(pt, val_);
}

void IntervalFunctionVectorDag::diff(const IntervalRegion& reg, IntervalMatrix& J)
{
   dag_->intervalEval(reg, val_);
   dag_->intervalDiff(J);
}

void IntervalFunctionVectorDag::violation(const IntervalRegion& reg,
                                       IntervalVector& viol)
{
   dag_->intervalEval(reg, val_);
   dag_->intervalViolation(viol);
}
*/

} // namespace
