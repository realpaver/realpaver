///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/NLPSolver.hpp"
#include <IpIpoptApplication.hpp>

namespace realpaver {

NLPSolver::NLPSolver(const Problem& pb) : NLPModel(pb)
{}

NLPSolver::NLPSolver(const RealFunction& obj) : NLPModel(obj)
{}

NLPSolver::NLPSolver(const RealFunction& obj, const RealFunctionVector& ctrs)
    : NLPModel(obj, ctrs)
{}

NLPSolver::~NLPSolver()
{}


OptimizationStatus NLPSolver::minimize(const IntervalRegion& reg,
                                       const RealPoint& src)
{
    status_ = OptimizationStatus::Other;
    Ipopt::SmartPtr<Ipopt::IpoptApplication> app = IpoptApplicationFactory();

    // Change some options
    app->Options()->SetNumericValue("tol", atol_.getVal());
    app->Options()->SetStringValue("mu_strategy", "adaptive");
    app->Options()->SetStringValue("hessian_approximation", "limited-memory");
    app->Options()->SetNumericValue("max_cpu_time", time_limit_);
    app->Options()->SetIntegerValue("max_iter", iter_limit_);
    app->Options()->SetIntegerValue("print_level", 0);

    // Intialize the IpoptApplication and process the options
    Ipopt::ApplicationReturnStatus status;
    status = app->Initialize();

    if (status != Ipopt::Solve_Succeeded) {
        std::cerr << std::endl << std::endl << "*** Error during IPOPT initialization!" << std::endl;
    }
    else
    {
        // Ask Ipopt to solve the problem
        Ipopt::SmartPtr<LocalTNLP> tnlp = new LocalTNLP(this, std::make_shared<IntervalRegion>(reg), src);

        status = app->OptimizeTNLP(tnlp);
        best_val_ = tnlp->best_val_;

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

NLPSolver::LocalTNLP::LocalTNLP(NLPSolver* ls, SharedIntervalRegion reg, const RealPoint& start)
    : ls_(ls), reg_(reg), start_(start)
{}

bool NLPSolver::LocalTNLP::get_nlp_info(Ipopt::Index& n, Ipopt::Index& m, Ipopt::Index& nnz_jac_g,
                        Ipopt::Index& nnz_h_lag, IndexStyleEnum& index_style)
{
    n = ls_->nbVars();
    m = ls_->nbCtrs();
    std::shared_ptr<RealFunction> obj = ls_->obj();
    std::shared_ptr<RealFunctionVector> ctrs = ls_->ctrs();
    
    Scope s = ls_->obj()->scope();
    if (m>0) s.insert(ls_->ctrs()->scope());
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
    // std::cerr<<"nnz_jac_g: "<<nnz_jac_g<<std::endl;
    // nnz_h_lag: number of nonzeros in hessian
    // TODO
    nnz_h_lag = 0;

    // use the C style indexing (0-based).the numbering style used for row/col entries in the sparse matrix format 
    index_style = TNLP::C_STYLE;

    return true;
}

bool NLPSolver::LocalTNLP::get_bounds_info(Ipopt::Index n, Ipopt::Number* x_l, Ipopt::Number* x_u,
                            Ipopt::Index m, Ipopt::Number* g_l, Ipopt::Number* g_u)
{
    std::shared_ptr<RealFunctionVector> ctrs = ls_->ctrs();
    
    for (size_t i=0; i<n; i++)
    {
        x_l[i] = (*reg_)[i].left();
        x_u[i] = (*reg_)[i].right();
    }

    // g_l and g_u are bounds for the constraints : g_l <= c <= g_u
    // TODO
    for(size_t i=0; i<m; i++)
    {
        // std::cerr<<"ctr_"<<i<<" in "<<ctrs->fun(i).getImage()<<std::endl;
        g_l[i] = ctrs->fun(i).getImage().left();
        g_u[i] = ctrs->fun(i).getImage().right();
    }

    return true;
}

bool NLPSolver::LocalTNLP::get_starting_point(Ipopt::Index n, bool init_x, Ipopt::Number* x,
                                bool init_z, Ipopt::Number* z_L, Ipopt::Number* z_U,
                                Ipopt::Index m, bool init_lambda,
                                Ipopt::Number* lambda)
{
    // std::shared_ptr<RealPoint> start = ls_->start();
    for (size_t i=0; i<n; i++)
        x[i] = start_[i];

    // z are dual variables, not mandatory
    // lambda are the dual multipliers, not mandatory
    // TODO?
    // std::cerr<<*start_<<std::endl;

    return true;
}

bool NLPSolver::LocalTNLP::eval_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number& obj_value)
{
    // compute obj_value, i.e. the value of the objective function, from x vector
    const std::shared_ptr<RealFunction> obj = ls_->obj();
    Scope s = ls_->obj()->scope();
    if (ls_->nbCtrs()>0) s.insert(ls_->ctrs()->scope());
    
    RealPoint pt(s);
    for(size_t i=0; i<s.size();i++)
        pt[i] = x[i];
    obj_value = obj->eval(pt);

    return true;
}

bool NLPSolver::LocalTNLP::eval_grad_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number* grad_f)
{
    std::shared_ptr<RealFunction> obj = ls_->obj();
    Scope s = ls_->obj()->scope();
    if (ls_->nbCtrs()>0) s.insert(ls_->ctrs()->scope());
    const Scope os = obj->scope();
    // compute grad_f, i.e. the gradient of the objective function, from x vector
    RealPoint pt(s);
    for(size_t i=0; i<os.size();i++)
    {
        pt[i] = x[i]; //x[s.index(os.var(i))];
    }
    RealPoint gf(obj->scope());
    obj->diff(pt.subPoint(os),gf);
    // std::cerr<<"gf: "<<gf<<" with x: "<<pt<<std::endl;
    for(size_t i=0; i<os.size();i++)
        grad_f[s.index(os.var(i))] = gf[i];

    return true;
}

bool NLPSolver::LocalTNLP::eval_g (Ipopt::Index n, const Ipopt::Number *x, bool new_x, Ipopt::Index m, Ipopt::Number *g)
{
    Scope s = ls_->obj()->scope();
    if (m>0) s.insert(ls_->ctrs()->scope());
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

bool NLPSolver::LocalTNLP::eval_jac_g (Ipopt::Index n, const Ipopt::Number *x, bool new_x,
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
    Scope s = ls_->obj()->scope();
    if (m>0) s.insert(ls_->ctrs()->scope());
    std::shared_ptr<RealFunctionVector> ctrs = ls_->ctrs();
    if (values == nullptr || values == NULL || values == 0)
    {
        size_t nb_val = 0;
        // Return the structure of the Jacobian:
        for (size_t j=0; j<m; j++)
        {
            // std::cerr<<"ctr"<<j<<": "<<ctrs->fun(j).scope()<<std::endl;
            for (size_t i=0; i<n; i++)
            {
                if (ctrs->fun(j).scope().contains(s.var(i)))
                {
                    // std::cerr<<"var"<<i<<":"<<s.var(i)<<std::endl;
                    // iRow[j*m+i] = j;
                    // jCol[j*m+i] = i;
                    iRow[nb_val] = j;
                    jCol[nb_val] = i;
                    nb_val++;
                }
            }
        }
        // std::cerr<<"nb_val: "<<nb_val<<std::endl;
    }
    else
    {
        // std::cerr<<"Computing jacobian"<<std::endl;
        Scope cs = ls_->ctrs()->scope();
        RealPoint pt(cs);
        // std::cerr<<"x_:";
        for(size_t i=0; i<n;i++)
        {
            // std::cerr<<x[i]<<",";
            if(cs.contains(s.var(i)))
                pt[cs.index(s.var(i))] = x[i];
            // pt[i] = x[i];
        }
        // std::cerr<<std::endl<<"x: "<<pt<<std::endl;
        size_t nb_val = 0;
        RealMatrix jac(m,cs.size());
        // ctrs->diff(pt.subPoint(cs),jac);
        ctrs->diff(pt,jac);
        // std::cerr<<"J: "<<std::endl<<jac<<std::endl;
        for(size_t j=0; j<m;j++)
        {
            for(size_t i=0;i<n;i++)
            {
                if (ctrs->fun(j).scope().contains(s.var(i)))
                {
                    // std::cerr<<"J["<<j<<","<<i<<"]:"<<jac.get(j,cs.index(s.var(i)))<<std::endl;
                    values[nb_val] = jac.get(j,cs.index(s.var(i)));
                }
            }
        }
    }

    return true;
}

// bool NLPSolver::LocalTNLP::eval_h (Ipopt::Index n, const Ipopt::Number *x, bool new_x,Ipopt::Number obj_factor,
//                         Ipopt::Index m, const Ipopt::Number *lambda, bool new_lambda, Ipopt::Index nele_hess,
//                         Ipopt::Index *iRow, Ipopt::Index *jCol, Ipopt::Number *values)
// {
//     // compute values for the Hessian matrix, where:
//     // - obj_factor is the factor in front of the objective term in the Hessian, $ \sigma_f$. (input)
//     // - lambda is the values for the constraint multipliers, $ \lambda$, at which the Hessian is to be evaluated. (input)
//     // - nele_hess is the number of nonzeros in the Hessian (dimension of iRow, jCol, and values). (input)
//     // - iRow is the row indices of entries in the Hessian. (output)
//     // - jCol is the column indices of entries in the Hessian. (output)
//     // - values is the values of the entries in the Hessian. (output)
//     return false;
// }

Ipopt::Index NLPSolver::LocalTNLP::get_number_of_nonlinear_variables()
{
    // TODO
    // const Scope s = ls_->scope();
    return ls_->nbVars();
}

bool NLPSolver::LocalTNLP::get_list_of_nonlinear_variables (Ipopt::Index num_nonlin_vars, Ipopt::Index *pos_nonlin_vars)
{
    // const Scope s = ls_->scope();
    // for (size_t i=0; i<s.size();i++)
    for (size_t i=0; i<ls_->nbVars();i++)
    {
        pos_nonlin_vars[i] = i;
    }

    return true;
}

void NLPSolver::LocalTNLP::finalize_solution(Ipopt::SolverReturn status,
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
    RealPoint best(start_.scope());
    best.setArray(x);
    ls_->setBestPoint(best);

    best_val_ = obj_value;
}

} // namespace