#include "test_config.hpp"
#include "realpaver/RealFunction.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/LocalSolver.hpp"
#include "realpaver/LocalSolverIpopt.hpp"

#include "IpIpoptApplication.hpp"

using namespace realpaver;

#include <filesystem>

std::shared_ptr<Problem> pb;
std::shared_ptr<RealFunction> obj;
// std::shared_ptr<RealVectorFunction> ctrs;


// Copy from the IPOPT documentation: https://coin-or.github.io/Ipopt/INTERFACES.html#INTERFACE_CPP_NLP
class HS071_NLP : public Ipopt::TNLP {
    public:
        HS071_NLP(){}
        ~HS071_NLP(){}

        bool get_nlp_info(
            Ipopt::Index&          n,
            Ipopt::Index&          m,
            Ipopt::Index&          nnz_jac_g,
            Ipopt::Index&          nnz_h_lag,
            IndexStyleEnum& Index_style
            )
        {
            // The problem described in HS071_NLP.hpp has 4 variables, x[0] through x[3]
            n = 4;
            
            // one equality constraint and one inequality constraint
            m = 2;
            
            // in this example the jacobian is dense and contains 8 nonzeros
            nnz_jac_g = 8;
            
            // the Hessian is also dense and has 16 total nonzeros, but we
            // only need the lower left corner (since it is symmetric)
            nnz_h_lag = 10;
            
            // use the C style Ipopt::Indexing (0-based)
            Index_style = TNLP::C_STYLE;
            
            return true;
        }
        bool get_bounds_info(
            Ipopt::Index   n,
            Ipopt::Number* x_l,
            Ipopt::Number* x_u,
            Ipopt::Index   m,
            Ipopt::Number* g_l,
            Ipopt::Number* g_u
            )
        {
            // here, the n and m we gave IPOPT in get_nlp_info are passed back to us.
            // If desired, we could assert to make sure they are what we think they are.
            assert(n == 4);
            assert(m == 2);
            
            // the variables have lower bounds of 1
            for( Ipopt::Index i = 0; i < 4; i++ )
            {
                x_l[i] = 1.0;
            }
            
            // the variables have upper bounds of 5
            for( Ipopt::Index i = 0; i < 4; i++ )
            {
                x_u[i] = 5.0;
            }
            
            // the first constraint g1 has a lower bound of 25
            g_l[0] = 25;
            // the first constraint g1 has NO upper bound, here we set it to 2e19.
            // Ipopt interprets any Ipopt::Number greater than nlp_upper_bound_inf as
            // infinity. The default value of nlp_upper_bound_inf and nlp_lower_bound_inf
            // is 1e19 and can be changed through ipopt options.
            g_u[0] = 2e19;
            
            // the second constraint g2 is an equality constraint, so we set the
            // upper and lower bound to the same value
            g_l[1] = g_u[1] = 40.0;
            
            return true;
        }

        // returns the initial point for the problem
        bool get_starting_point(
            Ipopt::Index   n,
            bool    init_x,
            Ipopt::Number* x,
            bool    init_z,
            Ipopt::Number* z_L,
            Ipopt::Number* z_U,
            Ipopt::Index   m,
            bool    init_lambda,
            Ipopt::Number* lambda
            )
        {
            // Here, we assume we only have starting values for x, if you code
            // your own NLP, you can provide starting values for the dual variables
            // if you wish
            assert(init_x == true);
            assert(init_z == false);
            assert(init_lambda == false);
            
            // initialize to the given starting point
            x[0] = 1.0;
            x[1] = 5.0;
            x[2] = 5.0;
            x[3] = 1.0;
            
            return true;
        }

        // returns the value of the objective function
        bool eval_f(
            Ipopt::Index         n,
            const Ipopt::Number* x,
            bool          new_x,
            Ipopt::Number&       obj_value
            )
        {
            assert(n == 4);
            
            obj_value = x[0] * x[3] * (x[0] + x[1] + x[2]) + x[2];
            
            return true;
        }

        // return the gradient of the objective function grad_{x} f(x)
        bool eval_grad_f(
            Ipopt::Index         n,
            const Ipopt::Number* x,
            bool          new_x,
            Ipopt::Number*       grad_f
            )
        {
            assert(n == 4);
            
            grad_f[0] = x[0] * x[3] + x[3] * (x[0] + x[1] + x[2]);
            grad_f[1] = x[0] * x[3];
            grad_f[2] = x[0] * x[3] + 1;
            grad_f[3] = x[0] * (x[0] + x[1] + x[2]);
            
            return true;
        }

        // return the value of the constraints: g(x)
        bool eval_g(
            Ipopt::Index         n,
            const Ipopt::Number* x,
            bool          new_x,
            Ipopt::Index         m,
            Ipopt::Number*       g
            )
        {
            assert(n == 4);
            assert(m == 2);
            
            g[0] = x[0] * x[1] * x[2] * x[3];
            g[1] = x[0] * x[0] + x[1] * x[1] + x[2] * x[2] + x[3] * x[3];
            
            return true;
        }

        // return the structure or values of the Jacobian
        bool eval_jac_g(
            Ipopt::Index         n,
            const Ipopt::Number* x,
            bool          new_x,
            Ipopt::Index         m,
            Ipopt::Index         nele_jac,
            Ipopt::Index*        iRow,
            Ipopt::Index*        jCol,
            Ipopt::Number*       values
            )
        {
            assert(n == 4);
            assert(m == 2);
            
            if( values == NULL )
            {
                // return the structure of the Jacobian
            
                // this particular Jacobian is dense
                iRow[0] = 0;
                jCol[0] = 0;
                iRow[1] = 0;
                jCol[1] = 1;
                iRow[2] = 0;
                jCol[2] = 2;
                iRow[3] = 0;
                jCol[3] = 3;
                iRow[4] = 1;
                jCol[4] = 0;
                iRow[5] = 1;
                jCol[5] = 1;
                iRow[6] = 1;
                jCol[6] = 2;
                iRow[7] = 1;
                jCol[7] = 3;
            }
            else
            {
                // return the values of the Jacobian of the constraints
            
                values[0] = x[1] * x[2] * x[3]; // 0,0
                values[1] = x[0] * x[2] * x[3]; // 0,1
                values[2] = x[0] * x[1] * x[3]; // 0,2
                values[3] = x[0] * x[1] * x[2]; // 0,3
            
                values[4] = 2 * x[0]; // 1,0
                values[5] = 2 * x[1]; // 1,1
                values[6] = 2 * x[2]; // 1,2
                values[7] = 2 * x[3]; // 1,3
            }
            
            return true;
        }

        //return the structure or values of the Hessian
        bool eval_h(
            Ipopt::Index         n,
            const Ipopt::Number* x,
            bool          new_x,
            Ipopt::Number        obj_factor,
            Ipopt::Index         m,
            const Ipopt::Number* lambda,
            bool          new_lambda,
            Ipopt::Index         nele_hess,
            Ipopt::Index*        iRow,
            Ipopt::Index*        jCol,
            Ipopt::Number*       values
            )
        {
            assert(n == 4);
            assert(m == 2);
            
            if( values == NULL )
            {
                // return the structure. This is a symmetric matrix, fill the lower left
                // triangle only.
            
                // the hessian for this problem is actually dense
                Ipopt::Index idx = 0;
                for( Ipopt::Index row = 0; row < 4; row++ )
                {
                    for( Ipopt::Index col = 0; col <= row; col++ )
                    {
                        iRow[idx] = row;
                        jCol[idx] = col;
                        idx++;
                    }
                }
            
                assert(idx == nele_hess);
            }
            else
            {
                // return the values. This is a symmetric matrix, fill the lower left
                // triangle only
            
                // fill the objective portion
                values[0] = obj_factor * (2 * x[3]); // 0,0
            
                values[1] = obj_factor * (x[3]);     // 1,0
                values[2] = 0.;                      // 1,1
            
                values[3] = obj_factor * (x[3]);     // 2,0
                values[4] = 0.;                      // 2,1
                values[5] = 0.;                      // 2,2
            
                values[6] = obj_factor * (2 * x[0] + x[1] + x[2]); // 3,0
                values[7] = obj_factor * (x[0]);                   // 3,1
                values[8] = obj_factor * (x[0]);                   // 3,2
                values[9] = 0.;                                    // 3,3
            
                // add the portion for the first constraint
                values[1] += lambda[0] * (x[2] * x[3]); // 1,0
            
                values[3] += lambda[0] * (x[1] * x[3]); // 2,0
                values[4] += lambda[0] * (x[0] * x[3]); // 2,1
            
                values[6] += lambda[0] * (x[1] * x[2]); // 3,0
                values[7] += lambda[0] * (x[0] * x[2]); // 3,1
                values[8] += lambda[0] * (x[0] * x[1]); // 3,2
            
                // add the portion for the second constraint
                values[0] += lambda[1] * 2; // 0,0
            
                values[2] += lambda[1] * 2; // 1,1
            
                values[5] += lambda[1] * 2; // 2,2
            
                values[9] += lambda[1] * 2; // 3,3
            }
            
            return true;
        }

        void finalize_solution(
            Ipopt::SolverReturn               status,
            Ipopt::Index                      n,
            const Ipopt::Number*              x,
            const Ipopt::Number*              z_L,
            const Ipopt::Number*              z_U,
            Ipopt::Index                      m,
            const Ipopt::Number*              g,
            const Ipopt::Number*              lambda,
            Ipopt::Number                     obj_value,
            const Ipopt::IpoptData*           ip_data,
            Ipopt::IpoptCalculatedQuantities* ip_cq
            )
        {
            // here is where we would store the solution to variables, or write to a file, etc
            // so we could use the solution.
            
            // For this example, we write the solution to the console
            std::cout << std::endl << std::endl << "Solution of the primal variables, x" << std::endl;
            for( Ipopt::Index i = 0; i < n; i++ )
            {
                std::cout << "x[" << i << "] = " << x[i] << std::endl;
            }
            
            std::cout << std::endl << std::endl << "Solution of the bound multipliers, z_L and z_U" << std::endl;
            for( Ipopt::Index i = 0; i < n; i++ )
            {
                std::cout << "z_L[" << i << "] = " << z_L[i] << std::endl;
            }
            for( Ipopt::Index i = 0; i < n; i++ )
            {
                std::cout << "z_U[" << i << "] = " << z_U[i] << std::endl;
            }
            
            std::cout << std::endl << std::endl << "Objective value" << std::endl;
            std::cout << "f(x*) = " << obj_value << std::endl;
            
            std::cout << std::endl << "Final value of the constraints:" << std::endl;
            for( Ipopt::Index i = 0; i < m; i++ )
            {
                std::cout << "g(" << i << ") = " << g[i] << std::endl;
            }
        }

};


void test_ipopt_ex()
{
    // Create a new instance of your nlp
    //  (use a SmartPtr, not raw)
    Ipopt::SmartPtr<Ipopt::TNLP> mynlp = new HS071_NLP();

    // Create a new instance of IpoptApplication
    //  (use a SmartPtr, not raw)
    // We are using the factory, since this allows us to compile this
    // example with an Ipopt Windows DLL
    Ipopt::SmartPtr<Ipopt::IpoptApplication> app = IpoptApplicationFactory();

    // Change some options
    // Note: The following choices are only examples, they might not be
    //       suitable for your optimization problem.
    app->Options()->SetNumericValue("tol", 3.82e-6);
    app->Options()->SetStringValue("mu_strategy", "adaptive");
    app->Options()->SetStringValue("output_file", "ipopt.out");
    // The following overwrites the default name (ipopt.opt) of the options file
    // app->Options()->SetStringValue("option_file_name", "hs071.opt");

    // Initialize the IpoptApplication and process the options
    Ipopt::ApplicationReturnStatus status;
    status = app->Initialize();
    if( status != Ipopt::Solve_Succeeded )
    {
        std::cout << std::endl << std::endl << "*** Error during initialization!" << std::endl;
        TEST_TRUE(false);
    }

    // Ask Ipopt to solve the problem
    status = app->OptimizeTNLP(mynlp);

    if( status == Ipopt::Solve_Succeeded )
    {
        std::cout << std::endl << std::endl << "*** The problem solved!" << std::endl;
    }
    else
    {
        std::cout << std::endl << std::endl << "*** The problem FAILED!" << std::endl;
    }

    // As the SmartPtrs go out of scope, the reference count
    // will be decremented and the objects will automatically
    // be deleted.

    TEST_TRUE(status == Ipopt::Solve_Succeeded);
}



bool init_pointers(std::string filepath)
{
    if (pb == nullptr)
    {
        pb = std::make_shared<Problem>(Problem(filepath));
        Parser parser;
        if (!parser.parseFile(filepath,*pb))
        {
            std::cerr<<"Unable to parse file "<<std::filesystem::current_path()<<filepath<<std::endl;
            return false;
        }
    }
    if (obj == nullptr)
        obj = std::make_shared<RealFunction>(RealFunction(pb->getObjective().getTerm()));

    return true;
}


void test_ipopt(std::string filepath)
{
    std::cerr<<"\n*** Solving "<<filepath<<" with IPOPT:"<<std::endl;
    
    if (!init_pointers(filepath))
    {
        TEST_TRUE(false);
    }

    LocalSolverIpopt rp_ipopt(*pb);
    
    RealPoint sol(pb->scope());
    OptimizationStatus status = OptimizationStatus::Other;
    IntervalRegion box(pb->getDomains());
    RealPoint start(pb->scope(),box.midpoint());
    // try
    // {    
    status = rp_ipopt.minimize(box,start,sol);
    std::cerr<<"\n*** Solving status with IPOPT:"<<status<<std::endl;
    std::cerr<<"Best point: "<<rp_ipopt.get_best_point()<<" with obj: "<<rp_ipopt.get_best_val()<<std::endl;
    // }
    // catch (...)
    // {
    //     std::cerr<<"Unexpected error!"<<std::endl;
    // }

    TEST_TRUE(status == OptimizationStatus::Optimal);
    std::cerr<<"\nDone!"<<std::endl;
}


int main()
{

    test_ipopt_ex();

    pb = nullptr;
    obj = nullptr;

    test_ipopt("../examples/bop/Beale.bop");

    std::cout<<"Solved!!!!!!!!!!!!"<<std::endl;

    pb = nullptr;
    obj = nullptr;

    test_ipopt("../examples/bop/Rapha.bop");

}