#include "realpaver/NodeOptim.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/SpaceOptimFD.hpp"
#include "realpaver/SpaceOptimLU.hpp"
#include "test_config.hpp"

Problem *prob;
Variable x, y;

void init()
{
   prob = new Problem();
   x = prob->addRealVar(-10, 10, "x");
   y = prob->addRealVar(-10, 10, "y");
}

void clean()
{
   delete prob;
}

void test_1()
{
   SpaceOptimLU space;
   space.setRatio(0.0);

   SharedNodeOptim n1 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n1->setLower(4.0).setUpper(6.0);
   n1->setIndex(0);
   space.pushPendingNode(n1);

   SharedNodeOptim n2 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n2->setLower(2.0).setUpper(8.0);
   n2->setIndex(1);
   space.pushPendingNode(n2);

   SharedNodeOptim n3 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n3->setLower(3.0).setUpper(4.0);
   n3->setIndex(2);
   space.pushPendingNode(n3);

   TEST_TRUE(space.getLowestLowerBound() == 2.0);

   SharedNode node = space.popPendingNode();
   TEST_TRUE(node->index() == 1);

   node = space.popPendingNode();
   TEST_TRUE(node->index() == 2);

   node = space.popPendingNode();
   TEST_TRUE(node->index() == 0);

   TEST_TRUE(space.nbPendingNodes() == 0);
}

void test_2()
{
   SpaceOptimLU space;
   space.setRatio(1.0);

   SharedNodeOptim n1 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n1->setLower(4.0).setUpper(6.0);
   n1->setIndex(0);
   space.pushPendingNode(n1);

   SharedNodeOptim n2 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n2->setLower(2.0).setUpper(8.0);
   n2->setIndex(1);
   space.pushPendingNode(n2);

   SharedNodeOptim n3 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n3->setLower(3.0).setUpper(4.0);
   n3->setIndex(2);
   space.pushPendingNode(n3);

   TEST_TRUE(space.getLowestLowerBound() == 2.0);

   SharedNode node = space.popPendingNode();
   TEST_TRUE(node->index() == 2);

   node = space.popPendingNode();
   TEST_TRUE(node->index() == 0);

   node = space.popPendingNode();
   TEST_TRUE(node->index() == 1);

   TEST_TRUE(space.nbPendingNodes() == 0);
}

void test_3()
{
   SpaceOptimFD space;

   SharedNodeOptim n1 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n1->setLower(4.0).setUpper(6.0);
   n1->setIndex(0);
   space.pushPendingNode(n1);
   space.popPendingNode(); // assigns the last node to n1
   space.pushPendingNode(n1);

   SharedNodeOptim n2 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n2->setLower(2.0).setUpper(8.0);
   n2->setIndex(1);
   n2->setParent(0); // parent is n1
   space.pushPendingNode(n2);

   SharedNodeOptim n3 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n3->setLower(3.0).setUpper(4.0);
   n3->setIndex(2);
   n3->setParent(0); // parent is n1
   space.pushPendingNode(n3);

   TEST_TRUE(space.getLowestLowerBound() == 2.0);

   SharedNode node = space.popPendingNode();
   TEST_TRUE(node->index() == 1);

   node = space.popPendingNode();
   TEST_TRUE(node->index() == 2);

   node = space.popPendingNode();
   TEST_TRUE(node->index() == 0);

   TEST_TRUE(space.nbPendingNodes() == 0);
}

void test_4()
{
   SpaceOptimFD space;

   SharedNodeOptim n1 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n1->setLower(4.0).setUpper(6.0);
   n1->setIndex(0);
   space.pushPendingNode(n1);

   SharedNodeOptim n2 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n2->setLower(2.0).setUpper(8.0);
   n2->setIndex(1);
   space.pushPendingNode(n2);

   SharedNodeOptim n3 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n3->setLower(3.0).setUpper(4.0);
   n3->setIndex(2);
   space.pushPendingNode(n3);

   space.setTol(Tolerance(1.0e-4, 1.0e-6));
   space.simplify(2.5);

   TEST_TRUE(space.nbPendingNodes() == 1);
}

void test_5()
{
   SpaceOptimFD space;

   SharedNodeOptim n1 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n1->setLower(4.0).setUpper(6.0);
   n1->setIndex(0);
   space.pushPendingNode(n1);

   SharedNodeOptim n5 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n5->setLower(7.0).setUpper(8.0);
   n5->setIndex(4);
   space.pushPendingNode(n5);

   SharedNodeOptim n6 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n6->setLower(7.0).setUpper(10.0);
   n6->setIndex(5);
   space.pushPendingNode(n6);

   SharedNodeOptim n2 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n1->setLower(1.0).setUpper(10.0);
   n1->setIndex(1);
   space.pushPendingNode(n1);

   SharedNodeOptim n3 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n3->setLower(2.0).setUpper(8.0);
   n3->setIndex(2);
   n3->setParent(1);
   space.pushPendingNode(n3);

   SharedNodeOptim n4 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n4->setLower(3.0).setUpper(4.0);
   n4->setIndex(3);
   n4->setParent(1);
   space.pushPendingNode(n4);

   space.setTol(Tolerance(1.0e-4, 1.0e-6));
   space.simplify(2.5);

   TEST_TRUE(space.nbPendingNodes() == 2);

   SharedNode node = space.popPendingNode();
   TEST_TRUE(node->index() == 1);

   node = space.popPendingNode();
   TEST_TRUE(node->index() == 2);
}

void test_6()
{
   SpaceOptimLU space;

   SharedNodeOptim n1 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n1->setLower(4.0).setUpper(6.0);
   n1->setIndex(0);
   space.pushSolNode(n1);

   SharedNodeOptim n2 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n2->setLower(2.0).setUpper(8.0);
   n2->setIndex(1);
   space.pushSolNode(n2);

   SharedNodeOptim n3 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n3->setLower(3.0).setUpper(4.0);
   n3->setIndex(2);
   space.pushSolNode(n3);

   TEST_TRUE(space.getLowestLowerBound() == 2.0);

   SharedNode node = space.popSolNode();
   TEST_TRUE(node->index() == 1);

   node = space.popSolNode();
   TEST_TRUE(node->index() == 2);

   node = space.popSolNode();
   TEST_TRUE(node->index() == 0);

   TEST_TRUE(space.nbSolNodes() == 0);
}

void test_7()
{
   SpaceOptimLU space;

   SharedNodeOptim n1 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n1->setLower(4.0).setUpper(6.0);
   n1->setIndex(0);
   space.pushSolNode(n1);

   SharedNodeOptim n2 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n2->setLower(2.0).setUpper(8.0);
   n2->setIndex(1);
   space.pushSolNode(n2);

   SharedNodeOptim n3 = std::make_shared<NodeOptim>(prob->scope(), 2);
   n3->setLower(3.0).setUpper(4.0);
   n3->setIndex(2);
   space.pushSolNode(n3);

   space.simplify(3.5);

   SharedNode node = space.popSolNode();
   TEST_TRUE(node->index() == 1);

   node = space.popSolNode();
   TEST_TRUE(node->index() == 2);

   TEST_TRUE(space.nbSolNodes() == 0);
}

int main()
{
   INIT_TEST

   TEST(test_1)
   TEST(test_2)
   TEST(test_3)
   TEST(test_4)
   TEST(test_5)
   TEST(test_6)
   TEST(test_7)

   CLEAN_TEST
   END_TEST
}
