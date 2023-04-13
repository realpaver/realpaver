#include "realpaver/Common.hpp"

using namespace realpaver;

#define TEST(f) if (!f()) { \
   std::cerr << "Fail: " << #f << std::endl; exit(1); \
}
