///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////


#include "realpaver/LocalSolverNlopt.hpp"

namespace realpaver {

double LocalSolverNlopt::f_(const std::vector<double> &x, std::vector<double> &grad, void* f_data)
{
    return 0.0;
}

LocalSolverNlopt::LocalSolverNlopt(const Problem& pb) : LocalSolver(pb)
{

}

LocalSolverNlopt::LocalSolverNlopt(const RealFunction& obj, const RealFunctionVector& ctrs) : LocalSolver(obj, ctrs)
{

}

LocalSolverNlopt::~LocalSolverNlopt()
{
}


void LocalSolverNlopt::set_algorithm_name(nlopt::algorithm alg)
{
    algorithm_ = alg;
}


OptimizationStatus LocalSolverNlopt::minimize(const IntervalRegion& reg,
                                    const RealPoint& src,
                                    RealPoint& dest) {
    size_t n = src.size();

    if (optimizer_==nullptr || !(optimizer_->get_algorithm() == algorithm_ && n == optimizer_->get_dimension()))
    {
        optimizer_ = std::make_shared<nlopt::opt>(nlopt::opt(algorithm_,n)); 
    }

    optimizer_->set_ftol_rel(1e-4);
    optimizer_->set_ftol_abs(1e-9);
    optimizer_->set_maxeval(100);
    optimizer_->set_maxtime(this->getTimeLimit());

    // double (*func)(const std::vector<double> &x, std::vector<double> &grad, void* f_data);
    // func = reinterpret_cast<nlopt::vfunc>(&LocalSolverNlopt::f_);
    // optimizer_->set_min_objective(func,NULL);
    // optimizer_->set_min_objective(&LocalSolverNlopt::f_,NULL);
    
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

    if (status == nlopt::SUCCESS) {
        printf("\n\n*** The problem solved!\n");
    }
    else {
        printf("\n\n*** The problem FAILED!\n");
    }

    if (best_ == nullptr)
        best_ = std::make_shared<RealPoint>(RealPoint(src.scope()));
    for (size_t i=0; i<n; i++)
        (*best_)[i] = x[i];
    dest = *best_;
    return OptimizationStatus::Other;
}




}