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
   }

   // objective function
   if (withobj)
   {
      z_ = problem.addRealVar(Interval::universe(), "_z");
      fullscope_.insert(z_);

      //LOG_INFO("   > creates an objective variable " << z_.name());

      if (problem.getObjective().isMinimization())
         dag_->insert( z_ - to == 0 );

      else
         dag_->insert( z_ + to == 0 );
   }

   // initial region
   init_ = new IntervalRegion(fullscope_);

   for (size_t i=0; i<problem.nbVars(); ++i)
   {
      Variable v = problem.varAt(i);
      init_->set(v, problem.getDomain(v));
   }

   if (withobj)
   {
      init_->set(z_, Interval::universe());
   }
}

BOModel::~BOModel()
{
   delete init_;
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

IntervalRegion BOModel::getInitRegion() const
{
   return *init_;
}

size_t BOModel::dim() const
{
   return objscope_.size();
}

Scope BOModel::rfunScope() const
{
   return objscope_;
}

size_t BOModel::rfunArity() const
{
   return objscope_.size();
}

double BOModel::rfunEval(const RealVector& x)
{
   // equation representing the objective function z +/- obj = 0
   DagFun* f = dag_->fun(dim());
   f->reval(x);

   // finds the root node of the objective function
   return f->node(f->nbNode() - 2)->rval();
}

void BOModel::rfunDiff(const RealVector& x, RealVector& g)
{
   ASSERT(g.size() == dim(), "Gradient with a bad dimension");

   // evaluates the dag
   dag_->reval(x);

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->rval());   
}

void BOModel::rfunEvalDiff(const RealVector& x, RealVector& g, double& val)
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

Scope BOModel::ifunScope() const
{
   return objscope_;
}

size_t BOModel::ifunArity() const
{
   return objscope_.size();
}

Interval BOModel::ifunEval(const IntervalRegion& reg)
{
   // equation representing the objective function z +/- obj = 0
   DagFun* f = dag_->fun(dim());
   f->eval(reg);

   // finds the root node of the objective function
   return f->node(f->nbNode() - 2)->val();
}

Interval BOModel::ifunEvalPoint(const RealVector& x)
{
   // equation representing the objective function z +/- obj = 0
   DagFun* f = dag_->fun(dim());
   f->eval(x);

   // finds the root node of the objective function
   return f->node(f->nbNode() - 2)->val();
}

void BOModel::ifunDiff(const IntervalRegion& reg, IntervalVector& g)
{
   ASSERT(g.size() == dim(), "Gradient with a bad dimension");

   // evaluates the dag
   dag_->eval(reg);

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->val());      
}

void BOModel::ifunEvalDiff(const IntervalRegion& reg, IntervalVector& g,
                           Interval& e)
{
   // evaluates the dag
   dag_->eval(reg);

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->val());   

   // finds the value
   DagFun* f = dag_->fun(dim());
   e = f->node(f->nbNode() - 2)->val();
}

} // namespace
