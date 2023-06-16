///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////


#include "realpaver/LocalOptimizerNlopt.hpp"

namespace realpaver {

double f_rp(const std::vector<double> &x, std::vector<double> &grad, void* f_data)
{
    LocalOptimizerNlopt* ls = reinterpret_cast<LocalOptimizerNlopt*>(f_data);

    RealPoint rp(ls->scope(),0.0);
    for (size_t i=0; i<x.size(); i++)
    {
        rp[i] = x[i];
    }
    double val = ls->obj()->eval(rp);
    return val;
}

double f_rp_diff(const std::vector<double> &x, std::vector<double> &grad, void* f_data)
{
    LocalOptimizerNlopt* ls = reinterpret_cast<LocalOptimizerNlopt*>(f_data);

    RealPoint rp(ls->scope(),0.0);
    for (size_t i=0; i<x.size(); i++)
    {
        rp[i] = x[i];
    }
    double val = ls->obj()->eval(rp);
    RealVector g(x.size(),0.0);
    ls->obj()->diff(rp,g);
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

LocalOptimizerNlopt::LocalOptimizerNlopt(const Problem& pb) : LocalOptimizer(pb)
{
}

LocalOptimizerNlopt::LocalOptimizerNlopt(const RealFunction& obj, const RealFunctionVector& ctrs)
    : LocalOptimizer(obj, ctrs)
{
}

LocalOptimizerNlopt::~LocalOptimizerNlopt()
{
}


void LocalOptimizerNlopt::algorithm_name(nlopt::algorithm alg)
{
    algorithm_ = alg;
}


nlopt::algorithm LocalOptimizerNlopt::algorithm_name() const
{
    return algorithm_;
}

OptimizationStatus LocalOptimizerNlopt::minimize(const IntervalRegion& reg,
                                    const RealPoint& src) {
    size_t n = src.size();

    if (optimizer_==nullptr || !(optimizer_->get_algorithm() == algorithm_ && n == optimizer_->get_dimension()))
    {
        optimizer_ = std::make_shared<nlopt::opt>(nlopt::opt(algorithm_,n)); 
    }

    optimizer_->set_ftol_rel(rtol_);
    optimizer_->set_ftol_abs(atol_);
    optimizer_->set_maxeval(this->iterLimit());
    optimizer_->set_maxtime(this->timeLimit());

    optimizer_->set_min_objective(f_rp,this);
    
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

    nlopt::result status = optimizer_->optimize(x,best_val_);

    if (best_ == nullptr)
        best_ = std::make_shared<RealPoint>(RealPoint(src.scope()));
    for (size_t i=0; i<n; i++)
        (*best_)[i] = x[i];

    if (status == nlopt::SUCCESS || status == nlopt::FTOL_REACHED || status == nlopt::XTOL_REACHED) {
        // std::cerr<<"\n\n*** The problem solved!\n"<<std::endl;
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




}