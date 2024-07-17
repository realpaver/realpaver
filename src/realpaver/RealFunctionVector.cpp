/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   RealFunctionVector.hpp
 * @brief  Vector of real functions
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/RealFunctionVector.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

RealFunctionVectorRep::~RealFunctionVectorRep()
{}

/*----------------------------------------------------------------------------*/

RealFunctionVector::RealFunctionVector()
      : rep_(nullptr)
{}

RealFunctionVector::RealFunctionVector(SharedDag dag)
      : rep_(std::make_shared<RealFunctionVectorDag>(dag))
{}

RealFunctionVector::RealFunctionVector(
   const std::initializer_list<Term>& lt)
      : rep_(std::make_shared<RealFunctionVectorDag>(lt))
{}

RealFunctionVector::RealFunctionVector(
   const std::initializer_list<Term>& lt,
   const std::initializer_list<Interval>& li)
      : rep_(std::make_shared<RealFunctionVectorDag>(lt, li))
{}

RealFunctionVector::RealFunctionVector(
   const std::initializer_list<RealFunction>& lf)
      : rep_(std::make_shared<RealFunctionVectorList>(lf))
{}

RealFunctionVector::SharedRep RealFunctionVector::rep() const
{
   return rep_;
}

RealFunctionVector::RealFunctionVector(SharedRep rep)
      : rep_(rep)
{
   ASSERT(rep != nullptr,
          "Creation of a real function vector from a null pointer");
}

Scope RealFunctionVector::scope() const
{
   ASSERT(rep_ != nullptr, "Real function vector with no representation");

   return rep_->scope();
}

size_t RealFunctionVector::nbVars() const
{
   ASSERT(rep_ != nullptr, "Real function vector with no representation");

   return rep_->nbVars();
}

size_t RealFunctionVector::nbFuns() const
{
   ASSERT(rep_ != nullptr, "Real function vector with no representation");

   return rep_->nbFuns();
}

RealFunction RealFunctionVector::fun(size_t i) const
{
   ASSERT(rep_ != nullptr, "Real function vector with no representation");

   return rep_->fun(i);
}

void RealFunctionVector::addFun(RealFunction f)
{
   if (rep_ == nullptr)
      rep_ = std::make_shared<RealFunctionVectorList>();

   RealFunctionVectorList* p
      = dynamic_cast<RealFunctionVectorList*>(rep_.get());

   if (p != nullptr)
      p->addFun(f);

   else
   {
      // changes the representation
      SharedRep other = std::make_shared<RealFunctionVectorList>();
      p = static_cast<RealFunctionVectorList*>(other.get());

      for (size_t i=0; i<nbFuns(); ++i)
         p->addFun(fun(i));

      p->addFun(f);

      rep_ = other;
   }
}

void RealFunctionVector::eval(const RealPoint& pt, RealVector& val)
{
   ASSERT(rep_ != nullptr, "Real function vector with no representation");

   return rep_->eval(pt, val);
}

void RealFunctionVector::diff(const RealPoint& pt, RealMatrix& J)
{
   ASSERT(rep_ != nullptr, "Real function vector with no representation");

   rep_->diff(pt, J);
}

void RealFunctionVector::evalDiff(const RealPoint& pt, RealVector& val,
                                  RealMatrix& J)
{
   ASSERT(rep_ != nullptr, "Interval function vector with no representation");

   rep_->evalDiff(pt, val, J);
}

/*----------------------------------------------------------------------------*/

RealFunctionVectorDag::RealFunctionVectorDag(SharedDag dag)
      : dag_(dag)
{
   ASSERT(dag->nbFuns() > 0,
          "Creation of a real function vector from an empty Dag");
}

RealFunctionVectorDag::RealFunctionVectorDag(
      const std::initializer_list<Term>& lt)
      : dag_(nullptr)
{
   ASSERT(lt.size() > 0,
          "Creation of a real function vector from an empty list");

   dag_ = std::make_shared<Dag>();
   for (const auto& t : lt)
      dag_->insert(t);
}

RealFunctionVectorDag::RealFunctionVectorDag(
      const std::initializer_list<Term>& lt,
      const std::initializer_list<Interval>& li)
      : dag_(nullptr)
{
   ASSERT(lt.size() > 0,
          "Creation of a real function vector from an empty list");

   ASSERT(lt.size() == li.size(),
          "Bad initialization of a real function vector ");

   dag_ = std::make_shared<Dag>();

   auto it = lt.begin();
   auto jt = li.begin();
   for (; it != lt.end(); ++it, ++jt)
      dag_->insert(*it, *jt);
}

SharedDag RealFunctionVectorDag::dag() const
{
   return dag_;
}

Scope RealFunctionVectorDag::scope() const
{
   return dag_->scope();
}

size_t RealFunctionVectorDag::nbVars() const
{
   return dag_->nbVars();
}

size_t RealFunctionVectorDag::nbFuns() const
{
   return dag_->nbFuns();
}

RealFunction RealFunctionVectorDag::fun(size_t i) const
{
   ASSERT(i < nbFuns(), "Bad access to a real function in a vector");

   return RealFunction(dag_, i);
}

void RealFunctionVectorDag::eval(const RealPoint& pt, RealVector& val)
{
   dag_->realEval(pt, val);
}

void RealFunctionVectorDag::diff(const RealPoint& pt, RealMatrix& J)
{
   RealVector val(nbFuns());

   dag_->realEval(pt, val);
   dag_->realDiff(J);
}

void RealFunctionVectorDag::evalDiff(const RealPoint& pt, RealVector& val,
                                     RealMatrix& J) 
{
   dag_->realEval(pt, val);
   dag_->realDiff(J);
}

/*----------------------------------------------------------------------------*/

RealFunctionVectorList::RealFunctionVectorList()
      : vf_(),
        scop_()
{}

RealFunctionVectorList::RealFunctionVectorList(
   const std::initializer_list<RealFunction>& lf)
      : vf_(),
        scop_()
{
   for (const auto& f : lf)
      addFun(f);
}

void RealFunctionVectorList::addFun(RealFunction f)
{
   vf_.push_back(f);
   scop_.insert(f.scope());
   scop_ = ScopeBank::getInstance()->insertScope(scop_);
}

Scope RealFunctionVectorList::scope() const
{
   return scop_;
}

size_t RealFunctionVectorList::nbVars() const
{
   return scop_.size();
}

size_t RealFunctionVectorList::nbFuns() const
{
   return vf_.size();
}

RealFunction RealFunctionVectorList::fun(size_t i) const
{
   ASSERT(i<nbFuns(), "Bad access in a function vector @ " << i);
   return vf_[i];
}

void RealFunctionVectorList::eval(const RealPoint& pt, RealVector& val)
{
   ASSERT(val.size() == nbFuns(),
          "Bad size of vector given for the evaluation of a function vector");

   for (size_t i=0; i<nbFuns(); ++i)
      val[i] = vf_[i].eval(pt);
}

void RealFunctionVectorList::diff(const RealPoint& pt, RealMatrix& J)
{
   ASSERT(nbVars() == J.ncols() && nbFuns() == J.nrows(),
          "Bad dimensions of a Jacobian matrix used in a function vector");

   for (size_t i=0; i<nbFuns(); ++i)
   {
      auto& f = vf_[i];

      RealVector G(f.nbVars());
      f.diff(pt, G);

      // fills the i-th row of the matrix
      size_t j = 0;
      for (auto v : scope())
      {
         if (f.scope().contains(v))
            J.set(i, j, G.get(f.scope().index(v)));
 
         else
            J.set(i, j, 0.0);

         ++j;
      }
   }
}

void RealFunctionVectorList::evalDiff(const RealPoint& pt, RealVector& val,
                                      RealMatrix& J)
{
   ASSERT(val.size() == nbFuns(),
          "Bad size of vector given for the evaluation of a function vector");

   ASSERT(nbVars() == J.ncols() && nbFuns() == J.nrows(),
          "Bad dimensions of a Jacobian matrix used in a function vector");

   for (size_t i=0; i<nbFuns(); ++i)
   {
      auto& f = vf_[i];

      RealVector G(f.nbVars());
      f.evalDiff(pt, val[i], G);

      // fills the i-th row of the matrix
      size_t j = 0;
      for (auto v : scope())
      {
         if (f.scope().contains(v))
            J.set(i, j, G.get(f.scope().index(v)));
 
         else
            J.set(i, j, 0.0);

         ++j;
      }
   }
}

} // namespace
