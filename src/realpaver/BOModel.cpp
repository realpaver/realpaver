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
        initreg_(problem.nbVars()),
        z_(""),
        objscope_(),
        fullscope_(),
        boundary_()
{
   // objective function
   Term to = problem.getObjective().getTerm();

   // scope of the objective function
   to.makeScope(objscope_);

   // DAG
   dag_ = new Dag();
   
   // for each variable but z_creates the equation df / dv = 0
   for (size_t i=0; i<problem.nbVars(); ++i)
   {
      Variable v = problem.varAt(i);
      THROW_IF(!to.dependsOn(v), "Variable " << v.getName()
                                             << " does not occur "
                                             << "in the objective function");

      TermDeriver deriver(v);
      to.acceptVisitor(deriver);

      // insertion of the equation in the dag
      dag_->insert(deriver.getDerivative() == 0);

      objscope_.insert(v);
      boundary_.insert(v);
      fullscope_.insert(v);
      initreg_.set(v, problem.getDomain(v));
   }

   // objective function
   if (withobj)
   {
      z_ = problem.addRealVar(Interval::universe(), "_z");
      fullscope_.insert(z_);
      initreg_.push(Interval::universe());

      //LOG_INFO("   > creates an objective variable " << z_.name());

      if (problem.getObjective().isMinimization())
         dag_->insert( z_ - to == 0 );

      else
         dag_->insert( z_ + to == 0 );
   }
}

BOModel::~BOModel()
{
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

void BOModel::setBoundaryVar(const Variable& v)
{
   if (!boundary_.contains(v)) boundary_.insert(v);
}


void BOModel::setInteriorVar(const Variable& v)
{
   if (boundary_.contains(v)) boundary_.remove(v);
   
}

bool BOModel::isBoundaryVar(const Variable& v) const
{
   return boundary_.contains(v);
}

bool BOModel::isInteriorVar(const Variable& v) const
{
   return !boundary_.contains(v);
}

IntervalVector BOModel::getInitRegion() const
{
   return initreg_;
}

size_t BOModel::dim() const
{
   return objscope_.size();
}

Scope BOModel::realFunScope() const
{
   return objscope_;
}

size_t BOModel::realFunArity() const
{
   return objscope_.size();
}

double BOModel::realFunEval(const RealVector& x)
{
   // equation representing the objective function z +/- obj = 0
   DagFun* f = dag_->fun(dim());
   f->reval(x);

   // finds the root node of the objective function
   return f->node(f->nbNode() - 2)->rval();
}

void BOModel::realFunDiff(const RealVector& x, RealVector& g)
{
   ASSERT(g.size() == dim(), "Gradient with a bad dimension");

   // evaluates the dag
   dag_->reval(x);

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->rval());   
}

void BOModel::realFunEvalDiff(const RealVector& x, RealVector& g, double& val)
{
   // evaluates the dag
   dag_->reval(x);

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->rval());   

   // finds the value
   DagFun* f = dag_->fun(dim());
   val = f->node(f->nbNode() - 2)->rval();
}

Interval BOModel::intervalEval(const IntervalVector& x)
{
   // equation representing the objective function z +/- obj = 0
   DagFun* f = dag_->fun(dim());
   f->eval(x);

   // finds the root node of the objective function
   return f->node(f->nbNode() - 2)->val();
}

void BOModel::intervalDiff(const IntervalVector& x, IntervalVector& g)
{
   ASSERT(g.size() == dim(), "Gradient with a bad dimension");

   // evaluates the dag
   dag_->eval(x);

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->val());      
}

Interval BOModel::intervalEvalDiff(const IntervalVector& x, IntervalVector& g)
{
   // evaluates the dag
   dag_->eval(x);

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->val());   

   // finds the value
   DagFun* f = dag_->fun(dim());
   return f->node(f->nbNode() - 2)->val();
}

} // namespace
