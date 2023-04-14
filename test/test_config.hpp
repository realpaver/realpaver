#include <string>
#include "realpaver/Common.hpp"

using namespace realpaver;

std::string realpaver_test_name = "";
int realpaver_test_result = 0;

#define TEST(f)                        \
   realpaver_test_name = #f;           \
   f();

#define TEST_TRUE(cond)                \
   if (!(cond)) {                      \
      std::cerr << "Fail in '"         \
                << realpaver_test_name \
                << "' line "           \
                << __LINE__            \
                << ": "                \
                << "TEST_TRUE( "       \
                << #cond               \
                << " )"                \
                << std::endl;          \
      realpaver_test_result = 1;       \
   }

#define TEST_FALSE(cond)               \
   if (cond) {                         \
      std::cerr << "Fail in '"         \
                << realpaver_test_name \
                << "' line "           \
                << __LINE__            \
                << ": "                \
                << "TEST_FALSE( "      \
                << #cond               \
                << " )"                \
                << std::endl;          \
      realpaver_test_result = 1;       \
   }

#define END_TEST exit(realpaver_test_result);
