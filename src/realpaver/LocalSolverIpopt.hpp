///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LOCAL_SOLVER_IPOPT_HPP
#define REALPAVER_LOCAL_SOLVER_IPOPT_HPP

#include "realpaver/LocalSolver.hpp"

#include <IpTNLP.hpp>

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for local optimization solvers.
///////////////////////////////////////////////////////////////////////////////
class LocalSolverIpopt : public LocalSolver, Ipopt::TNLP {
public:
   /// Default constructor
   LocalSolverIpopt(const Problem& pb);
   LocalSolverIpopt(const RealFunction& obj, const RealFunctionVector& ctrs);

   /// Virtual destructor
   virtual ~LocalSolverIpopt();

   /// Minimization of a problem
   /// @param reg interval region in the search space
   /// @param src starting point that belongs to the region
   /// @param dest final point found by the optimization procedure
   /// @return an optimization status
   virtual OptimizationStatus minimize(const IntervalRegion& reg,
                                       const RealPoint& src,
                                       RealPoint& dest);
   
   virtual bool get_nlp_info(Ipopt::Index& n, Ipopt::Index& m, Ipopt::Index& nnz_jac_g,
                            Ipopt::Index& nnz_h_lag, IndexStyleEnum& index_style);

   virtual bool get_bounds_info(Ipopt::Index n, Ipopt::Number* x_l, Ipopt::Number* x_u,
                               Ipopt::Index m, Ipopt::Number* g_l, Ipopt::Number* g_u);

   virtual bool get_starting_point(Ipopt::Index n, bool init_x, Ipopt::Number* x,
                                   bool init_z, Ipopt::Number* z_L, Ipopt::Number* z_U,
                                   Ipopt::Index m, bool init_lambda,
                                   Ipopt::Number* lambda);
   
   virtual bool eval_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number& obj_value);

   virtual bool eval_grad_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number* grad_f);

   virtual bool eval_g (Ipopt::Index n, const Ipopt::Number *x, bool new_x, Ipopt::Index m, Ipopt::Number *g);

   virtual bool eval_jac_g (Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                                 Ipopt::Index m, Ipopt::Index nele_jac, Ipopt::Index *iRow,
                                 Ipopt::Index *jCol, Ipopt::Number *values);
   
   virtual bool eval_h (Ipopt::Index n, const Ipopt::Number *x, bool new_x,Ipopt::Number obj_factor,
                           Ipopt::Index m, const Ipopt::Number *lambda, bool new_lambda, Ipopt::Index nele_hess,
                           Ipopt::Index *iRow, Ipopt::Index *jCol, Ipopt::Number *values);

   virtual Ipopt::Index get_number_of_nonlinear_variables();

   virtual bool get_list_of_nonlinear_variables (Ipopt::Index num_nonlin_vars, Ipopt::Index *pos_nonlin_vars);

   virtual void finalize_solution(Ipopt::SolverReturn status,
                                  Ipopt::Index n, const Ipopt::Number* x, const Ipopt::Number* z_L, const Ipopt::Number* z_U,
                                  Ipopt::Index m, const Ipopt::Number* g, const Ipopt::Number* lambda,
                                  Ipopt::Number obj_value,
				  const Ipopt::IpoptData* ip_data,
				  Ipopt::IpoptCalculatedQuantities* ip_cq);

};

} // namespace

#endif
