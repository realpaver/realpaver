/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   NLPSolverNlopt.cpp
 * @brief  Implementation of NLP solver for Nlopt
 * @author Raphaël Chenouard
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/NLPSolverIpopt.hpp"
#include "realpaver/RealPoint.hpp"

namespace realpaver {

RealPaverTNLP::RealPaverTNLP(NLPSolver *ls)
    : ls_(ls)
    , box_(ls_->scope())
    , start_(ls_->scope())
{
   size_t n = ls_->nbVars();
   size_t m = ls_->nbCtrs();
   // nnz_jac_g: number of nonzeros in jacobian
   // TODO
   nnz_jac_g_ = 0;
   for (size_t j = 0; j < m; j++)
   {
      for (size_t i = 0; i < n; i++)
      {
         if (ls_->ctrs()->fun(j).scope().contains(ls_->scope().var(i)))
            nnz_jac_g_++;
      }
   }
   // std::cerr<<"nnz_jac_g: "<<nnz_jac_g<<std::endl;
   // nnz_h_lag: number of nonzeros in hessian
   // TODO
   nnz_h_lag_ = 0;
}

bool RealPaverTNLP::get_nlp_info(Ipopt::Index &n, Ipopt::Index &m,
                                 Ipopt::Index &nnz_jac_g, Ipopt::Index &nnz_h_lag,
                                 IndexStyleEnum &index_style)
{
   n = ls_->nbVars();
   m = ls_->nbCtrs();

   nnz_jac_g = nnz_jac_g_;

   nnz_h_lag = nnz_h_lag_;

   // use the C style indexing (0-based).the numbering style used for row/col entries in
   // the sparse matrix format
   index_style = TNLP::C_STYLE;

   return true;
}

bool RealPaverTNLP::get_bounds_info(Ipopt::Index n, Ipopt::Number *x_l,
                                    Ipopt::Number *x_u, Ipopt::Index m,
                                    Ipopt::Number *g_l, Ipopt::Number *g_u)
{
   for (size_t i = 0; i < n; i++)
   {
      x_l[i] = box_[i].left();
      x_u[i] = box_[i].right();
   }

   // g_l and g_u are bounds for the constraints : g_l <= c <= g_u
   // TODO
   for (size_t i = 0; i < m; i++)
   {
      // std::cerr<<"ctr_"<<i<<" in "<<ctrs->fun(i).getImage()<<std::endl;
      g_l[i] = ls_->ctrs()->fun(i).getImage().left();
      g_u[i] = ls_->ctrs()->fun(i).getImage().right();
   }

   return true;
}

bool RealPaverTNLP::get_starting_point(Ipopt::Index n, bool init_x, Ipopt::Number *x,
                                       bool init_z, Ipopt::Number *z_L,
                                       Ipopt::Number *z_U, Ipopt::Index m,
                                       bool init_lambda, Ipopt::Number *lambda)
{
   for (size_t i = 0; i < n; i++)
      x[i] = start_[i];

   // z are dual variables, not mandatory
   // lambda are the dual multipliers, not mandatory
   // TODO?
   // std::cerr<<*start_<<std::endl;

   return true;
}

bool RealPaverTNLP::eval_f(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                           Ipopt::Number &obj_value)
{
   // compute obj_value, i.e. the value of the objective function, from x vector
   RealPoint pt(ls_->scope());
   for (size_t i = 0; i < ls_->scope().size(); i++)
      pt[i] = x[i];
   obj_value = ls_->obj()->eval(pt);

   return true;
}

bool RealPaverTNLP::eval_grad_f(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                                Ipopt::Number *grad_f)
{
   const Scope s = ls_->scope();
   const Scope os = ls_->obj()->scope();
   // compute grad_f, i.e. the gradient of the objective function, from x vector
   RealPoint pt(ls_->scope());
   for (size_t i = 0; i < ls_->obj()->scope().size(); i++)
   {
      pt[i] = x[i]; // x[s.index(os.var(i))];
   }
   RealPoint gf(ls_->obj()->scope());
   ls_->obj()->diff(pt.subPoint(ls_->obj()->scope()), gf);
   // std::cerr<<"gf: "<<gf<<" with x: "<<pt<<std::endl;
   for (size_t i = 0; i < ls_->obj()->scope().size(); i++)
      grad_f[ls_->scope().index(ls_->obj()->scope().var(i))] = gf[i];

   return true;
}

bool RealPaverTNLP::eval_g(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                           Ipopt::Index m, Ipopt::Number *g)
{
   // compute g, i.e. the value of the constraints, from x vector
   RealPoint pt(ls_->scope());
   for (size_t i = 0; i < ls_->scope().size(); i++)
      pt[i] = x[i];
   for (size_t j = 0; j < m; j++)
   {
      g[j] = ls_->ctrs()->fun(j).eval(pt);
   }

   return true;
}

bool RealPaverTNLP::eval_jac_g(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
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
   const Scope s = ls_->scope();
   if (values == nullptr || values == NULL || values == 0)
   {
      size_t nb_val = 0;
      // Return the structure of the Jacobian:
      for (size_t j = 0; j < m; j++)
      {
         // std::cerr<<"ctr"<<j<<": "<<ctrs->fun(j).scope()<<std::endl;
         for (size_t i = 0; i < n; i++)
         {
            if (ls_->ctrs()->fun(j).scope().contains(s.var(i)))
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
      for (size_t i = 0; i < n; i++)
      {
         // std::cerr<<x[i]<<",";
         if (cs.contains(s.var(i)))
            pt[cs.index(s.var(i))] = x[i];
         // pt[i] = x[i];
      }
      // std::cerr<<std::endl<<"x: "<<pt<<std::endl;
      size_t nb_val = 0;
      RealMatrix jac(m, cs.size());
      // ctrs->diff(pt.subPoint(cs),jac);
      ls_->ctrs()->diff(pt, jac);
      // std::cerr<<"J: "<<std::endl<<jac<<std::endl;
      for (size_t j = 0; j < m; j++)
      {
         for (size_t i = 0; i < n; i++)
         {
            if (ls_->ctrs()->fun(j).scope().contains(s.var(i)))
            {
               // std::cerr<<"J["<<j<<","<<i<<"]:"<<jac.get(j,cs.index(s.var(i)))<<std::endl;
               values[nb_val] = jac.get(j, cs.index(s.var(i)));
            }
         }
      }
   }

   return true;
}

// bool RealPaverTNLP::eval_h (Ipopt::Index n, const Ipopt::Number *x, bool
// new_x,Ipopt::Number obj_factor,
//                         Ipopt::Index m, const Ipopt::Number *lambda, bool new_lambda,
//                         Ipopt::Index nele_hess, Ipopt::Index *iRow, Ipopt::Index *jCol,
//                         Ipopt::Number *values)
// {
//     // compute values for the Hessian matrix, where:
//     // - obj_factor is the factor in front of the objective term in the Hessian, $
//     \sigma_f$. (input)
//     // - lambda is the values for the constraint multipliers, $ \lambda$, at which the
//     Hessian is to be evaluated. (input)
//     // - nele_hess is the number of nonzeros in the Hessian (dimension of iRow, jCol,
//     and values). (input)
//     // - iRow is the row indices of entries in the Hessian. (output)
//     // - jCol is the column indices of entries in the Hessian. (output)
//     // - values is the values of the entries in the Hessian. (output)
//     return false;
// }

Ipopt::Index RealPaverTNLP::get_number_of_nonlinear_variables()
{
   return ls_->nbVars();
}

bool RealPaverTNLP::get_list_of_nonlinear_variables(Ipopt::Index num_nonlin_vars,
                                                    Ipopt::Index *pos_nonlin_vars)
{
   for (size_t i = 0; i < ls_->nbVars(); i++)
   {
      pos_nonlin_vars[i] = i;
   }
   return true;
}

void RealPaverTNLP::finalize_solution(Ipopt::SolverReturn status, Ipopt::Index n,
                                      const Ipopt::Number *x, const Ipopt::Number *z_L,
                                      const Ipopt::Number *z_U, Ipopt::Index m,
                                      const Ipopt::Number *g, const Ipopt::Number *lambda,
                                      Ipopt::Number obj_value,
                                      const Ipopt::IpoptData *ip_data,
                                      Ipopt::IpoptCalculatedQuantities *ip_cq)
{
   /*
    status: (in), gives the status of the algorithm as specified in IpAlgTypes.hpp,
        SUCCESS: Algorithm terminated successfully at a locally optimal point, satisfying
    the convergence tolerances (can be specified by options). MAXITER_EXCEEDED: Maximum
    number of iterations exceeded (can be specified by an option). CPUTIME_EXCEEDED:
    Maximum number of CPU seconds exceeded (can be specified by an option).
        STOP_AT_TINY_STEP: Algorithm proceeds with very little progress.
        STOP_AT_ACCEPTABLE_POINT: Algorithm stopped at a point that was converged, not to
    ``desired'' tolerances, but to ``acceptable'' tolerances (see the acceptable-...
    options). LOCAL_INFEASIBILITY: Algorithm converged to a point of local infeasibility.
    Problem may be infeasible. USER_REQUESTED_STOP: The user call-back function
    intermediate_callback (see Section 3.3.4) returned false, i.e., the user code
    requested a premature termination of the optimization. DIVERGING_ITERATES: It seems
    that the iterates diverge. RESTORATION_FAILURE: Restoration phase failed, algorithm
    doesn't know how to proceed. ERROR_IN_STEP_COMPUTATION: An unrecoverable error
    occurred while IPOPT tried to compute the search direction. INVALID_NUMBER_DETECTED:
    Algorithm received an invalid number (such as NaN or Inf) from the NLP; see also
    option check_derivatives_for_naninf. INTERNAL_ERROR: An unknown internal error
    occurred. Please contact the IPOPT authors through the mailing list. n: (in), the
    number of variables in the problem (dimension of $ x$). x: (in), the final values for
    the primal variables, $ x_*$. z_L: (in), the final values for the lower bound
    multipliers, $ z^L_*$. z_U: (in), the final values for the upper bound multipliers, $
    z^U_*$. m: (in), the number of constraints in the problem (dimension of $ g(x)$). g:
    (in), the final value of the constraint function values, $ g(x_*)$. lambda: (in), the
    final values of the constraint multipliers, $ \lambda_*$. obj_value: (in), the final
    value of the objective, $ f(x_*)$. ip_data and ip_cq are provided for expert users.
*/
   RealPoint best(ls_->scope());
   for (size_t i = 0; i < n; i++)
      best[i] = x[i];
   ls_->setBestPoint(best);
   best_val_ = obj_value;
}

double RealPaverTNLP::getBestVal() const
{
   return best_val_;
}

void RealPaverTNLP::update(const IntervalBox &box, const RealPoint src)
{
   box_ = box;
   start_ = src;
}

NLPSolver::NLPSolver(const Problem &pb)
    : NLPModel(pb)
    , nlp_(nullptr)
    , optimizer_(nullptr)
{
   this->setAlgorithm("IPOPT");
   makeIpoptProblem();
   makeIpoptAlgorithm();
}

NLPSolver::NLPSolver(const RealFunction &obj)
    : NLPModel(obj)
    , nlp_(nullptr)
    , optimizer_(nullptr)
{
   this->setAlgorithm("IPOPT");
   makeIpoptProblem();
   makeIpoptAlgorithm();
}

NLPSolver::NLPSolver(const RealFunction &obj, const RealFunctionVector &ctrs)
    : NLPModel(obj, ctrs)
    , nlp_(nullptr)
    , optimizer_(nullptr)
{
   this->setAlgorithm("IPOPT");
   makeIpoptProblem();
   makeIpoptAlgorithm();
}

NLPSolver::~NLPSolver()
{
   // if (nlp_ != nullptr)
   //    delete nlp_;
   // if (optimizer_ != nullptr)
   //    delete optimizer_;
}

OptimizationStatus NLPSolver::minimize(const IntervalBox &box, const RealPoint &src)
{
   REALPAVER_ASSERT(box.scope().contains(scope()),
                    "Bad scope of an interval box given as input of NlpOpt");

   REALPAVER_ASSERT(src.scope().contains(scope()),
                    "Bad scope of astarting point given as input of NlpOpt");

   // projects B and src on the scope of this
   IntervalBox X(box, this->scope());
   RealPoint P(src, this->scope());

   LOG_LOW("Nlopt optimize at point: " << P);

   // makes the optimizer
   // if (optimizer_ == nullptr)
   // makeIpoptAlgorithm();

   nlp_->update(box, src);

   // calls the optimizer
   // Initialize the IpoptApplication and process the options
   Ipopt::ApplicationReturnStatus status;
   status = optimizer_->Initialize();
   if (status != Ipopt::Solve_Succeeded)
   {
      LOG_LOW("*** Error during IPOPT initialization!");
      status_ = OptimizationStatus::Other;
      return status_;
   }
   // Ask Ipopt to solve the problem
   status = optimizer_->OptimizeTNLP(nlp_);
   if (status == Ipopt::Solve_Succeeded)
   {
      status_ = OptimizationStatus::Optimal;
   }
   else if (status == Ipopt::Solved_To_Acceptable_Level)
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
      status_ = OptimizationStatus::Other;
   }
   LOG_LOW("Result of local optimization: " << status_);

   if (status_ != OptimizationStatus::Other)
   {
      best_val_ = nlp_->getBestVal();
      LOG_LOW("Optimum found by Ipopt: " << best_val_);
      LOG_LOW("Best point found by Ipopt: " << (*best_));
   }

   return status_;
}

void NLPSolver::makeIpoptProblem()
{
   nlp_ = new RealPaverTNLP(this);
   // nlp_ = std::make_shared<RealPaverTNLP>(std::make_shared<NLPSolver>(*this));
   // nlp_ = std::make_shared<RealPaverTNLP>(this);
}

void NLPSolver::makeIpoptAlgorithm()
{
   optimizer_ = IpoptApplicationFactory();

   // Change some options
   optimizer_->Options()->SetNumericValue("tol", tol_.getRelTol());
   optimizer_->Options()->SetStringValue("mu_strategy", "adaptive");
   optimizer_->Options()->SetStringValue("line_search_method", "penalty");
   optimizer_->Options()->SetStringValue("fast_step_computation", "yes");
   optimizer_->Options()->SetStringValue("nlp_scaling_method", "gradient-based");
   optimizer_->Options()->SetStringValue("hessian_approximation", "limited-memory");
   optimizer_->Options()->SetNumericValue("max_cpu_time", this->timeLimit());
   optimizer_->Options()->SetIntegerValue("max_iter", this->iterLimit());
   optimizer_->Options()->SetIntegerValue("print_level", 0);
}

} // namespace realpaver
