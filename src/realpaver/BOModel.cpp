///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOModel.hpp"
#include "realpaver/TermDeriver.hpp"

namespace realpaver {

BOModel::BOModel(Problem& problem, bool withobj)
      : dag_(nullptr),
        init_(nullptr),
        z_(""),
        objscope_(),
        fullscope_(),
        boundary_(),
        ic_(0),
        odag_(nullptr)
{
   bool ismin = problem.getObjective().isMinimization();

   // objective function
   Term to = problem.getObjective().getTerm();

   // objective function to be minimized
   Term tomin = ismin ? to : -to;

   // scope of the objective function
   to.makeScope(objscope_);

   // DAG
   dag_ = new Dag();
   odag_ = new Dag();
   
   // for each variable but z_creates the equation df / dv = 0 and assigns its
   // tolerance to 0, i.e. its domain is split whenever it is possible
   for (size_t i=0; i<problem.nbVars(); ++i)
   {
      Variable v = problem.varAt(i);
      v.setTolerance(Tolerance::makeAbs(0.0));

      if (!to.dependsOn(v)) continue;

      TermDeriver deriver(v);
      tomin.acceptVisitor(deriver);

DEBUG("DV : " << deriver.getDerivative());


      // insertion of the equation in the dag
      dag_->insert(deriver.getDerivative() == 0);

      objscope_.insert(v);
      boundary_.insert(v);
      fullscope_.insert(v);
   }

   // objective function
   if (withobj)
   {
      // objective function
      odag_->insert(tomin);

      // new variable representing the objective function
      z_ = problem.addRealVar(Interval::universe(), "_z");
      fullscope_.insert(z_);

      //LOG_INFO("   > creates an objective variable " << z_.name());

      bool simpl = Term::simplification(true);

      // insertion of the objective constraint in the DAG
      if (ismin)
         ic_ = dag_->insert( to - z_ == 0 );

      else
         ic_ = dag_->insert( to + z_ == 0 );

      Term::simplification(simpl);
   }

   // initial region
   init_ = new IntervalRegion(fullscope_);

   for (Variable v : objscope_)
      init_->set(v, problem.getDomain(v));

   if (withobj)
      init_->set(z_, Interval::universe());
}

BOModel::~BOModel()
{
   delete init_;
   delete odag_;
   delete dag_;
}

Variable BOModel::getObjVar() const
{
   return z_;
}

Scope BOModel::getObjScope() const
{
   return objscope_;
}

Scope BOModel::getFullScope() const
{
   return fullscope_;
}

Dag* BOModel::getDag()
{
   return dag_;
}

DagFun* BOModel::getDerivative(size_t i) const
{
   return dag_->fun(i);
}

DagFun* BOModel::getObjConstraint() const
{
   return dag_->fun(ic_);
}

void BOModel::setBoundaryVar(Variable v)
{
   if (!boundary_.contains(v)) boundary_.insert(v);
}

void BOModel::setInteriorVar(Variable v)
{
   if (boundary_.contains(v)) boundary_.remove(v);
}

bool BOModel::isBoundaryVar(Variable v) const
{
   return boundary_.contains(v);
}

bool BOModel::isBoundaryVar(Variable v, const IntervalRegion& reg) const
{
   bool res = false;

   if (isBoundaryVar(v))
   {
      Interval x(init_->get(v));
      Interval y(reg.get(v));
      if (!x.strictlyContains(y)) res = true;
   }

   return res;   
}

bool BOModel::isInteriorVar(Variable v) const
{
   return !boundary_.contains(v);
}

bool BOModel::isInteriorVar(Variable v, const IntervalRegion& reg) const
{
   return !isBoundaryVar(v, reg);
}

Bitset BOModel::makeRelaxationBitset(const IntervalRegion& reg)
{
   Bitset bs(dag_->nbFuns());
   bs.setAllOne();

   if (boundary_.isEmpty()) return bs;

   size_t i = 0;

   for (Variable v : objscope_)
   {
      if (isBoundaryVar(v, reg)) bs.setZero(i);
      i = i + 1;
   }

   return bs;
}

void BOModel::linearize(const IntervalRegion& reg, LPModel& lm)
{
   // evaluates the DAG => every node has a domain
   dag_->eval(reg);
   
   // linearizes the functions
   Bitset bs = makeRelaxationBitset(reg);
   dag_->linearize(lm, bs);

   // defines the objective function
   // assumes that the objective variable is the last variable in the DAG
   DagVar* node = dag_->varNode(dag_->nbVars()-1);
   LinVar z = lm.getLinVar(node->indexLinVar());

   LinExpr e = { {1.0}, {z} };
   lm.setObj(e, true);
}

IntervalRegion BOModel::getInitRegion() const
{
   return *init_;
}

size_t BOModel::dim() const
{
   return objscope_.size();
}

Scope BOModel::funScope() const
{
   return objscope_;
}

size_t BOModel::funArity() const
{
   return objscope_.size();
}

double BOModel::realEval(const RealPoint& pt)
{
   return odag_->fun(0)->reval(pt);
}

void BOModel::realDiff(const RealPoint& pt, RealVector& g)
{
   ASSERT(g.size() == dim(), "Gradient with a bad dimension");

   DagFun* f = odag_->fun(0);

   f->reval(pt);
   f->rdiff();
   f->toRealGradient(g);
}

double BOModel::realEvalDiff(const RealPoint& pt, RealVector& g)
{
   ASSERT(g.size() == dim(), "Gradient with a bad dimension");

   DagFun* f = odag_->fun(0);

   double e = f->reval(pt);
   f->rdiff();
   f->toRealGradient(g);
   return e;
}

Interval BOModel::intervalEval(const IntervalRegion& reg)
{
   return odag_->fun(0)->eval(reg);
}

Interval BOModel::intervalPointEval(const RealPoint& pt)
{   
   return odag_->fun(0)->eval(pt);
}

void BOModel::intervalDiff(const IntervalRegion& reg, IntervalVector& g)
{
   ASSERT(g.size() == dim(), "Gradient with a bad dimension");

   DagFun* f = odag_->fun(0);

   f->eval(reg);
   f->diff();
   f->toIntervalGradient(g);
}

Interval BOModel::intervalEvalDiff(const IntervalRegion& reg,
                                   IntervalVector& g)
{
   ASSERT(g.size() == dim(), "Gradient with a bad dimension");

   DagFun* f = odag_->fun(0);

   Interval e = f->eval(reg);
   f->diff();
   f->toIntervalGradient(g);
   return e;
}

} // namespace
