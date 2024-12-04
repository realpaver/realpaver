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
        string fullpath = std::filesystem::current_path().string()+"/"+filepath;
        if (!parser.parseFile(fullpath,*pb))
        {
            std::cerr<<"Unable to parse file "<<fullpath<<std::endl;
            std::cerr<<parser.getParseError()<<std::endl;
            return false;
        }
    }
    if (obj == nullptr)
        obj = std::make_shared<RealFunction>(RealFunction(pb->getObjective().getTerm()));

    return true;
}

void test_localopt(std::string filepath)
{
    std::cerr<<"\n*** Solving "<<filepath<<std::endl;

    if (!init_pointers(filepath))
    {
        TEST_TRUE(false);
    }

    NLPSolver rp_localopt(*pb);

    RealPoint sol(pb->scope());
    OptimizationStatus status = OptimizationStatus::Other;
    IntervalBox box(pb->scope());
    RealPoint start(pb->scope(),box.midpoint());

    std::cerr<<"Search space: "<<box<<std::endl;
    std::cerr<<"Starting point: "<<start<<std::endl;

    status = rp_localopt.minimize(box,start);
    std::cerr<<"\n*** Solving status with NLPSolver:"<<status<<std::endl;
    std::cerr<<"Best point: "<<rp_localopt.bestPoint()<<" with obj: "<<rp_localopt.bestVal()<<std::endl;

    TEST_TRUE(status != OptimizationStatus::Other);
    std::cerr<<"\nDone!"<<std::endl;
}

int main()
{
    // test_ipopt_ex();

    pb = nullptr;
    obj = nullptr;

    test_localopt("../benchmarks/cop/Beale.rp");

    std::cout<<"Solved!!!!!!!!!!!!"<<std::endl;

    pb = nullptr;
    obj = nullptr;

    test_localopt("../benchmarks/cop/OLS.rp");

    END_TEST
}
