///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/NLPSolver.hpp"

namespace realpaver {

double f_rp(const std::vector<double> &x, std::vector<double> &grad, void* f_data)
{
    NLPSolver* ls = reinterpret_cast<NLPSolver*>(f_data);

    Scope s = ls->obj()->scope();
    if (ls->nbCtrs()>0) s.insert(ls->ctrs()->scope());

    RealPoint rp(s,0.0);
    for (size_t i=0; i<x.size(); i++)
    {
        rp[i] = x[i];
    }
    double val = ls->obj()->eval(rp);
    return val;
}

double f_rp_diff(const std::vector<double> &x, std::vector<double> &grad, void* f_data)
{
    NLPSolver* ls = reinterpret_cast<NLPSolver*>(f_data);

    Scope s = ls->obj()->scope();
    if (ls->nbCtrs()>0) s.insert(ls->ctrs()->scope());

    RealPoint rp(s,0.0);
    for (size_t i=0; i<x.size(); i++)
    {
        rp[i] = x[i];
    }
    double val = ls->obj()->eval(rp);
    RealVector g(x.size(), 0.0);
    ls->obj()->diff(rp, g);
    if (grad.empty())
    {
        for (size_t i=0; i<g.size(); i++)
        {
            grad.push_back(g[i]);
        }
    }
    else
    {
        for (size_t i=0; i<g.size(); i++)
        {
            grad[i] = g[i];
        }
    }
    return val;
}

NLPSolver::NLPSolver(const Problem& pb) : NLPModel(pb)
{
    optimizer_ = nullptr;
}

NLPSolver::NLPSolver(const RealFunction& obj)
    : NLPModel(obj)
{
    optimizer_ = nullptr;
}

NLPSolver::NLPSolver(const RealFunction& obj, const RealFunctionVector& ctrs)
    : NLPModel(obj, ctrs)
{
    optimizer_ = nullptr;
}

NLPSolver::~NLPSolver()
{
    if (optimizer_ != nullptr) delete optimizer_;
}

void NLPSolver::setAlgorithm(std::string alg)
{
    NLPModel::setAlgorithm(alg);

    if (alg == "NELDERMEAD")
        nlopt_algorithm_ = nlopt::algorithm::LN_NELDERMEAD;
    else if (alg == "SLSQP")
        nlopt_algorithm_ = nlopt::algorithm::LD_SLSQP;
    else if (alg == "BOBYQA")
        nlopt_algorithm_ = nlopt::algorithm::LN_BOBYQA;
    else if (alg == "MMA")
        nlopt_algorithm_ = nlopt::algorithm::LD_MMA;
}

OptimizationStatus NLPSolver::minimize(const IntervalRegion& reg,
                                       const RealPoint& src) {
    size_t n = src.size();

    if (optimizer_==nullptr || !(optimizer_->get_algorithm() == nlopt_algorithm_ &&
                                 n == optimizer_->get_dimension()))
    {
        optimizer_ = new nlopt::opt(nlopt_algorithm_, n); 
    }

    optimizer_->set_ftol_rel(rtol_.getVal());
    optimizer_->set_ftol_abs(atol_.getVal());
    optimizer_->set_maxeval(this->iterLimit());
    optimizer_->set_maxtime(this->timeLimit());

    if (nlopt_algorithm_ == nlopt::algorithm::LN_NELDERMEAD || nlopt_algorithm_ == nlopt::algorithm::LN_BOBYQA)
        optimizer_->set_min_objective(f_rp, this);
    else
        optimizer_->set_min_objective(f_rp_diff, this);
    
    std::vector<double> x_l(n);
    std::vector<double> x_u(n);
    std::vector<double> x(n);
    for (size_t i=0; i<n; i++)
    {
        x_l[i] = reg[i].left();
        x_u[i] = reg[i].right();
        x[i] = src[i];
    }
    optimizer_->set_lower_bounds(x_l);
    optimizer_->set_upper_bounds(x_u);

    nlopt::result status = optimizer_->optimize(x, best_val_);

    // creates and assigns the best point
    if (best_ != nullptr) delete best_;
    best_ = new RealPoint(src.scope());

//    for (size_t i=0; i<n; i++)
    best_->setArray(x.data());

    if (status == nlopt::SUCCESS || status == nlopt::FTOL_REACHED ||
        status == nlopt::XTOL_REACHED) {
        status_ = OptimizationStatus::Optimal;
    }
    else if (status == nlopt::MAXEVAL_REACHED)
    {
        status_ = OptimizationStatus::StopOnIterLimit;
    }
    else if (status == nlopt::MAXTIME_REACHED)
    {
        status_ = OptimizationStatus::StopOnTimeLimit;
    }
    else {
        std::cerr<<"\n\n*** The problem FAILED!\n"<<status<<std::endl;
        status_ = OptimizationStatus::Other;
    }

    return status_;
}

} // namespace