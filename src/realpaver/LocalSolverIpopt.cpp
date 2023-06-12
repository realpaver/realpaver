///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////


#include "realpaver/LocalSolverIpopt.hpp"
#include <IpIpoptApplication.hpp>

namespace realpaver {

LocalSolverIpopt::LocalSolverIpopt(const Problem& pb) : LocalSolver(pb)
{
    
}

LocalSolverIpopt::LocalSolverIpopt(const RealFunction& obj, const RealFunctionVector& ctrs) : LocalSolver(obj, ctrs)
{
    /// TODO
}

LocalSolverIpopt::~LocalSolverIpopt()
{
   std::cout<<"Freeing IPOPT solver object"<<std::endl;
}


OptimizationStatus LocalSolverIpopt::minimize(const IntervalRegion& reg,
                                    const RealPoint& src,
                                    RealPoint& dest)
{
    status_ = OptimizationStatus::Other;

    updateRegion(reg);
    updateStart(src);

    Ipopt::SmartPtr<Ipopt::IpoptApplication> app = IpoptApplicationFactory();

    // Change some options
    app->Options()->SetNumericValue("tol", 1e-9);
    app->Options()->SetStringValue("mu_strategy", "adaptive");
    app->Options()->SetStringValue("hessian_approximation", "limited-memory");

    // Intialize the IpoptApplication and process the options
    Ipopt::ApplicationReturnStatus status;
    status = app->Initialize();
    if (status != Ipopt::Solve_Succeeded) {
        std::cerr << std::endl << std::endl << "*** Error during initialization!" << std::endl;
    }
    else
    {
        // Ask Ipopt to solve the problem
        status = app->OptimizeTNLP(this);

        if (status == Ipopt::Solve_Succeeded) {
            std::cout << std::endl << std::endl << "*** The problem solved!" << std::endl;
            dest = *best_;
            std::cout<<"LocalSolverIpopt::minimize ended with success!"<<std::endl;
            status_ = OptimizationStatus::Optimal;
        }
        else {
            std::cerr << std::endl << std::endl << "*** The problem FAILED!" << std::endl;
        }
    }
    std::cout<<"LocalSolverIpopt::minimize ended!"<<std::endl;
    return status_;
}


bool LocalSolverIpopt::get_nlp_info(Ipopt::Index& n, Ipopt::Index& m, Ipopt::Index& nnz_jac_g,
                        Ipopt::Index& nnz_h_lag, IndexStyleEnum& index_style)
{
    // std::cout<<"get_nlp_info"<<std::endl;
    n = n_;
    m = m_;
    
    // std::cout<<"get_nlp_info"<<std::endl;
    // nnz_jac_g: number of nonzeros in jacobian
    // TODO
    nnz_jac_g = 0;
    // std::cout<<"Number of constraints: "<<dag_->nbFuns()<<std::endl;
    for(size_t j=0; j<m_;j++)
    {
        for(size_t i=0; i<s_.size();i++)
        {
            if (ctrs_->fun(j).scope().contains(s_.var(i)))
                nnz_jac_g++;
        }
    }
    // std::cout<<"Number of nonzeros in jacobian: "<<nnz_jac_g<<std::endl;

    // nnz_h_lag: number of nonzeros in hessian
    // TODO
    nnz_h_lag = 0;

    // use the C style indexing (0-based).the numbering style used for row/col entries in the sparse matrix format 
    index_style = TNLP::C_STYLE;

    // std::cout<<"get_nlp_info"<<std::endl;
    return true;
}

bool LocalSolverIpopt::get_bounds_info(Ipopt::Index n, Ipopt::Number* x_l, Ipopt::Number* x_u,
                            Ipopt::Index m, Ipopt::Number* g_l, Ipopt::Number* g_u)
{
    // std::cout<<"get_bounds_info"<<std::endl;
    for (size_t i=0; i<n; i++)
    {
        x_l[i] = (*reg_)[i].left();
        x_u[i] = (*reg_)[i].right();
    }

    // g_l and g_u are bounds for the constraints : g_l <= c <= g_u
    // TODO
    for(size_t i=0; i<m_; i++)
    {
        g_l[i] = dag_->fun(i)->getImage().left();
        g_u[i] = dag_->fun(i)->getImage().right();
    }

    // std::cout<<"get_bounds_info"<<std::endl;
    return true;
}

bool LocalSolverIpopt::get_starting_point(Ipopt::Index n, bool init_x, Ipopt::Number* x,
                                bool init_z, Ipopt::Number* z_L, Ipopt::Number* z_U,
                                Ipopt::Index m, bool init_lambda,
                                Ipopt::Number* lambda)
{
    // std::cout<<"get_starting_point"<<std::endl;
    for (size_t i=0; i<n; i++)
        x[i] = (*start_)[i];

    // z are dual variables, not mandatory
    // lambda are the dual multipliers, not mandatory
    // TODO?

    // std::cout<<"get_starting_point"<<std::endl;
    return true;
}

bool LocalSolverIpopt::eval_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number& obj_value)
{
    // std::cout<<"eval_f"<<std::endl;
    // compute obj_value, i.e. the value of the objective function, from x vector
    // if (x==nullptr || x == 0)
    //     return false;
    RealPoint pt(s_);
    for(size_t i=0; i<s_.size();i++)
        pt[i] = x[i];
    obj_value = obj_->eval(pt);

    // std::cout<<"eval_f"<<std::endl;
    return true;
}

bool LocalSolverIpopt::eval_grad_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number* grad_f)
{
    // std::cout<<"eval_grad_f"<<std::endl;
    // compute grad_f, i.e. the gradient of the objective function, from x vector
    // if (x==nullptr || x == 0)
    //     return false;
    RealPoint pt(s_);
    for(size_t i=0; i<s_.size();i++)
        pt[i] = x[i];
    RealPoint gf(s_);
    obj_->diff(pt,gf);
    for(size_t i=0; i<s_.size();i++)
        grad_f[i] = gf[i];
    
    // std::cout<<"eval_grad_f: "<<gf<<std::endl;
    return true;
}

bool LocalSolverIpopt::eval_g (Ipopt::Index n, const Ipopt::Number *x, bool new_x, Ipopt::Index m, Ipopt::Number *g)
{
    // std::cout<<"eval_g"<<std::endl;
    // if (x==nullptr || x == 0)
    //     return false;
    // compute g, i.e. the value of the constraints, from x vector
    // TODO
    RealPoint pt(s_);
    for(size_t i=0; i<s_.size();i++)
        pt[i] = x[i];
    for(size_t j=0; j<ctrs_->nbFuns();j++)
    {
        g[j] = ctrs_->fun(j).eval(pt);
    }

    // std::cout<<"eval_g"<<std::endl;
    return true;
}

bool LocalSolverIpopt::eval_jac_g (Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                                Ipopt::Index m, Ipopt::Index nele_jac, Ipopt::Index *iRow,
                                Ipopt::Index *jCol, Ipopt::Number *values)
{
    // std::cout<<"eval_jac_g"<<std::endl;
    // compute values, i.e. the Jacobian of the constraints, from x vector where:
    // - nele_jac is the number of nonzeros in the Jacobian
    // - iRow are the row indices of the Jacobian entries
    // - iCol are the column indices of the Jacobian entries
    // Then, each nonzero of J is referenced by an index i, where:
    // - iRow[i] and iCol[i] defines its coordinates in J
    // - values[i] defines its value
    // TODO
    if (values == nullptr || values == NULL || values == 0)
    {
        // Return the structure of the Jacobian:
        for (size_t j=0; j<m_; j++)
        {
            for (size_t i=0; i<n_; i++)
            {
                iRow[j*m_+i] = j;
                jCol[j*m_+i] = i;
            }
        }
        return true;
    }
    else
    {
        RealPoint pt(s_);
        for(size_t i=0; i<n;i++)
        {
            pt[i] = x[i];
        }
        size_t nb_val = 0;
        for(size_t j=0; j<m_;j++)
        {
            RealMatrix jac(m_,n_);
            ctrs_->diff(pt,jac);
            for(size_t i=0;i<s_.size();i++)
            {
                // if (nele_jac==nb_val)
                //     return true;
                if (ctrs_->fun(j).scope().contains(s_.var(i)))
                {
                    values[nb_val] = jac.get(j,i);
                }
            }
        }
    }
    // std::cout<<"eval_jac_g"<<std::endl;
    return true;
}

bool LocalSolverIpopt::eval_h (Ipopt::Index n, const Ipopt::Number *x, bool new_x,Ipopt::Number obj_factor,
                        Ipopt::Index m, const Ipopt::Number *lambda, bool new_lambda, Ipopt::Index nele_hess,
                        Ipopt::Index *iRow, Ipopt::Index *jCol, Ipopt::Number *values)
{
    // std::cout<<"eval_h"<<std::endl;
    // compute values for the Hessian matrix, where:
    // - obj_factor is the factor in front of the objective term in the Hessian, $ \sigma_f$. (input)
    // - lambda is the values for the constraint multipliers, $ \lambda$, at which the Hessian is to be evaluated. (input)
    // - nele_hess is the number of nonzeros in the Hessian (dimension of iRow, jCol, and values). (input)
    // - iRow is the row indices of entries in the Hessian. (output)
    // - jCol is the column indices of entries in the Hessian. (output)
    // - values is the values of the entries in the Hessian. (output)
    // if (values == nullptr || values == 0)
    //     return false;
    // RealPoint pt(s_);
    // for(size_t i=0; i<s_.size();i++)
    //     pt[i] = x[i];
    // for(size_t j=0; j<diff_obj_->nbFuns();j++)
    // {
    //     RealPoint df_j(diff_obj_->fun(j).scope());
    //     ctrs_->fun(j).diff(pt,df_j);
    //     // TODO: save values in values using nele_hess, iRow and jCol
    // }
    // std::cout<<"eval_h"<<std::endl;
    return false;
}

Ipopt::Index LocalSolverIpopt::get_number_of_nonlinear_variables()
{
    return s_.size();
}

bool LocalSolverIpopt::get_list_of_nonlinear_variables (Ipopt::Index num_nonlin_vars, Ipopt::Index *pos_nonlin_vars)
{
    for (size_t i=0; i<s_.size();i++)
    {
        pos_nonlin_vars[i] = i;
    }
    return true;
}

void LocalSolverIpopt::finalize_solution(Ipopt::SolverReturn status,
                                Ipopt::Index n, const Ipopt::Number* x, const Ipopt::Number* z_L, const Ipopt::Number* z_U,
                                Ipopt::Index m, const Ipopt::Number* g, const Ipopt::Number* lambda,
                                Ipopt::Number obj_value,
                const Ipopt::IpoptData* ip_data,
                Ipopt::IpoptCalculatedQuantities* ip_cq)
{
    // std::cout<<"finalize_solution"<<std::endl;
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
    if (best_ == nullptr)
        best_ = std::make_shared<RealPoint>(RealPoint(start_->scope()));
    for (size_t i=0; i<n; i++)
        (*best_)[i] = x[i];
        
    // std::cout<<"finalize_solution"<<std::endl;
}




}