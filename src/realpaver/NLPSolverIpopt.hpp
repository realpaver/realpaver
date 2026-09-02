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
 * @file   NLPSolverIpopt.hpp
 * @brief  Implementation of NLP solver for Nlopt
 * @author Raphaël Chenouard
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_NLP_SOLVER_IPOPT_HPP
#define REALPAVER_NLP_SOLVER_IPOPT_HPP

#include "realpaver/IntervalBox.hpp"
#include "realpaver/NLPModel.hpp"

#include "IpIpoptApplication.hpp"
#include "IpTNLP.hpp"

#include <memory>

// using namespace Ipopt;

namespace realpaver {

class NLPSolver;

/// class to state the problem for Ipopt
class RealPaverTNLP : public Ipopt::TNLP {
public:
   /// Constructor
   /// @param ls a pointer to a local optimizer object
   RealPaverTNLP(NLPSolver *ls);

   bool get_nlp_info(Ipopt::Index &n, Ipopt::Index &m, Ipopt::Index &nnz_jac_g,
                     Ipopt::Index &nnz_h_lag, IndexStyleEnum &index_style);
   bool get_bounds_info(Ipopt::Index n, Ipopt::Number *x_l, Ipopt::Number *x_u,
                        Ipopt::Index m, Ipopt::Number *g_l, Ipopt::Number *g_u);

   bool get_starting_point(Ipopt::Index n, bool init_x, Ipopt::Number *x, bool init_z,
                           Ipopt::Number *z_L, Ipopt::Number *z_U, Ipopt::Index m,
                           bool init_lambda, Ipopt::Number *lambda);
   virtual bool eval_f(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                       Ipopt::Number &obj_value);

   bool eval_grad_f(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                    Ipopt::Number *grad_f);

   bool eval_g(Ipopt::Index n, const Ipopt::Number *x, bool new_x, Ipopt::Index m,
               Ipopt::Number *g);

   bool eval_jac_g(Ipopt::Index n, const Ipopt::Number *x, bool new_x, Ipopt::Index m,
                   Ipopt::Index nele_jac, Ipopt::Index *iRow, Ipopt::Index *jCol,
                   Ipopt::Number *values);

   // bool eval_h (Ipopt::Index n, const Ipopt::Number *x, bool new_x,Ipopt::Number
   // obj_factor,
   //                     Ipopt::Index m, const Ipopt::Number *lambda, bool new_lambda,
   //                     Ipopt::Index nele_hess, Ipopt::Index *iRow, Ipopt::Index *jCol,
   //                     Ipopt::Number *values);

   Ipopt::Index get_number_of_nonlinear_variables();

   bool get_list_of_nonlinear_variables(Ipopt::Index num_nonlin_vars,
                                        Ipopt::Index *pos_nonlin_vars);

   void finalize_solution(Ipopt::SolverReturn status, Ipopt::Index n,
                          const Ipopt::Number *x, const Ipopt::Number *z_L,
                          const Ipopt::Number *z_U, Ipopt::Index m,
                          const Ipopt::Number *g, const Ipopt::Number *lambda,
                          Ipopt::Number obj_value, const Ipopt::IpoptData *ip_data,
                          Ipopt::IpoptCalculatedQuantities *ip_cq);

   void update(const IntervalBox &, const RealPoint src);

   double getBestVal() const;

private:
   NLPSolver *ls_;
   Ipopt::Index nnz_jac_g_;
   Ipopt::Index nnz_h_lag_;

   IntervalBox box_; // Interval region in which search for an optimal value
   RealPoint start_; // Starting point
   double best_val_;
};

/// Implementation of NLP solver for Ipopt
class NLPSolver : public NLPModel {
public:
   /// @name Constructors
   ///@{

   /// Constructor from a problem
   NLPSolver(const Problem &pb);

   /// Constructor given an objective function to be minimized
   NLPSolver(const RealFunction &obj);

   /// Constructor given an objective function and a set of constraints
   NLPSolver(const RealFunction &obj, const RealFunctionVector &ctrs);
   ///@}
   /// Destructor
   ~NLPSolver();

   /// No copy
   NLPSolver(const NLPSolver &) = delete;

   /// No assignment
   NLPSolver &operator=(const NLPSolver &) = delete;

   OptimizationStatus minimize(const IntervalBox &box, const RealPoint &src) override;

private:
   Ipopt::SmartPtr<RealPaverTNLP> nlp_; // ipopt problem
   // std::shared_ptr<RealPaverTNLP> nlp_;
   Ipopt::SmartPtr<Ipopt::IpoptApplication> optimizer_; // Ipopt optimizer
   void makeIpoptAlgorithm();
   void makeIpoptProblem();
};

} // namespace realpaver

#endif
