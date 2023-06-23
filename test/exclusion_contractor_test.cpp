#include "test_config.hpp"
#include "realpaver/ExclusionRegionContractor.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Parser.hpp"


using namespace realpaver;


void test_1(std::string csp_file)
{
    std::cerr<<"\n*** Loading "<<csp_file<<std::endl;
    
    Parser parser;
    Problem p;
    if (!parser.parseFile(csp_file,p))
    {
        std::cerr<<"Unable to parse file "<<csp_file<<std::endl;
        std::cerr<<parser.getParseError()<<std::endl;
        TEST_TRUE(false);
    }

    ExclusionRegionContractor ec(p);
    IntervalRegion reg(p.getDomains());
    Proof proof = ec.contract(reg);
    std::cerr<<"Contract proof: "<<proof<<std::endl;
    ec.print(std::cerr);

    std::cerr<<"\nDone!"<<std::endl;
}


int main()
{
    test_1("../ncsp/benchmarks/Brown5.rp");

}