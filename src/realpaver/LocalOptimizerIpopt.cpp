///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////


#include "realpaver/LocalOptimizerIpopt.hpp"
#include <IpIpoptApplication.hpp>

namespace realpaver {

LocalOptimizerIpopt::LocalOptimizerIpopt(const Problem& pb) : LocalOptimizer(pb)
{
}

LocalOptimizerIpopt::LocalOptimizerIpopt(const RealFunction& obj, const RealFunctionVector& ctrs) : LocalOptimizer(obj, ctrs)
{
}

LocalOptimizerIpopt::~LocalOptimizerIpopt()
{
}


OptimizationStatus LocalOptimizerIpopt::minimize(const IntervalRegion& reg,
                                    const RealPoint& src)
{
    status_ = OptimizationStatus::Other;

    region(reg);
    start(src);

    Ipopt::SmartPtr<Ipopt::IpoptApplication> app = IpoptApplicationFactory();

    // Change some options
    app->Options()->SetNumericValue("tol", 1e-9);
    app->Options()->SetStringValue("mu_strategy", "adaptive");
    app->Options()->SetStringValue("hessian_approximation", "limited-memory");
    app->Options()->SetNumericValue("max_cpu_time", time_limit_);
    app->Options()->SetNumericValue("max_iter", iter_limit_);

    // Intialize the IpoptApplication and process the options
    Ipopt::ApplicationReturnStatus status;
    status = app->Initialize();
    if (status != Ipopt::Solve_Succeeded) {
        std::cerr << std::endl << std::endl << "*** Error during IPOPT initialization!" << std::endl;
    }
    else
    {
        // Ask Ipopt to solve the problem
        Ipopt::SmartPtr<Ipopt::TNLP> tnlp = new LocalTNLP(this);
        status = app->OptimizeTNLP(tnlp);

        if (status == Ipopt::Solve_Succeeded || status == Ipopt::Solved_To_Acceptable_Level)
        {
            status_ = OptimizationStatus::Optimal;
        }
        else if (status == Ipopt::Maximum_Iterations_Exceeded)
        {
            status_ = OptimizationStatus::StopOnIterLimit;
        }
        else if (status == Ipopt::Maximum_CpuTime_Exceeded)
        {
            status_ = OptimizationStatus::StopOnTimeLimit;
        }
        else
        {
            std::cerr << std::endl << std::endl << "*** IPOPT FAILED!" << std::endl;
        }
    }
    return status_;
}



LocalOptimizerIpopt::LocalTNLP::LocalTNLP(LocalOptimizerIpopt* ls) : ls_(ls)
{
}


bool LocalOptimizerIpopt::LocalTNLP::get_nlp_info(Ipopt::Index& n, Ipopt::Index& m, Ipopt::Index& nnz_jac_g,
                        Ipopt::Index& nnz_h_lag, IndexStyleEnum& index_style)
{
    n = ls_->nbVars();
    m = ls_->nbCtrs();
    const Scope s = ls_->scope();
    const std::shared_ptr<RealFunction> obj = ls_->obj();
    const std::shared_ptr<RealFunctionVector> ctrs = ls_->ctrs();
    
    // nnz_jac_g: number of nonzeros in jacobian
    // TODO
    nnz_jac_g = 0;
    for(size_t j=0; j<m;j++)
    {
        for(size_t i=0; i<n;i++)
        {
            if (ctrs->fun(j).scope().contains(s.var(i)))
                nnz_jac_g++;
        }
    }
    
    // nnz_h_lag: number of nonzeros in hessian
    // TODO
    nnz_h_lag = 0;

    // use the C style indexing (0-based).the numbering style used for row/col entries in the sparse matrix format 
    index_style = TNLP::C_STYLE;
    
    return true;
}

bool LocalOptimizerIpopt::LocalTNLP::get_bounds_info(Ipopt::Index n, Ipopt::Number* x_l, Ipopt::Number* x_u,
                            Ipopt::Index m, Ipopt::Number* g_l, Ipopt::Number* g_u)
{
    SharedIntervalRegion reg = ls_->region();
    std::shared_ptr<RealFunctionVector> ctrs = ls_->ctrs();
    
    for (size_t i=0; i<n; i++)
    {
        x_l[i] = (*reg)[i].left();
        x_u[i] = (*reg)[i].right();
    }

    // g_l and g_u are bounds for the constraints : g_l <= c <= g_u
    // TODO
    for(size_t i=0; i<m; i++)
    {
        g_l[i] = ctrs->fun(i).getImage().left();
        g_u[i] = ctrs->fun(i).getImage().right();
    }

    return true;
}

bool LocalOptimizerIpopt::LocalTNLP::get_starting_point(Ipopt::Index n, bool init_x, Ipopt::Number* x,
                                bool init_z, Ipopt::Number* z_L, Ipopt::Number* z_U,
                                Ipopt::Index m, bool init_lambda,
                                Ipopt::Number* lambda)
{
    std::shared_ptr<RealPoint> start = ls_->start();
    for (size_t i=0; i<n; i++)
        x[i] = (*start)[i];

    // z are dual variables, not mandatory
    // lambda are the dual multipliers, not mandatory
    // TODO?

    return true;
}

bool LocalOptimizerIpopt::LocalTNLP::eval_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number& obj_value)
{
    // compute obj_value, i.e. the value of the objective function, from x vector
    const Scope s = ls_->scope();
    const std::shared_ptr<RealFunction> obj = ls_->obj();
    
    RealPoint pt(s);
    for(size_t i=0; i<s.size();i++)
        pt[i] = x[i];
    obj_value = obj->eval(pt);

    return true;
}

bool LocalOptimizerIpopt::LocalTNLP::eval_grad_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number* grad_f)
{
    const Scope s = ls_->scope();
    std::shared_ptr<RealFunction> obj = ls_->obj();
    // compute grad_f, i.e. the gradient of the objective function, from x vector
    RealPoint pt(s);
    for(size_t i=0; i<s.size();i++)
        pt[i] = x[i];
    RealPoint gf(s);
    obj->diff(pt,gf);
    for(size_t i=0; i<s.size();i++)
        grad_f[i] = gf[i];
    
    return true;
}

bool LocalOptimizerIpopt::LocalTNLP::eval_g (Ipopt::Index n, const Ipopt::Number *x, bool new_x, Ipopt::Index m, Ipopt::Number *g)
{
    const Scope s = ls_->scope();
    std::shared_ptr<RealFunctionVector> ctrs = ls_->ctrs();
    // compute g, i.e. the value of the constraints, from x vector
    // TODO
    RealPoint pt(s);
    for(size_t i=0; i<s.size();i++)
        pt[i] = x[i];
    for(size_t j=0; j<m;j++)
    {
        g[j] = ctrs->fun(j).eval(pt);
    }
    
    return true;
}

bool LocalOptimizerIpopt::LocalTNLP::eval_jac_g (Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                                Ipopt::Index m, Ipopt::Index nele_jac, Ipopt::Index *iRow,
                                Ipopt::Index *jCol, Ipopt::Number *values)
{
    // compute values, i.e. the Jacobian of the constraints, from x vector where:
    // - nele_jac is the number of nonzeros in the Jacobian
    // - iRow are the row indices of the Jacobian entries
    // - iCol are the column indices of the Jacobian entries
    // Then, each nonzero of J is referenced by an index i, where:
    // - iRow[i] and iCol[i] defines its coordinates in J
    // - values[i] defines its value
    if (values == nullptr || values == NULL || values == 0)
    {
        // Return the structure of the Jacobian:
        for (size_t j=0; j<m; j++)
        {
            for (size_t i=0; i<n; i++)
            {
                iRow[j*m+i] = j;
                jCol[j*m+i] = i;
            }
        }
        return true;
    }
    else
    {
        const Scope s = ls_->scope();
        std::shared_ptr<RealFunctionVector> ctrs = ls_->ctrs();

        RealPoint pt(s);
        for(size_t i=0; i<n;i++)
        {
            pt[i] = x[i];
        }
        size_t nb_val = 0;
        for(size_t j=0; j<m;j++)
        {
            RealMatrix jac(m,n);
            ctrs->diff(pt,jac);
            for(size_t i=0;i<n;i++)
            {
                if (ctrs->fun(j).scope().contains(s.var(i)))
                {
                    values[nb_val] = jac.get(j,i);
                }
            }
        }
    }

    return true;
}

bool LocalOptimizerIpopt::LocalTNLP::eval_h (Ipopt::Index n, const Ipopt::Number *x, bool new_x,Ipopt::Number obj_factor,
                        Ipopt::Index m, const Ipopt::Number *lambda, bool new_lambda, Ipopt::Index nele_hess,
                        Ipopt::Index *iRow, Ipopt::Index *jCol, Ipopt::Number *values)
{
    // compute values for the Hessian matrix, where:
    // - obj_factor is the factor in front of the objective term in the Hessian, $ \sigma_f$. (input)
    // - lambda is the values for the constraint multipliers, $ \lambda$, at which the Hessian is to be evaluated. (input)
    // - nele_hess is the number of nonzeros in the Hessian (dimension of iRow, jCol, and values). (input)
    // - iRow is the row indices of entries in the Hessian. (output)
    // - jCol is the column indices of entries in the Hessian. (output)
    // - values is the values of the entries in the Hessian. (output)
    return false;
}

Ipopt::Index LocalOptimizerIpopt::LocalTNLP::get_number_of_nonlinear_variables()
{
    // TODO
    const Scope s = ls_->scope();
    return s.size();
}

bool LocalOptimizerIpopt::LocalTNLP::get_list_of_nonlinear_variables (Ipopt::Index num_nonlin_vars, Ipopt::Index *pos_nonlin_vars)
{
    const Scope s = ls_->scope();
    for (size_t i=0; i<s.size();i++)
    {
        pos_nonlin_vars[i] = i;
    }
    return true;
}

void LocalOptimizerIpopt::LocalTNLP::finalize_solution(Ipopt::SolverReturn status,
                                Ipopt::Index n, const Ipopt::Number* x, const Ipopt::Number* z_L, const Ipopt::Number* z_U,
                                Ipopt::Index m, const Ipopt::Number* g, const Ipopt::Number* lambda,
                                Ipopt::Number obj_value,
                const Ipopt::IpoptData* ip_data,
                Ipopt::IpoptCalculatedQuantities* ip_cq)
{
   /*
    status: (in), gives the status of the algorithm as specified in IpAlgTypes.hpp,
        SUCCESS: Algorithm terminated successfully at a locally optimal point, satisfying the convergence tolerances (can be specified by options).
        MAXITER_EXCEEDED: Maximum number of iterations exceeded (can be specified by an option).
        CPUTIME_EXCEEDED: Maximum number of CPU seconds exceeded (can be specified by an option).
        STOP_AT_TINY_STEP: Algorithm proceeds with very little progress.
        STOP_AT_ACCEPTABLE_POINT: Algorithm stopped at a point that was converged, not to ``desired'' tolerances, but to ``acceptable'' tolerances (see the acceptable-... options).
        LOCAL_INFEASIBILITY: Algorithm converged to a point of local infeasibility. Problem may be infeasible.
        USER_REQUESTED_STOP: The user call-back function intermediate_callback (see Section 3.3.4) returned false, i.e., the user code requested a premature termination of the optimization.
        DIVERGING_ITERATES: It seems that the iterates diverge.
        RESTORATION_FAILURE: Restoration phase failed, algorithm doesn't know how to proceed.
        ERROR_IN_STEP_COMPUTATION: An unrecoverable error occurred while IPOPT tried to compute the search direction.
        INVALID_NUMBER_DETECTED: Algorithm received an invalid number (such as NaN or Inf) from the NLP; see also option check_derivatives_for_naninf.
        INTERNAL_ERROR: An unknown internal error occurred. Please contact the IPOPT authors through the mailing list.
    n: (in), the number of variables in the problem (dimension of $ x$).
    x: (in), the final values for the primal variables, $ x_*$.
    z_L: (in), the final values for the lower bound multipliers, $ z^L_*$.
    z_U: (in), the final values for the upper bound multipliers, $ z^U_*$.
    m: (in), the number of constraints in the problem (dimension of $ g(x)$).
    g: (in), the final value of the constraint function values, $ g(x_*)$.
    lambda: (in), the final values of the constraint multipliers, $ \lambda_*$.
    obj_value: (in), the final value of the objective, $ f(x_*)$.
    ip_data and ip_cq are provided for expert users.
*/
    std::shared_ptr<RealPoint> best = ls_->bestPoint();

    if (best == nullptr)
    {
        std::shared_ptr<RealPoint> start = ls_->start();
        best = std::make_shared<RealPoint>(RealPoint(start->scope()));
    }
    for (size_t i=0; i<n; i++)
        (*best)[i] = x[i];
    ls_->bestPoint(best);
}




}