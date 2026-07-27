#include <memory>

#include "realpaver_api.hpp"

using namespace realpaver;

SharedProblem getProblem(std::string &rp_string)
{
   SharedProblem pb = std::make_shared<Problem>();
   Parser parser;
   parser.parseStr(rp_string, *pb);
   return pb;
}

SharedDagExtended getExtendedDag(SharedProblem pb, std::shared_ptr<Env> e = nullptr)
{
   std::shared_ptr<Env> env = e;
   if (env == nullptr)
      env = std::make_shared<Env>();
   ContractorFactory cf(*pb, env);
   return cf.extDag();
}
