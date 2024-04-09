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
 * @brief  Implementation of NLP solver for Ipopt
 * @author Raphaël Chenouard
 * @date   2023-2-17
*/

#ifndef REALPAVER_NLP_SOLVER_IPOPT_HPP
#define REALPAVER_NLP_SOLVER_IPOPT_HPP

#include <IpTNLP.hpp>
#include "realpaver/NLPModel.hpp"

namespace realpaver {

/// Implementation of NLP solver for Ipopt
class NLPSolver : public NLPModel {
public:
   /// @name Constructors
   ///@{

   /// Constructor from a problem
   NLPSolver(const Problem& pb);

   /// Constructor given an objective function to be minimized
   NLPSolver(const RealFunction& obj);

   /// Constructor given an objective function and a set of constraints
   NLPSolver(const RealFunction& obj, const RealFunctionVector& ctrs);
   ///@}

   /// Destructor
   ~NLPSolver();

   /// Inner class to state the problem for Ipopt
   class LocalTNLP : public Ipopt::TNLP {
   public:
      /**
       * @brief Constructor.
       * @param ls a pointer to a local optimizer object
       * @param box the variable domains
       * @param start a starting point in the given box
       */
      LocalTNLP(NLPSolver* ls, SharedIntervalBox box, const RealPoint& start);

      /// Gets info about the NLP
      bool get_nlp_info(Ipopt::Index& n, Ipopt::Index& m,
                        Ipopt::Index& nnz_jac_g, Ipopt::Index& nnz_h_lag,
                        IndexStyleEnum& index_style);

      /// Gets the variable bounds
      bool get_bounds_info(Ipopt::Index n, Ipopt::Number* x_l, Ipopt::Number* x_u,
                           Ipopt::Index m, Ipopt::Number* g_l, Ipopt::Number* g_u);

      /// Gets the starting point
      bool get_starting_point(Ipopt::Index n, bool init_x, Ipopt::Number* x,
                              bool init_z, Ipopt::Number* z_L,
                              Ipopt::Number* z_U, Ipopt::Index m,
                              bool init_lambda, Ipopt::Number* lambda);

      /// Evaluates the objective function
      virtual bool eval_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number& obj_value);

      /// Evaluates the gradient of the objective function
      bool eval_grad_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number* grad_f);

      /// Evaluates the constraints
      bool eval_g(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                  Ipopt::Index m, Ipopt::Number *g);

      /// Evaluates the gradient of the constraints
      bool eval_jac_g(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                      Ipopt::Index m, Ipopt::Index nele_jac,
                      Ipopt::Index *iRow, Ipopt::Index *jCol,
                      Ipopt::Number *values);

      // bool eval_h(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
      //             Ipopt::Number obj_factor, Ipopt::Index m,
      //             const Ipopt::Number *lambda, bool new_lambda,
      //             Ipopt::Index nele_hess, Ipopt::Index *iRow,
      //             Ipopt::Index *jCol, Ipopt::Number *values);

      /// Returns the number of nonlinear variables
      Ipopt::Index get_number_of_nonlinear_variables();

      /// Gets the list of nonlinear variables
      bool get_list_of_nonlinear_variables(Ipopt::Index num_nonlin_vars,
                                           Ipopt::Index *pos_nonlin_vars);

      /// Finalizes the solution
      void finalize_solution(Ipopt::SolverReturn status, Ipopt::Index n,
                             const Ipopt::Number* x, const Ipopt::Number* z_L,
                             const Ipopt::Number* z_U, Ipopt::Index m,
                             const Ipopt::Number* g,
                             const Ipopt::Number* lambda,
                             Ipopt::Number obj_value,
                             const Ipopt::IpoptData* ip_data,
                             Ipopt::IpoptCalculatedQuantities* ip_cq);

      /// Pointer to the NLP solver
      NLPSolver* ls_;

      SharedIntervalBox box_;  // box in which search for an optimal value
      RealPoint start_;        // starting point
      double best_val_;        // objective function value for optimal point
    };

   OptimizationStatus minimize(const IntervalBox& box,
                               const RealPoint& src) override;
};

} // namespace

#endif
