///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LOCAL_OPTIMIZER_IPOPT_HPP
#define REALPAVER_LOCAL_OPTIMIZER_IPOPT_HPP

#include "realpaver/LocalOptimizer.hpp"

#include <IpTNLP.hpp>

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for local optimization solvers.
///////////////////////////////////////////////////////////////////////////////
class LocalOptimizerIpopt : public LocalOptimizer {
public:
    /// Default constructor
    LocalOptimizerIpopt(const Problem& pb);
    LocalOptimizerIpopt(const RealFunction& obj, const RealFunctionVector& ctrs);

    /// Inner class to state the problem for Ipopt
    class LocalTNLP : public Ipopt::TNLP {
    public:
        /// Constructor
        /// @param ls a pointer to a local optimizer object
        LocalTNLP(LocalOptimizerIpopt* ls, SharedIntervalRegion reg, std::shared_ptr<RealPoint> start);
        
        bool get_nlp_info(Ipopt::Index& n, Ipopt::Index& m, Ipopt::Index& nnz_jac_g,
                            Ipopt::Index& nnz_h_lag, IndexStyleEnum& index_style);
        bool get_bounds_info(Ipopt::Index n, Ipopt::Number* x_l, Ipopt::Number* x_u,
                            Ipopt::Index m, Ipopt::Number* g_l, Ipopt::Number* g_u);

        bool get_starting_point(Ipopt::Index n, bool init_x, Ipopt::Number* x,
                            bool init_z, Ipopt::Number* z_L, Ipopt::Number* z_U,
                            Ipopt::Index m, bool init_lambda,
                            Ipopt::Number* lambda);
                            virtual bool eval_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number& obj_value);

        bool eval_grad_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number* grad_f);

        bool eval_g (Ipopt::Index n, const Ipopt::Number *x, bool new_x, Ipopt::Index m, Ipopt::Number *g);

        bool eval_jac_g (Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                            Ipopt::Index m, Ipopt::Index nele_jac, Ipopt::Index *iRow,
                            Ipopt::Index *jCol, Ipopt::Number *values);

        // bool eval_h (Ipopt::Index n, const Ipopt::Number *x, bool new_x,Ipopt::Number obj_factor,
        //                     Ipopt::Index m, const Ipopt::Number *lambda, bool new_lambda, Ipopt::Index nele_hess,
        //                     Ipopt::Index *iRow, Ipopt::Index *jCol, Ipopt::Number *values);

        Ipopt::Index get_number_of_nonlinear_variables();

        bool get_list_of_nonlinear_variables (Ipopt::Index num_nonlin_vars, Ipopt::Index *pos_nonlin_vars);

        void finalize_solution(Ipopt::SolverReturn status,
                            Ipopt::Index n, const Ipopt::Number* x, const Ipopt::Number* z_L, const Ipopt::Number* z_U,
                            Ipopt::Index m, const Ipopt::Number* g, const Ipopt::Number* lambda,
                            Ipopt::Number obj_value,
                            const Ipopt::IpoptData* ip_data,
                            Ipopt::IpoptCalculatedQuantities* ip_cq);
        LocalOptimizerIpopt* ls_;

        SharedIntervalRegion reg_;                      // Interval region in which search for an optimal value
        std::shared_ptr<RealPoint> start_;              // Starting point
        double best_val_;                              // objective function value for optimal point
    };


   /// Virtual destructor
   virtual ~LocalOptimizerIpopt();

   /// Minimization of a problem
   /// @param reg interval region in the search space
   /// @param src starting point that belongs to the region
   /// @return an optimization status
   virtual OptimizationStatus minimize(const IntervalRegion& reg,
                                       const RealPoint& src);

};

using DefaultLocalOptimizer=LocalOptimizerIpopt;

} // namespace

#endif
