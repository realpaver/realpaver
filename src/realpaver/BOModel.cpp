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
        if_(0),
        ic_(0),
        id_(0)
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
   
   // for each variable but z_creates the equation df / dv = 0
   for (size_t i=0; i<problem.nbVars(); ++i)
   {
      Variable v = problem.varAt(i);
      THROW_IF(!to.dependsOn(v), "Variable " << v.getName()
                                             << " does not occur "
                                             << "in the objective function");

      TermDeriver deriver(v);
      tomin.acceptVisitor(deriver);

      // insertion of the equation in the dag
      dag_->insert(deriver.getDerivative() == 0);

      objscope_.insert(v);
      boundary_.insert(v);
      fullscope_.insert(v);
   }
   id_ = dag_->nbNode() - 1;

   // objective function
   if (withobj)
   {
      // objective function
      if_ = dag_->insert(tomin);

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

DEBUG("DAG : \n " << *dag_);


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

bool BOModel::isInteriorVar(Variable v) const
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

double BOModel::rfunEval(const RealPoint& pt)
{
   return dag_->fun(if_)->reval(pt);
}

void BOModel::rfunDiff(const RealPoint& pt, RealVector& g)
{
   ASSERT(g.size() == dim(), "Gradient with a bad dimension");

   // evaluates the nodes of the partial derivatives
   for (size_t i=0; i<=id_; ++i)
   {
      DagNode* node = dag_->node(i);
      node->reval(pt);
   }

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->rval());
}

void BOModel::rfunEvalDiff(const RealPoint& pt, RealVector& g, double& e)
{
   // evaluates the DAG
   for (size_t i=0; i<dag_->nbNode(); ++i)
   {
      DagNode* node = dag_->node(i);
      node->reval(pt);
   }

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->rval());   

   // finds the value
   e = dag_->fun(if_)->rootNode()->rval();
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
   return dag_->fun(if_)->eval(reg);
}

Interval BOModel::ifunEvalPoint(const RealPoint& pt)
{   
   return dag_->fun(if_)->eval(pt);
}

void BOModel::ifunDiff(const IntervalRegion& reg, IntervalVector& g)
{
   ASSERT(g.size() == dim(), "Gradient with a bad dimension");

   // evaluates the nodes of the partial derivatives
   for (size_t i=0; i<=id_; ++i)
   {
      DagNode* node = dag_->node(i);
      node->eval(reg);
   }

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->val());      
}

void BOModel::ifunEvalDiff(const IntervalRegion& reg, IntervalVector& g,
                           Interval& e)
{
   // evaluates the DAG
   for (size_t i=0; i<dag_->nbNode(); ++i)
   {
      DagNode* node = dag_->node(i);
      node->eval(reg);
   }

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->val());   

   // finds the value
   e = dag_->fun(if_)->rootNode()->val();
}

} // namespace
