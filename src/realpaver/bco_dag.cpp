// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/AssertDebug.hpp"
#include "realpaver/bco_dag.hpp"
#include "realpaver/contractor_bc4.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/term_deriver.hpp"

namespace realpaver {

BcoDag::BcoDag(Problem& P) :
   dag_(nullptr),
   init_(nullptr),
   z_(""),
   fscope_(),
   dscope_(),
   pool_(),
   propag_(&pool_)
{
   // objective function
   Term to = P.obj().getTerm();

   // scope of the objective function
   to.makeScope(fscope_);

   // DAG
   dag_ = new Dag();
   
   // for each variable but z_creates the equation df / dv = 0
   for (size_t i=0; i<P.nbVars(); ++i)
   {
      Variable v = P.varAt(i);
      THROW_IF(!to.dependsOn(v), "variable " << v.getName() << " does not occur"
                                             << " in the objective function");

      TermDeriver deriver(v);
      to.acceptVisitor(deriver);

      // insertion of the equation in the dag
      dag_->insert(deriver.get() == 0);
   }

   // variable representing the objective function
   z_ = P.addRealVar(Interval::universe(), "_z");

   //LOG_INFO("   > creates an objective variable " << z_.name());

   if (P.obj().isMinimize())
      dag_->insert( z_ - to == 0 );
   else
      dag_->insert( z_ + to == 0 );
      
         dscope_ = fscope_.clone();

   // scope of the dag
   dscope_ = dag_->scope();

   // initial box
   init_ = std::make_shared<Box>(P.getBox());
}

BcoDag::~BcoDag()
{
   delete dag_;
}

bool BcoDag::checkInitialBox() const
{
  return !init_->isEmpty();
}

bool BcoDag::checkObjRange() const
{
   DagFun* f = dag_->fun(fscope_.size());
   Interval x = f->eval(*init_);
   return !x.isEmpty();
}

bool BcoDag::checkDerivRange() const
{
   for (size_t i=0; i<fscope_.size(); ++i)
   {
      DagFun* f = dag_->fun(i);
      Interval x = f->eval(*init_);
      if (x.isEmpty())
         return false;
   }
   return true;
}

bool BcoDag::checkProblem()
{
   bool res = checkInitialBox();
   std::string log = "   > checks the initial box: ";
   log += res ? "true" : "false";
   //LOG_INFO(log);   
   //LOG_INFO("     " << *init_);

   if (res)
   {
      res = checkObjRange();
      std::string log = "   > checks the range of the objective function: ";
      log += res ? "true" : "false";
      //LOG_INFO(log);
   }

   if (res)
   {
      res = checkDerivRange();
      std::string log = "   > checks the partial derivatives: ";
      log += res ? "true" : "false";
      //LOG_INFO(log);
   }

   return res;
}

double BcoDag::reval(const RealVector& x)
{
   // equation representing the objective function
   // z +/- obj = 0
   DagFun* f = dag_->fun(dim());
   f->reval(x);

   // finds the root node of the objective function
   return f->node(f->nbNode() - 2)->rval();
}

void BcoDag::rdiff(const RealVector& x, RealVector& g)
{
   ASSERT(g.size() == dim(), "gradient with a bad dimension");

   // evaluates the dag
   dag_->reval(x);

   // fills the gradient
   for (size_t i=0; i<dim(); ++i)
      g.set(i, dag_->fun(i)->rval());   
}

void BcoDag::revalDiff(const RealVector& x, double& val, RealVector& g)
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

void BcoDag::makeDefaultPropagator()
{
   if (pool_.poolSize() == 0)
      makeHc4Propagator();
}

void BcoDag::makeHc4Propagator()
{
   size_t i = 0;

   // operators associated with df / dv = 0
   for (Variable v : fscope_)
   {
      SharedContractor op = std::make_shared<Hc4Contractor>(dag_, i++);
      SharedContractor bcop =
         std::make_shared<BcoContractor>(dag_, i, v, op, init_);

      pool_.push(bcop);
   }

   // operator associated with z_ = objective function
   SharedContractor op = std::make_shared<Hc4Contractor>(dag_, i);
   pool_.push(op);
}

void BcoDag::makeBc4Propagator()
{
   size_t i = 0;

   // operators associated with df / dv = 0
   for (Variable v : fscope_)
   {
      SharedContractor op = std::make_shared<Bc4Contractor>(dag_, i++);
      SharedContractor bcop =
         std::make_shared<BcoContractor>(dag_, i, v, op, init_);

      pool_.push(bcop);
   }

   // operator associated with z_ = objective function
   SharedContractor op = std::make_shared<Bc4Contractor>(dag_, i);
   pool_.push(op);
}


Dag* BcoDag::getDag() const
{
   return dag_;
}

Variable BcoDag::objVar() const
{
   return z_;
}

Scope BcoDag::fScope() const
{
   return fscope_;
}

Scope BcoDag::dScope() const
{
   return dscope_;
}

SharedBox BcoDag::initialBox() const
{
   return init_;
}

Propagator* BcoDag::propagator()
{
   return &propag_;
}

size_t BcoDag::dim() const
{
   return fScope().size();
}

} // namespace
