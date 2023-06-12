///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/LocalSolver.hpp"
#include "realpaver/Param.hpp"

#include "realpaver/Term.hpp"
#include "realpaver/TermDeriver.hpp"

namespace realpaver {

LocalSolver::LocalSolver(const Problem& pb, bool withobj)
      : pb_(std::make_shared<Problem>(pb)),
      n_(pb.nbVars()), m_(pb.nbCtrs()), s_(pb.scope()),
      time_limit_(Param::GetDblParam("LOCAL_SOLVER_TIME_LIMIT"))
{
   // std::cout<<"LocalSolver constructor"<<std::endl;
   bool ismin = pb_->getObjective().isMinimization();

   // objective function
   Term to = pb_->getObjective().getTerm();

   // objective function to be minimized
   Term tomin = ismin ? to : -to;

   // scope of the objective function
   to.makeScope(os_);

   // DAG
   dag_ =  std::make_shared<Dag>();
   odag_ = std::make_shared<Dag>();
   odag_->insert(tomin);   // Add the objective expression to the objective dag
   obj_ = std::make_shared<RealFunction>(odag_,0);
   // std::cout<<"Min "<<*odag_<<std::endl;
   
   diff_obj_ = std::make_shared<RealFunctionVector>();

   // for each variable but z_creates the equation df / dv = 0 and assigns its
   // tolerance to 0, i.e. its domain is split whenever it is possible
   for (size_t i=0; i<pb_->nbVars(); ++i)
   {
      Variable v = pb_->varAt(i);
      v.setTolerance(Tolerance::makeAbs(0.0));

      if (!to.dependsOn(v)) continue;

      TermDeriver deriver(v);
      tomin.acceptVisitor(deriver);

// DEBUG("DV : " << deriver.getDerivative());
      // TODO: store Realfunctions for each partial derivative

      os_.insert(v); // df/dv to the objective dag
      // s_.insert(v); // df/dv to the objective dag
      odag_->insert(deriver.getDerivative());
      diff_obj_->addFun(RealFunction(odag_,i+1)); // The 1rst function in the dag is the objective function itself

   }

   // std::cout<<"Number of columns of J: "<<diff_obj_->nbFuns()<<std::endl;
   

   // Add all constraints to the constraint dag
   for (size_t j=0; j<m_; j++)
   {
      dag_->insert(pb.ctrAt(j));
      // ctrs_->addFun(RealFunction(dag_,j));
   }
   if (dag_->nbFuns()==0)
      ctrs_ = std::make_shared<RealFunctionVector>();
   else
      ctrs_ = std::make_shared<RealFunctionVector>(dag_);


   // std::cout<<"Number of constraints: "<<dag_->nbFuns()<<std::endl;

   // objective function
   if (withobj)
   {
      // new variable representing the objective function
      z_ = pb_->addRealVar(Interval::universe(), "_z");
      s_.insert(z_);

      // insertion of the objective constraint in the DAG
      if (ismin)
         ic_ = dag_->insert( to - z_ == 0 );

      else
         ic_ = dag_->insert( to + z_ == 0 );
   }

   // initial region
   reg_ = std::shared_ptr<IntervalRegion>(new IntervalRegion(s_));

   for (Variable v : s_)
      reg_->set(v, pb_->getDomain(v));

   if (withobj)
      reg_->set(z_, Interval::universe());
   
   start_ = std::shared_ptr<RealPoint>(new RealPoint(reg_->midpoint()));
   // std::cout<<"LocalSolver constructor"<<std::endl;
}

LocalSolver::LocalSolver(const RealFunction& obj, const RealFunctionVector& ctrs)
      : m_(ctrs.nbFuns()), s_(obj.scope() | ctrs.scope()),
      time_limit_(Param::GetDblParam("LOCAL_SOLVER_TIME_LIMIT"))
{
    obj_ = std::make_shared<RealFunction>(obj);
    ctrs_ = std::make_shared<RealFunctionVector>(ctrs);
    best_ = nullptr;
    best_val_ = Interval::universe().right();
    n_ = s_.size();
}

LocalSolver::~LocalSolver()
{
   std::cout<<"Freeing localsolver object"<<std::endl;
}


double LocalSolver::get_best_val() const
{
   return best_val_;
}

RealPoint LocalSolver::get_best_point() const
{
   return *best_;
}

OptimizationStatus LocalSolver::get_status() const
{
   return status_;
}


double LocalSolver::getTimeLimit() const
{
   return time_limit_;
}

void LocalSolver::setTimeLimit(double val)
{
   time_limit_ = val;
}


void LocalSolver::updateRegion(const IntervalRegion& reg)
{
    reg_ = std::make_shared<IntervalRegion>(reg);
}

void LocalSolver::updateStart(const RealPoint& start)
{
    start_ = std::make_shared<RealPoint>(start);
}
} // namespace
