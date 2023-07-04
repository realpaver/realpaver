#include "test_config.hpp"
#include "realpaver/RealFunction.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/NLPSolver.hpp"

#include "realpaver/Prover.hpp"

using namespace realpaver;

#include <filesystem>

std::shared_ptr<Problem> pb;
std::shared_ptr<RealFunction> obj;


bool init_pointers(std::string filepath)
{
    if (pb == nullptr)
    {
        pb = std::make_shared<Problem>(Problem(filepath));
        Parser parser;
        if (!parser.parseFile(filepath,*pb))
        {
            std::cerr<<"Unable to parse file "<<std::filesystem::current_path()<<filepath<<std::endl;
            std::cerr<<parser.getParseError()<<std::endl;
            return false;
        }
    }
    if (obj == nullptr)
        obj = std::make_shared<RealFunction>(RealFunction(pb->getObjective().getTerm()));

    return true;
}



void test_ipopt(std::string filepath)
{
    std::cerr<<"\n*** Solving "<<filepath<<std::endl;
    
    if (!init_pointers(filepath))
    {
        TEST_TRUE(false);
    }

    NLPSolver rp_ipopt(*pb);
    
    RealPoint sol(pb->scope());
    OptimizationStatus status = OptimizationStatus::Other;
    IntervalRegion box(pb->getDomains());
    RealPoint start(pb->scope(),box.midpoint());

    std::cerr<<"Search space: "<<box<<std::endl;
    std::cerr<<"Starting point: "<<start<<std::endl;
    
    status = rp_ipopt.minimize(box,start);
    std::cerr<<"\n*** Solving status with NLPSolver:"<<status<<std::endl;
    std::cerr<<"Best point: "<<*(rp_ipopt.bestPoint())<<" with obj: "<<rp_ipopt.bestVal()<<std::endl;

    TEST_TRUE(status != OptimizationStatus::Other);
    std::cerr<<"\nDone!"<<std::endl;
}


int main()
{

    // test_ipopt_ex();

    pb = nullptr;
    obj = nullptr;

    test_ipopt("../examples/bop/Beale.bop");

    std::cout<<"Solved!!!!!!!!!!!!"<<std::endl;

    pb = nullptr;
    obj = nullptr;

    test_ipopt("../examples/bop/Rapha.bop");


    END_TEST
}