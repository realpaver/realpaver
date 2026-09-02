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
 * @file   DagExtended.cpp
 * @brief  DAG representation of a problem
 * @author Laurent Granvilliers
 * @date   3 Apr 2026
 */

#include "RealPoint.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/ConstraintCond.hpp"
#include "realpaver/ConstraintPiecewise.hpp"
#include "realpaver/DagExtended.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

DagFunExt::DagFunExt(SharedDag dag, Constraint ctr)
    : dag_(dag)
    , ctr_(ctr)
{
}

DagFunExt::~DagFunExt()
{
}

Constraint DagFunExt::ctr() const
{
   return ctr_;
}

SharedDag DagFunExt::dag() const
{
   return dag_;
}

SharedDagFun DagFunExt::getActive(const IntervalBox &B, const Bitset &active)
{
   return (active.isEmpty() || active.get(ctr_.id())) ? getActiveImpl(B) : nullptr;
}

SharedDagFun DagFunExt::getActive(const RealPoint &P, const Bitset &active)
{
   // LOG_LOW("GET ACTIVE: " << (active.isEmpty() || active.get(ctr_.id())))
   return (active.isEmpty() || active.get(ctr_.id())) ? getActiveImpl(P) : nullptr;
}

/*----------------------------------------------------------------------------*/

DagFunArith::DagFunArith(SharedDag dag, Constraint ctr, int i)
    : DagFunExt(dag, ctr)
    , idx_(i)
{
}

int DagFunArith::index() const
{
   return idx_;
}

SharedDagFun DagFunArith::getActiveImpl(const IntervalBox &B)
{
   return dag_->fun(idx_);
}

SharedDagFun DagFunArith::getActiveImpl(const RealPoint &P)
{
   return dag_->fun(idx_);
}

void DagFunArith::acceptVisitor(DagFunExtVisitor &vis) const
{
   vis.apply(this);
}

/*----------------------------------------------------------------------------*/

DagFunCond::DagFunCond(SharedDag dag, Constraint ctr, int i)
    : DagFunExt(dag, ctr)
    , idx_(i)
{
}

SharedDagFun DagFunCond::getActiveImpl(const IntervalBox &B)
{
   ConstraintCond *cc = static_cast<ConstraintCond *>(ctr_.rep().get());
   return ((cc->guard().isSatisfied(B) == Proof::Inner) && (idx_ >= 0)) ? dag_->fun(idx_)
                                                                        : nullptr;
}

SharedDagFun DagFunCond::getActiveImpl(const RealPoint &P)
{
   ConstraintCond *cc = static_cast<ConstraintCond *>(ctr_.rep().get());
   return ((cc->guard().isSatisfied(P) == Proof::Inner) && (idx_ >= 0)) ? dag_->fun(idx_)
                                                                        : nullptr;
}

void DagFunCond::acceptVisitor(DagFunExtVisitor &vis) const
{
   vis.apply(this);
}

/*----------------------------------------------------------------------------*/

DagFunPiecewise::DagFunPiecewise(SharedDag dag, Constraint ctr)
    : DagFunExt(dag, ctr)
    , idx_()
{
}

void DagFunPiecewise::addIndex(int i)
{
   idx_.push_back(i);
}

SharedDagFun DagFunPiecewise::getActiveImpl(const IntervalBox &B)
{
   ConstraintPiecewise *cp = static_cast<ConstraintPiecewise *>(ctr_.rep().get());
   Variable v = cp->variable();

   for (size_t i = 0; i < cp->nbPieces(); ++i)
   {
      if (idx_[i] < 0)
         continue;

      if (B.get(cp->binary(i)).isOne() || cp->interval(i).contains(B.get(v)))
      {
         return dag_->fun(idx_[i]);
      }
   }
   return nullptr;
}

SharedDagFun DagFunPiecewise::getActiveImpl(const RealPoint &P)
{
   ConstraintPiecewise *cp = static_cast<ConstraintPiecewise *>(ctr_.rep().get());
   Variable v = cp->variable();

   for (size_t i = 0; i < cp->nbPieces(); ++i)
   {
      if (idx_[i] < 0)
         continue;

      if (P.get(cp->binary(i)) == 1 || cp->interval(i).contains(P.get(v)))
      {
         return dag_->fun(idx_[i]);
      }
   }
   return nullptr;
}

void DagFunPiecewise::acceptVisitor(DagFunExtVisitor &vis) const
{
   vis.apply(this);
}

/*----------------------------------------------------------------------------*/

DagFunExtVisitor::~DagFunExtVisitor()
{
}

void DagFunExtVisitor::apply(const DagFunArith *f)
{
   THROW("Visit method not implemented");
}

void DagFunExtVisitor::apply(const DagFunCond *f)
{
   THROW("Visit method not implemented");
}

void DagFunExtVisitor::apply(const DagFunPiecewise *f)
{
   THROW("Visit method not implemented");
}

/*----------------------------------------------------------------------------*/

DagExtended::DagExtended(const Problem &pbm)
    : dag_(nullptr)
    , lfun_()
    , lctr_()
    , scope_(pbm.scope())
{
   dag_ = std::make_shared<Dag>();
   for (size_t i = 0; i < pbm.nbCtrs(); ++i)
   {
      Constraint ctr = pbm.ctrAt(i);
      DagExtendedCreator creator(this, ctr);
      ctr.acceptVisitor(creator);

      if (!creator.success())
         lctr_.push_back(ctr);
   }
}

SharedDag DagExtended::dag() const
{
   return dag_;
}

Scope DagExtended::scope() const
{
   // return dag_->scope();
   return scope_;
}

size_t DagExtended::size() const
{
   return lfun_.size();
}

bool DagExtended::isEmpty() const
{
   return size() == 0;
}

std::shared_ptr<DagFunExt> DagExtended::fun(size_t i) const
{
   return lfun_[i];
}

void DagExtended::addFun(std::shared_ptr<DagFunExt> f)
{
   lfun_.push_back(std::move(f));
}

SharedDagFun DagExtended::getActiveFun(size_t i, const IntervalBox &B,
                                       const Bitset &active)
{
   return lfun_[i]->getActive(B, active);
}

SharedDagFun DagExtended::getActiveFun(size_t i, const RealPoint &P, const Bitset &active)
{
   return lfun_[i]->getActive(P, active);
}

size_t DagExtended::nbOutsideCtrs() const
{
   return lctr_.size();
}

Constraint DagExtended::outsideCtr(size_t i) const
{
   return lctr_[i];
}

/*----------------------------------------------------------------------------*/

DagExtendedCreator::DagExtendedCreator(DagExtended *edag_, Constraint ctr)
    : edag_(edag_)
    , ctr_(ctr)
    , success_(true)
{
}

bool DagExtendedCreator::success() const
{
   return success_;
}

void DagExtendedCreator::apply(const ConstraintCond *c)
{
   SharedDag dag = edag_->dag();
   int i = -1;
   try
   {
      i = dag->insert(c->body());
   }
   catch (Exception &e)
   {
   }
   std::shared_ptr<DagFunCond> f = std::make_shared<DagFunCond>(dag, ctr_, i);
   edag_->addFun(std::move(f));
}

void DagExtendedCreator::apply(const ConstraintTable *c)
{
   success_ = false;
}

void DagExtendedCreator::apply(const ConstraintPiecewise *c)
{
   SharedDag dag = edag_->dag();
   std::shared_ptr<DagFunPiecewise> f = std::make_shared<DagFunPiecewise>(dag, ctr_);

   for (size_t i = 0; i < c->nbPieces(); ++i)
   {
      int j = -1;
      try
      {
         j = dag->insert(c->constraint(i));
      }
      catch (Exception &e)
      {
      }
      f->addIndex(j);
   }
   edag_->addFun(std::move(f));
}

void DagExtendedCreator::apply(const ConstraintEq *c)
{
   makeArith();
}

void DagExtendedCreator::apply(const ConstraintLe *c)
{
   makeArith();
}

void DagExtendedCreator::apply(const ConstraintLt *c)
{
   makeArith();
}

void DagExtendedCreator::apply(const ConstraintGe *c)
{
   makeArith();
}

void DagExtendedCreator::apply(const ConstraintGt *c)
{
   makeArith();
}

void DagExtendedCreator::apply(const ConstraintIn *c)
{
   makeArith();
}

void DagExtendedCreator::makeArith()
{
   SharedDag dag = edag_->dag();
   int i = dag->insert(ctr_);
   std::shared_ptr<DagFunArith> f = std::make_shared<DagFunArith>(dag, ctr_, i);
   edag_->addFun(std::move(f));
}

} // namespace realpaver
