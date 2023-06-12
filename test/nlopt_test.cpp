#include "test_config.hpp"
#include "realpaver/RealFunction.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/LocalSolver.hpp"
#include "realpaver/LocalSolverNlopt.hpp"

#include <nlopt.hpp>

using namespace realpaver;

#include <filesystem>

std::shared_ptr<Problem> pb;
std::shared_ptr<RealFunction> obj;
// std::shared_ptr<RealVectorFunction> ctrs;


double fun(const std::vector<double> &x, std::vector<double> &grad, void* f_data)
{
    RealPoint rp(pb->scope(),0.0);
    for (size_t i=0; i<x.size(); i++)
    {
        rp[i] = x[i];
    }
    double val = obj->eval(rp);

    return val;
}
double fun_grad(const std::vector<double> &x, std::vector<double> &grad, void* f_data)
{
    RealPoint rp(pb->scope(),0.0);
    for (size_t i=0; i<x.size(); i++)
    {
        rp[i] = x[i];
    }
    double val = obj->eval(rp);
    RealVector g(x.size(),0.0);
    obj->diff(rp,g);
    for (size_t i=0; i<g.size(); i++)
    {
        grad.push_back(g[i]);
    }

    return val;
}

void report(nlopt::result status, std::vector<double>& x, double fval)
{
    if (status >0 ) //== nlopt::SUCCESS)
    {
        // std::cerr<<"--- The problem solved with SLSQP!\n"<<std::endl;
        if (status == nlopt::SUCCESS)
            std::cerr<<"Success!"<<std::endl;
        else if (status == nlopt::STOPVAL_REACHED)
            std::cerr<<"Stop val reached!"<<std::endl;
        else if (status == nlopt::FTOL_REACHED)
            std::cerr<<"Ftol reached!"<<std::endl;
        else if (status == nlopt::XTOL_REACHED)
            std::cerr<<"Xtol reached!"<<std::endl;
        else if (status == nlopt::MAXEVAL_REACHED)
            std::cerr<<"Max eval reached!"<<std::endl;
        else //if (status == nlopt::MAXTIME_REACHED)
            std::cerr<<"Max time reached!"<<std::endl;
    }
    else
    {
        std::cerr<<"\n\n*** The Problem FAILED!\n"<<std::endl;
        if (status == nlopt::FAILURE)
            std::cerr<<"Failure!"<<std::endl;
        else if (status == nlopt::INVALID_ARGS)
            std::cerr<<"Invalid args!"<<std::endl;
        else if (status == nlopt::OUT_OF_MEMORY)
            std::cerr<<"Out of memory!"<<std::endl;
        else if (status == nlopt::ROUNDOFF_LIMITED)
            std::cerr<<"Roundoff limited!"<<std::endl;
        else //if (status == nlopt::FORCED_STOP)
            std::cerr<<"Forced stop!"<<std::endl;
    }
    std::cerr<<"Final point: ";
    for (size_t i=0; i<x.size(); i++)
        std::cerr<<x[i]<<" ";
    std::cerr<<" with f="<<fval<<std::endl;
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

void configure_optimizer(nlopt::opt& optimizer, std::vector<double>& x)
{
    size_t n = x.size();
    IntervalRegion box = pb->getDomains();

    optimizer.set_ftol_rel(1e-4);
    optimizer.set_ftol_abs(1e-9);
    optimizer.set_maxeval(1000);
    optimizer.set_maxtime(2);

    std::vector<double> x_l(n);
    std::vector<double> x_u(n);
    for (size_t i=0; i<n; i++)
    {
        x_l[i] = box[i].left();
        x_u[i] = box[i].right();
        x[i] = box[i].midpoint();
    }

    optimizer.set_lower_bounds(x_l);
    optimizer.set_upper_bounds(x_u);
}


void test_solver_creation(std::string filepath)
{
    std::cerr<<"\n*** Creating "<<filepath<<std::endl;
    
    if (!init_pointers(filepath))
    {
        TEST_TRUE(false);
    }

    size_t n = pb->nbVars();
    LocalSolverNlopt nlopt(*pb);
    
}

void test_neldermead(std::string filepath)
{
    std::cerr<<"\n*** Solving "<<filepath<<" with NELDERMEAD:"<<std::endl;
    
    if (!init_pointers(filepath))
    {
        TEST_TRUE(false);
    }

    size_t n = pb->nbVars();
    nlopt::opt optimizer(nlopt::algorithm::LN_NELDERMEAD,n);
    std::vector<double> x(n);
    configure_optimizer(optimizer, x);
    optimizer.set_min_objective(fun,NULL);

    double best_val;
    nlopt::result status = optimizer.optimize(x,best_val);

    report(status,x,best_val);

    TEST_TRUE(status > 0);
}

void test_slsqp(std::string filepath)
{
    std::cerr<<"\n*** Solving "<<filepath<<" with SLSQP:"<<std::endl;
    
    if (!init_pointers(filepath))
    {
        TEST_TRUE(false);
    }

    size_t n = pb->nbVars();
    nlopt::opt optimizer(nlopt::algorithm::LD_SLSQP,n);
    std::vector<double> x(n);
    configure_optimizer(optimizer, x);
    optimizer.set_min_objective(fun_grad,NULL);

    double best_val;
    nlopt::result status = optimizer.optimize(x,best_val);

    report(status,x,best_val);

    TEST_TRUE(status > 0);
}


void test_bobyqa(std::string filepath)
{
    std::cerr<<"\n*** Solving "<<filepath<<" with BOBYQA:"<<std::endl;
    
    if (!init_pointers(filepath))
    {
        TEST_TRUE(false);
    }

    size_t n = pb->nbVars();
    nlopt::opt optimizer(nlopt::algorithm::LN_BOBYQA,n);
    std::vector<double> x(n);
    configure_optimizer(optimizer, x);
    optimizer.set_min_objective(fun,NULL);

    double best_val;
    nlopt::result status = optimizer.optimize(x,best_val);

    report(status,x,best_val);

    TEST_TRUE(status > 0);
}

void test_mma(std::string filepath)
{
    std::cerr<<"\n*** Solving "<<filepath<<" with MMA:"<<std::endl;
    
    if (!init_pointers(filepath))
    {
        TEST_TRUE(false);
    }

    size_t n = pb->nbVars();
    nlopt::opt optimizer(nlopt::algorithm::LD_MMA,n);
    std::vector<double> x(n);
    configure_optimizer(optimizer, x);
    optimizer.set_min_objective(fun_grad,NULL);

    double best_val;
    nlopt::result status = optimizer.optimize(x,best_val);

    report(status,x,best_val);

    TEST_TRUE(status > 0);
}

int main()
{
    pb = nullptr;
    obj = nullptr;

    test_solver_creation("../examples/bop/Beale.bop");
    
    test_neldermead("../examples/bop/Beale.bop");

    test_slsqp("../examples/bop/Beale.bop");

    test_bobyqa("../examples/bop/Beale.bop");

    test_mma("../examples/bop/Beale.bop");

    pb = nullptr;
    obj = nullptr;

    test_neldermead("../examples/bop/Rapha.bop");

    test_slsqp("../examples/bop/Rapha.bop");

    test_bobyqa("../examples/bop/Rapha.bop");

    test_mma("../examples/bop/Rapha.bop");
}