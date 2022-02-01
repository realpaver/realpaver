#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <iostream>

#include "realpaver/Common.hpp"

using namespace std;
using namespace realpaver;

#define REGISTER_TEST(test)                           \
   int main(int argc, char **argv)                    \
   {                                                  \
      CPPUNIT_TEST_SUITE_REGISTRATION( test );        \
      CppUnit::TextUi::TestRunner runner;             \
      CppUnit::TestFactoryRegistry &registry =        \
         CppUnit::TestFactoryRegistry::getRegistry(); \
      runner.addTest( registry.makeTest() );          \
      return runner.run() ? 0 : -1;                   \
   }

#define TEST_TRUE(cond) CPPUNIT_ASSERT(cond)
#define TEST_FAIL(cond) CPPUNIT_ASSERT(!(cond))

#define TEST_THROW(inst)                              \
   do                                                 \
   {                                                  \
      bool thrown = false;                            \
      try { inst; }                                   \
      catch(Menhir m) { thrown = true; }              \
      TEST_TRUE( thrown );                            \
   } while(0)
