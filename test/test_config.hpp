#include <string>
#include "realpaver/Common.hpp"
#include "realpaver/Exception.hpp"

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

#define TEST_THROW(inst)                     \
   do                                        \
   {                                         \
      bool thrown = false;                   \
      try { inst; }                          \
      catch(Exception& e) { thrown = true; } \
      if (!(thrown)) {                       \
         std::cerr << "Fail in '"            \
                   << realpaver_test_name    \
                   << "' line "              \
                   << __LINE__               \
                   << ": "                   \
                   << "TEST_THROW( "         \
                   << #inst                  \
                   << " )"                   \
                   << std::endl;             \
         realpaver_test_result = 1;          \
      }                                      \
   } while(0)

#define END_TEST exit(realpaver_test_result);

#define INIT_TEST  init();
#define CLEAN_TEST clean();
