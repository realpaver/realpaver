#include <string>
#include "realpaver/Common.hpp"

using namespace realpaver;

std::string test_fun_name = "";

#define TEST(f)                        \
   test_fun_name = #f;                 \
   f();

#define TEST_TRUE(cond)                \
   if (!(cond)) {                      \
      std::cerr << "Fail in '"         \
                << test_fun_name       \
                << "' line "           \
                << __LINE__            \
                << ": "                \
                << "TEST_TRUE( "       \
                << #cond               \
                << " )"                \
                << std::endl;          \
      exit(1);                         \
}

#define TEST_FALSE(cond)               \
   if (cond) {                         \
      std::cerr << "Fail in '"         \
                << test_fun_name       \
                << "' line "           \
                << __LINE__            \
                << ": "                \
                << "TEST_FALSE( "      \
                << #cond               \
                << " )"                \
                << std::endl;          \
      exit(1);                         \
}
