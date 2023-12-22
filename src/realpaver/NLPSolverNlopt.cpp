///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/NLPSolverNlopt.hpp"

namespace realpaver {

double ctr_nlopt(const std::vector<double>& x, std::vector<double>& grad,
                 void* f_data)
{
   NLPSolver::Ctr* c = reinterpret_cast<NLPSolver::Ctr*>(f_data);
   NLPSolver* ls = c->ls;
   RealFunctionVector* V = ls->ctrs();
   RealFunction f = V->fun(c->idx);
   Interval img = f.getImage();

   // evaluates the function on x
   // we have a <= f (if c->isleft) or f <= b
   // i.e. a - f <= 0 or f - b <= 0
    RealPoint pt(ls->scope());
    pt.setArray(x.data());
    double tmp = f.eval(pt),
           val = (c->isleft ? img.left() - tmp : tmp - img.right());

    // no evaluation of the gradient if grad is empty
    if (grad.empty()) return val;

    // evaluates the gradient
    RealVector g(f.nbVars(), 0.0);
    f.diff(pt, g);

    // puts g (scope : variables in f) in grad (scope : all the variables)
    size_t i = 0, j = 0;
    for (auto v : ls->scope())
    {
       if (f.scope().contains(v))
       {
          grad[i] = (c->isleft ? -g[j] : g[j]);
          ++j;
       }
       else
          grad[i] = 0.0;

       ++i;
    }

    return val;
}

double f_nlopt(const std::vector<double>& x, std::vector<double>& grad,
               void* f_data)
{
    NLPSolver* ls = reinterpret_cast<NLPSolver*>(f_data);
    RealFunction* obj = ls->obj();

    ASSERT(x.size() == ls->scope().size(), "Bad scopes in f_nlopt");

    // evaluates the function on x
    RealPoint pt(ls->scope());
    pt.setArray(x.data());
    double val = obj->eval(pt);

    // no evaluation of the gradient if grad is empty
    if (grad.empty()) return val;

    // evaluates the gradient
    RealVector g(obj->nbVars(), 0.0);
    obj->diff(pt, g);

    size_t i = 0, j = 0;
    for (auto v : ls->scope())
    {
       if (obj->scope().contains(v))
       {
          grad[i] = g[j];
          ++j;
       }
       else
          grad[i] = 0.0;

       ++i;
    }

    return val;
}

NLPSolver::NLPSolver(const Problem& pb)
    : NLPModel(pb),
      optimizer_(nullptr),
      nl_ctrs_()
{}

NLPSolver::NLPSolver(const RealFunction& obj)
    : NLPModel(obj),
      optimizer_(nullptr),
      nl_ctrs_()
{}

NLPSolver::NLPSolver(const RealFunction& obj, const RealFunctionVector& ctrs)
    : NLPModel(obj, ctrs),
      optimizer_(nullptr),
      nl_ctrs_()
{}

NLPSolver::~NLPSolver()
{
   if (optimizer_ != nullptr) delete optimizer_;
}

OptimizationStatus NLPSolver::minimize(const IntervalBox& box,
                                       const RealPoint& src)
{
    ASSERT(box.scope().contains(scope()),
           "Bad scope of an interval box given as input of NlpOpt");

    ASSERT(src.scope().contains(scope()),
           "Bad scope of astarting point given as input of NlpOpt");

    // gets the dimension
    size_t n = this->scope().size();

    // projects B and src on the scope of this
    IntervalBox X(box, this->scope());
    RealPoint P(src, this->scope());

    LOG_LOW("Nlopt optimize at point: " << P);

    // makes the optimizer
    makeAlgorithm();
    if (optimizer_ != nullptr) delete optimizer_;
    optimizer_ = new nlopt::opt(nlopt_alg_, n); 

    // assigns the minimization of the objective function
    optimizer_->set_min_objective(f_nlopt, this);

    // assigns the parameters
    optimizer_->set_ftol_rel(tol_.getRelTol());
    optimizer_->set_ftol_abs(tol_.getAbsTol());
    optimizer_->set_maxeval(iter_limit_);
    optimizer_->set_maxtime(this->timeLimit());

    // assigns the bound constraints
    std::vector<double> x_l(n), x_u(n), x(n);
    for (size_t i=0; i<n; i++)
    {
        x_l[i] = X[i].left();
        x_u[i] = X[i].right();
        x[i] = P[i];
    }
    optimizer_->set_lower_bounds(x_l);
    optimizer_->set_upper_bounds(x_u);

    // creates the constraints
    makeCtrs();
    for (size_t i=0; i<nl_ctrs_.size(); ++i)
        optimizer_->add_inequality_constraint(ctr_nlopt, &nl_ctrs_[i],
                                              tol_.getAbsTol());

    // calls the optimizer
    nlopt::result status;
    try
    {
       status = optimizer_->optimize(x, best_val_);
    }
    catch(std::exception& e)
    {
       LOG_LOW("Nlopt exception catched: " << e.what());
       status_ = OptimizationStatus::Other;
       return status_;
    }

    // assigns the resulting status
    if (status == nlopt::SUCCESS || status == nlopt::FTOL_REACHED ||
        status == nlopt::XTOL_REACHED)
        status_ = OptimizationStatus::Optimal;
    
    else if (status == nlopt::MAXEVAL_REACHED)
        status_ = OptimizationStatus::StopOnIterLimit;

    else if (status == nlopt::MAXTIME_REACHED)
        status_ = OptimizationStatus::StopOnTimeLimit;

    else {
        status_ = OptimizationStatus::Other;
    }

    LOG_LOW("Result of local optimization: " << status_);

    if (status_ != OptimizationStatus::Other)
    {
       // creates and assigns the best point
       if (best_ != nullptr) delete best_;
       best_ = new RealPoint(P.scope());
       best_->setArray(x.data());

       LOG_LOW("Optimum found by Nlopt: " << best_val_);
       LOG_LOW("Best point found by NlOpt: " << (*best_));
    }

    return status_;
}

void NLPSolver::makeCtrs()
{
   RealFunctionVector* V = this->ctrs();

   for(size_t i=0; i<nbCtrs(); ++i)
   {
      RealFunction f = V->fun(i);
      Interval img = f.getImage();

      // left bound
      if (!img.isInfLeft())
      {
         Ctr c = {this, i, true};
         nl_ctrs_.push_back(c);
      }

      // right bound
      if (!img.isInfRight())
      {
         Ctr c = {this, i, false};
         nl_ctrs_.push_back(c);
      }
   }
}

void NLPSolver::makeAlgorithm()
{
    // user algorithm
    std::string alg = getAlgorithm();

    if (nbCtrs() > 0)
    {
        nlopt_alg_ = nlopt::algorithm::LD_SLSQP;

        if (alg == "NLOPT_MMA")
            nlopt_alg_ = nlopt::algorithm::LD_MMA;
    }
    else
    {
        if (alg == "NLOPT_BOBYQA")
           nlopt_alg_ = nlopt::algorithm::LN_BOBYQA;

        else if (alg == "NLOPT_MMA")
           nlopt_alg_ = nlopt::algorithm::LD_MMA;

        else if (alg == "NLOPT_SLSQP")
           nlopt_alg_ = nlopt::algorithm::LD_SLSQP;

        else
           nlopt_alg_ = nlopt::algorithm::LN_NELDERMEAD;
    }
}

} // namespace
