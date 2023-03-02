///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
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

IntervalFunctionVector::IntervalFunctionVector()
      : rep_(nullptr)
{}

IntervalFunctionVector::IntervalFunctionVector(SharedDag dag)
      : rep_(std::make_shared<IntervalFunctionVectorDag>(dag))
{}

IntervalFunctionVector::IntervalFunctionVector(
   const std::initializer_list<Term>& lt)
      : rep_(std::make_shared<IntervalFunctionVectorDag>(lt))
{}

IntervalFunctionVector::IntervalFunctionVector(
   const std::initializer_list<Term>& lt,
   const std::initializer_list<Interval>& li)
      : rep_(std::make_shared<IntervalFunctionVectorDag>(lt, li))
{}

IntervalFunctionVector::IntervalFunctionVector(
   const std::initializer_list<IntervalFunction>& lf)
      : rep_(std::make_shared<IntervalFunctionVectorList>(lf))
{}

IntervalFunctionVector::SharedRep IntervalFunctionVector::rep() const
{
   return rep_;
}

IntervalFunctionVector::IntervalFunctionVector(SharedRep rep)
      : rep_(rep)
{
   ASSERT(rep != nullptr,
          "Creation of an interval function vector from a null pointer");
}

Scope IntervalFunctionVector::scope() const
{
   ASSERT(rep_ != nullptr, "Interval function vector with no representation");

   return rep_->scope();
}

size_t IntervalFunctionVector::nbVars() const
{
   if (rep_ == nullptr)
      return 0;

   else
      return rep_->nbVars();
}

size_t IntervalFunctionVector::nbFuns() const
{
   if (rep_ == nullptr)
      return 0;

   else
      return rep_->nbFuns();
}

bool IntervalFunctionVector::isSquare() const
{
   return nbVars() == nbFuns() && nbVars() > 0;
}

IntervalFunction IntervalFunctionVector::fun(size_t i) const
{
   ASSERT(rep_ != nullptr, "Interval function vector with no representation");

   return rep_->fun(i);
}

void IntervalFunctionVector::addFun(IntervalFunction f)
{
   if (rep_ == nullptr)
      rep_ = std::make_shared<IntervalFunctionVectorList>();

   IntervalFunctionVectorList* p
      = dynamic_cast<IntervalFunctionVectorList*>(rep_.get());

   if (p != nullptr)
      p->addFun(f);

   else
   {
      // changes the representation
      SharedRep other = std::make_shared<IntervalFunctionVectorList>();
      p = static_cast<IntervalFunctionVectorList*>(other.get());

      for (size_t i=0; i<nbFuns(); ++i)
         p->addFun(fun(i));

      p->addFun(f);

      rep_ = other;
   }
}

void IntervalFunctionVector::eval(const IntervalRegion& reg,
                                  IntervalVector& val)
{
   ASSERT(rep_ != nullptr, "Interval function vector with no representation");

   return rep_->eval(reg, val);
}

void IntervalFunctionVector::pointEval(const RealPoint& pt, IntervalVector& val)
{
   ASSERT(rep_ != nullptr, "Interval function vector with no representation");

   rep_->pointEval(pt, val);
}

void IntervalFunctionVector::diff(const IntervalRegion& reg, IntervalMatrix& J)
{
   ASSERT(rep_ != nullptr, "Interval function vector with no representation");

   rep_->diff(reg, J);
}

void IntervalFunctionVector::evalDiff(const IntervalRegion& reg,
                                      IntervalVector& val, IntervalMatrix& J)
{
   ASSERT(rep_ != nullptr, "Interval function vector with no representation");

   rep_->evalDiff(reg, val, J);
}

void IntervalFunctionVector::violation(const IntervalRegion& reg,
                                       IntervalVector& val,
                                       RealVector& viol)
{
   ASSERT(rep_ != nullptr, "Interval function vector with no representation");

   rep_->violation(reg, val, viol);
}

///////////////////////////////////////////////////////////////////////////////

IntervalFunctionVectorDag::IntervalFunctionVectorDag(SharedDag dag)
      : dag_(dag)
{
   ASSERT(dag->nbFuns() > 0,
          "Creation of an interval function vector from an empty Dag");
}

IntervalFunctionVectorDag::IntervalFunctionVectorDag(
      const std::initializer_list<Term>& lt)
      : dag_(nullptr)
{
   ASSERT(lt.size() > 0,
          "Creation of an interval function vector from an empty list");

   dag_ = std::make_shared<Dag>();
   for (const auto& t : lt)
      dag_->insert(t);
}

IntervalFunctionVectorDag::IntervalFunctionVectorDag(
      const std::initializer_list<Term>& lt,
      const std::initializer_list<Interval>& li)
      : dag_(nullptr)
{
   ASSERT(lt.size() > 0,
          "Creation of an interval function vector from an empty list");

   ASSERT(lt.size() == li.size(),
          "Bad initialization of an interval function vector ");

   auto it = lt.begin();
   auto jt = li.begin();
   for (; it != lt.end(); ++it, ++jt)
      dag_->insert(*it, *jt);
}

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

void IntervalFunctionVectorDag::eval(const IntervalRegion& reg,
                                     IntervalVector& val)
{
   dag_->intervalEval(reg, val);
}

void IntervalFunctionVectorDag::pointEval(const RealPoint& pt,
                                          IntervalVector& val)
{
   dag_->intervalPointEval(pt, val);
}

void IntervalFunctionVectorDag::diff(const IntervalRegion& reg,
                                     IntervalMatrix& J)
{
   IntervalVector val(nbFuns());

   dag_->intervalEval(reg, val);
   dag_->intervalDiff(J);
}

void IntervalFunctionVectorDag::evalDiff(const IntervalRegion& reg,
                                         IntervalVector& val,
                                         IntervalMatrix& J) 
{
   dag_->intervalEval(reg, val);
   dag_->intervalDiff(J);
}

void IntervalFunctionVectorDag::violation(const IntervalRegion& reg,
                                          IntervalVector& val,
                                          RealVector& viol)
{
   dag_->intervalEval(reg, val);
   dag_->intervalViolation(viol);
}

///////////////////////////////////////////////////////////////////////////////

IntervalFunctionVectorList::IntervalFunctionVectorList()
      : vf_(),
        scope_()
{}

IntervalFunctionVectorList::IntervalFunctionVectorList(
   const std::initializer_list<IntervalFunction>& lf)
      : vf_(),
        scope_()
{
   for (const auto& f : lf)
      addFun(f);
}

void IntervalFunctionVectorList::addFun(IntervalFunction f)
{
   vf_.push_back(f);
   scope_.insert(f.scope());
}

Scope IntervalFunctionVectorList::scope() const
{
   return scope_;
}

size_t IntervalFunctionVectorList::nbVars() const
{
   return scope_.size();
}

size_t IntervalFunctionVectorList::nbFuns() const
{
   return vf_.size();
}

IntervalFunction IntervalFunctionVectorList::fun(size_t i) const
{
   ASSERT(i<nbFuns(), "Bad access in a function vector @ " << i);
   return vf_[i];
}

void IntervalFunctionVectorList::eval(const IntervalRegion& reg,
                                      IntervalVector& val)
{
   ASSERT(val.size() == nbFuns(),
          "Bad size of vector given for the evaluation of a function vector");

   for (size_t i=0; i<nbFuns(); ++i)
      val[i] = vf_[i].eval(reg);
}

void IntervalFunctionVectorList::pointEval(const RealPoint& pt,
                                           IntervalVector& val)
{
   ASSERT(val.size() == nbFuns(),
          "Bad size of vector given for the evaluation of a function vector");

   for (size_t i=0; i<nbFuns(); ++i)
      val[i] = vf_[i].pointEval(pt);
}

void IntervalFunctionVectorList::diff(const IntervalRegion& reg,
                                      IntervalMatrix& J)
{
   ASSERT(nbVars() == J.ncols() && nbFuns() == J.nrows(),
          "Bad dimensions of a Jacobian matrix used in a function vector");

   for (size_t i=0; i<nbFuns(); ++i)
   {
      auto& f = vf_[i];

      IntervalVector G(f.nbVars());
      f.diff(reg, G);

      // fills the i-th row of the matrix
      size_t j = 0;
      for (auto v : scope())
      {
         if (f.scope().contains(v))
            J.set(i, j, G.get(f.scope().index(v)));
 
         else
            J.set(i, j, Interval::zero());

         ++j;
      }
   }
}

void IntervalFunctionVectorList::evalDiff(const IntervalRegion& reg,
                                          IntervalVector& val,
                                          IntervalMatrix& J)
{
   ASSERT(val.size() == nbFuns(),
          "Bad size of vector given for the evaluation of a function vector");

   ASSERT(nbVars() == J.ncols() && nbFuns() == J.nrows(),
          "Bad dimensions of a Jacobian matrix used in a function vector");

   for (size_t i=0; i<nbFuns(); ++i)
   {
      auto& f = vf_[i];

      IntervalVector G(f.nbVars());
      f.evalDiff(reg, val[i], G);

      // fills the i-th row of the matrix
      size_t j = 0;
      for (auto v : scope())
      {
         if (f.scope().contains(v))
            J.set(i, j, G.get(f.scope().index(v)));
 
         else
            J.set(i, j, Interval::zero());

         ++j;
      }
   }
}

void IntervalFunctionVectorList::violation(const IntervalRegion& reg,
                                           IntervalVector& val,
                                           RealVector& viol)
{
   for (size_t i=0; i<nbFuns(); ++i)
      vf_[i].violation(reg, val[i], viol[i]);
}

} // namespace
