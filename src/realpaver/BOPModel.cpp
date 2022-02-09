///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOPModel.hpp"
#include "realpaver/TermDeriver.hpp"

namespace realpaver {

BOPModel::BOPModel(Problem& problem)
      : dag_(nullptr),
        initreg_(problem.getDomains()),
        z_(""),
        objscope_(),
        fullscope_()
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
      THROW_IF(!to.dependsOn(v), "variable " << v.getName()
                                             << " does not occur "
                                             << "in the objective function");

      TermDeriver deriver(v);
      to.acceptVisitor(deriver);

      // insertion of the equation in the dag
      dag_->insert(deriver.getDerivative() == 0);

      objscope_.insert(v);
      fullscope_.insert(v);
   }

   // variable representing the objective function
   z_ = problem.addRealVar(Interval::universe(), "_z");
   fullscope_.insert(z_);

   //LOG_INFO("   > creates an objective variable " << z_.name());

   if (problem.getObjective().isMinimization())
      dag_->insert( z_ - to == 0 );

   else
      dag_->insert( z_ + to == 0 );
}

BOPModel::~BOPModel()
{
   delete dag_;
}

Variable BOPModel::getObjVar() const
{
   return z_;
}

Scope BOPModel::getObjScope() const
{
   return objscope_;
}

Scope BOPModel::getFullScope() const
{
   return fullscope_;
}

Dag* BOPModel::getDag()
{
   return dag_;
}

IntervalVector BOPModel::getInitRegion() const
{
   return initreg_;
}

size_t BOPModel::dim() const
{
   return objscope_.size();
}

double BOPModel::realEval(const RealVector& x)
{
   // equation representing the objective function
   // z +/- obj = 0
   DagFun* f = dag_->fun(dim());
   f->reval(x);

   // finds the root node of the objective function
   return f->node(f->nbNode() - 2)->rval();
}

void BOPModel::realDiff(const RealVector& x, RealVector& g)
{
   ASSERT(g.size() == dim(), "gradient with a bad dimension");

   // evaluates the dag
   dag_->reval(x);

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->rval());   
}

bool BOPModel::isDifferentiable() const
{
   return true;
}

double BOPModel::realEvalDiff(const RealVector& x, RealVector& g)
{
   // evaluates the dag
   dag_->reval(x);

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->rval());   

   // finds the value
   DagFun* f = dag_->fun(dim());
   return f->node(f->nbNode() - 2)->rval();
}

} // namespace
