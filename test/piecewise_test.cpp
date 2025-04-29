#include "realpaver/AssertDebug.hpp"
#include "realpaver/Constraint.hpp"
#include "realpaver/Domain.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/Interval.hpp"
#include "realpaver/IntervalBox.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/ZeroOne.hpp"
#include "test_config.hpp"

Problem *prob;
Variable x("x"), y("y");
// Variable z1("z1"), z2("z2"), z3("z3");
PiecewiseCtr *pw;

void init()
{
   // Nothing to do
}

void clean()
{
   delete prob;
}

void problem1()
{
   string pb_str = R"RP(
    Variables
        x in {1,2,3},
        y in [-10,10];
    Constraints
        piecewise(x,{
            [1,1] : y == 2,
            [2,2] : y == 4,
            [3,3] : y == 6
        });
    )RP";
   if (prob != nullptr)
      delete prob;
   prob = new Problem();
   Parser parser;
   parser.parseStr(pb_str, *prob);
}

void problem2()
{
   string pb_str = R"RP(
    Variables
        x in [0,2],
        y in [0,10];
    Constraints
        piecewise(x,{
            [0,1] : y <= x,
            [1,2] : y >= x
        });
    )RP";
   if (prob != nullptr)
      delete prob;
   prob = new Problem();
   Parser parser;
   parser.parseStr(pb_str, *prob);
}

// Test creation of problem with 3 additional dinary variables
void test_create()
{
   prob = new Problem();
   x = prob->addRealVar(0, 10, "x");
   y = prob->addRealVar(-10, 10, "y");

   std::vector<Interval> intervals;
   intervals.push_back(Interval(0, 5));
   intervals.push_back(Interval(5, 8));
   intervals.push_back(Interval(9, 11));

   std::vector<Constraint> bodies;
   bodies.push_back(x <= y / 2);
   bodies.push_back(x == pow(y, 2));
   bodies.push_back(x == sqrt(y));
   // prob->addCtr(piecewise(x,binaries,intervals,bodies));
   prob->addCtr(piecewise(x, intervals, bodies, prob));

   Constraint::SharedRep ctr_rep = prob->ctrAt(0).rep();
   pw = dynamic_cast<PiecewiseCtr *>(ctr_rep.get());

   TEST_TRUE(prob->nbVars() == 5)

   TEST_TRUE(checkNonOverlappingIntervals(intervals))
}

// All binaries = 0 -> inconsistency
void testDB_empty1()
{
   DomainBox B(prob->scope());
   Constraint c = prob->ctrAt(0); // The piecewise constraint
   Proof p;

   std::unique_ptr<Domain> z1Dom(new BinaryDomain(ZeroOne::zero()));
   B.set(pw->binary(0), std::move(z1Dom));
   std::unique_ptr<Domain> z2Dom(new BinaryDomain(ZeroOne::zero()));
   B.set(pw->binary(1), std::move(z2Dom));
   std::unique_ptr<Domain> z3Dom(new BinaryDomain(ZeroOne::zero()));
   B.set(pw->binary(2), std::move(z3Dom));

   p = c.contract(B);

   TEST_TRUE(p == Proof::Empty)
}
void testIB_empty1()
{
   IntervalBox B(prob->scope());
   Constraint c = prob->ctrAt(0);
   Proof p;

   // All binaries = 0 -> inconsistency
   B.set(pw->binary(0), Interval::zero());
   B.set(pw->binary(1), Interval::zero());
   B.set(pw->binary(2), Interval::zero());
   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty)
}

// sum(binaries) > 1 -> inconsistency
void testDB_empty2()
{
   DomainBox B(prob->scope());
   Constraint c = prob->ctrAt(0); // The piecewise constraint
   Proof p;

   std::unique_ptr<Domain> z1Dom1(new BinaryDomain(ZeroOne::one()));
   B.set(pw->binary(0), std::move(z1Dom1));
   std::unique_ptr<Domain> z2Dom1(new BinaryDomain(ZeroOne::one()));
   B.set(pw->binary(1), std::move(z2Dom1));

   p = c.contract(B);

   TEST_TRUE(p == Proof::Empty)
}
void testIB_empty2()
{
   IntervalBox B(prob->scope());
   Constraint c = prob->ctrAt(0);
   Proof p;

   B.set(pw->binary(0), Interval::one());
   B.set(pw->binary(1), Interval::one());

   p = c.contract(B);

   TEST_TRUE(p == Proof::Empty)
}

// Inconsistency between active binary and interval
void testDB_empty3()
{
   DomainBox B(prob->scope());
   Constraint c = prob->ctrAt(0); // The piecewise constraint
   Proof p;

   // b_1 and interval_3 active -> incons
   std::unique_ptr<Domain> xDom3(new IntervalDomain(Interval(8, 10)));
   B.set(x, std::move(xDom3));
   std::unique_ptr<Domain> z1Dom3(new BinaryDomain(ZeroOne::one()));
   B.set(pw->binary(0), std::move(z1Dom3));

   p = c.contract(B);

   TEST_TRUE(p == Proof::Empty)
}
void testIB_empty3()
{
   IntervalBox B(prob->scope());
   Constraint c = prob->ctrAt(0);
   Proof p;

   // inconsistency between binaries and guard interval
   B.set(x, Interval(8, 10));
   B.set(pw->binary(0), Interval::one());

   p = c.contract(B);

   TEST_TRUE(p == Proof::Empty)
}

// Inconsistency between guard variable domain and guard intervals
void testDB_empty4()
{
   DomainBox B(prob->scope());
   Constraint c = prob->ctrAt(0); // The piecewise constraint
   Proof p;

   // guard variable domain does not intersect guard interval
   std::unique_ptr<Domain> xDom2(new IntervalDomain(Interval(8.1, 8.9)));
   B.set(x, std::move(xDom2));
   std::unique_ptr<Domain> z1Dom2(new BinaryDomain(ZeroOne::one()));
   B.set(pw->binary(0), std::move(z1Dom2));

   p = c.contract(B);

   TEST_TRUE(p == Proof::Empty)
}
void testIB_empty4()
{
   IntervalBox B(prob->scope());
   Constraint c = prob->ctrAt(0); // The piecewise constraint
   Proof p;

   B.set(x, Interval(8.1, 8.9));

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty)
}

// Inconsistency of activated constraint
void testDB_empty5()
{
   DomainBox B(prob->scope());
   Constraint c = prob->ctrAt(0); // The piecewise constraint
   Proof p;

   // guard variable domain does not intersect guard interval
   std::unique_ptr<Domain> xDom2(new IntervalDomain(Interval(0, 5)));
   B.set(x, std::move(xDom2));
   std::unique_ptr<Domain> yDom2(new IntervalDomain(Interval(-5, -1)));
   B.set(y, std::move(yDom2));
   std::unique_ptr<Domain> z1Dom2(new BinaryDomain(ZeroOne::one()));
   B.set(pw->binary(0), std::move(z1Dom2));

   p = c.contract(B);

   TEST_TRUE(p == Proof::Empty)
}
void testIB_empty5()
{
   IntervalBox B(prob->scope());
   Constraint c = prob->ctrAt(0); // The piecewise constraint
   Proof p;

   B.set(x, Interval(0, 5));
   B.set(y, Interval(-5, -1));
   B.set(pw->binary(0), Interval::one());

   p = c.contract(B);

   TEST_TRUE(p == Proof::Empty)
}

// A single piece active and active constraint contraction return inner
void testDB_inner()
{
   DomainBox B(prob->scope());
   Constraint c = prob->ctrAt(0);
   Proof p;

   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(1, 1.1)));
   B.set(x, std::move(xDom));
   std::unique_ptr<Domain> yDom(new IntervalDomain(Interval(3, 5)));
   B.set(y, std::move(yDom));
   std::unique_ptr<Domain> z1Dom(new BinaryDomain(ZeroOne::one()));
   B.set(pw->binary(0), std::move(z1Dom));
   std::unique_ptr<Domain> z2Dom(new BinaryDomain(ZeroOne::universe()));
   B.set(pw->binary(1), std::move(z2Dom));
   std::unique_ptr<Domain> z3Dom(new BinaryDomain(ZeroOne::zero()));
   B.set(pw->binary(2), std::move(z3Dom));

   p = c.contract(B);

   TEST_TRUE(p == Proof::Inner)
}
void testIB_inner()
{
   IntervalBox B(prob->scope());
   Constraint c = prob->ctrAt(0);
   Proof p;

   B.set(x, Interval(1, 1.1));
   B.set(y, Interval(3, 5));
   B.set(pw->binary(0), Interval::one());
   B.set(pw->binary(1), Interval(0, 1));
   B.set(pw->binary(2), Interval::zero());

   p = c.contract(B);

   TEST_TRUE(p == Proof::Inner)
}

// No active piece and no inconsistency
void testDB_maybe1()
{
   Constraint c = prob->ctrAt(0);
   DomainBox B(prob->scope());
   Proof p;

   p = c.contract(B);

   TEST_TRUE(p == Proof::Maybe);
}
void testIB_maybe1()
{
   Constraint c = prob->ctrAt(0);
   IntervalBox B(prob->scope());
   Proof p;

   p = c.contract(B);

   TEST_TRUE(p == Proof::Maybe);
}

// A single constraint active that contract to Maybe
void testDB_maybe2()
{
   Constraint c = prob->ctrAt(0);
   DomainBox B(prob->scope());
   Proof p;

   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(0, 5)));
   B.set(x, std::move(xDom));
   std::unique_ptr<Domain> yDom(new IntervalDomain(Interval(0, 5)));
   B.set(y, std::move(yDom));
   std::unique_ptr<Domain> z1Dom(new BinaryDomain(ZeroOne::one()));
   B.set(pw->binary(0), std::move(z1Dom));

   p = c.contract(B);

   TEST_TRUE(p == Proof::Maybe);
}
void testIB_maybe2()
{
   Constraint c = prob->ctrAt(0);
   IntervalBox B(prob->scope());
   Proof p;

   Constraint::SharedRep ctr_rep = prob->ctrAt(0).rep();
   PiecewiseCtr *pw = dynamic_cast<PiecewiseCtr *>(ctr_rep.get());

   B.set(x, Interval(0, 5));
   B.set(y, Interval(0, 5));
   B.set(pw->binary(0), Interval::one());

   p = c.contract(B);

   TEST_TRUE(p == Proof::Maybe);
}

void test_prob1_1()
{
   Constraint c = prob->ctrAt(0);

   DomainBox B(prob->scope());
   TEST_TRUE(c.isSatisfied(B) == Proof::Maybe)
}
void test_prob1_2()
{
   Constraint c = prob->ctrAt(0);

   IntervalBox B(prob->scope());
   TEST_TRUE(c.isSatisfied(B) == Proof::Maybe)
}
void test_prob1_3()
{
   Constraint c = prob->ctrAt(0);

   IntervalBox B(prob->scope());
   B.set(prob->scope().var(0), Interval(-1, -1));
   TEST_TRUE(c.violation(B) == 2)
}
void test_prob1_4()
{
   Constraint c = prob->ctrAt(0);

   DomainBox B(prob->scope());
   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(-1, -1)));
   B.set(prob->scope().var(0), std::move(xDom));
   TEST_TRUE(c.violation(B) == 2)
}

void test_prob2_1()
{
   Constraint c = prob->ctrAt(0);

   DomainBox B(prob->scope());

   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(1.1, 2)));
   B.set(prob->scope().var(0), std::move(xDom));
   std::unique_ptr<Domain> yDom(new IntervalDomain(Interval(5, 10)));
   B.set(prob->scope().var(1), std::move(yDom));

   Constraint::SharedRep ctr_rep = prob->ctrAt(0).rep();
   pw = dynamic_cast<PiecewiseCtr *>(ctr_rep.get());
   std::unique_ptr<Domain> z2Dom(new BinaryDomain(ZeroOne::one()));
   B.set(pw->binary(1), std::move(z2Dom));

   Proof p;
   p = c.isSatisfied(B);
   TEST_TRUE(p == Proof::Inner)
}
void test_prob2_2()
{
   Constraint c = prob->ctrAt(0);

   IntervalBox B(prob->scope());

   B.set(prob->scope().var(0), Interval(1.1, 2));
   B.set(prob->scope().var(1), Interval(5, 10));

   Constraint::SharedRep ctr_rep = prob->ctrAt(0).rep();
   pw = dynamic_cast<PiecewiseCtr *>(ctr_rep.get());
   B.set(pw->binary(1), Interval::one());

   Proof p;
   p = c.isSatisfied(B);
   TEST_TRUE(p == Proof::Inner)
}

void test_prob2_3()
{
   Constraint c = prob->ctrAt(0);

   DomainBox B(prob->scope());

   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(1.1, 2)));
   B.set(prob->scope().var(0), std::move(xDom));
   std::unique_ptr<Domain> yDom(new IntervalDomain(Interval(0, 0.1)));
   B.set(prob->scope().var(1), std::move(yDom));

   Constraint::SharedRep ctr_rep = prob->ctrAt(0).rep();
   pw = dynamic_cast<PiecewiseCtr *>(ctr_rep.get());
   std::unique_ptr<Domain> z2Dom(new BinaryDomain(ZeroOne::one()));
   B.set(pw->binary(1), std::move(z2Dom));

   Proof p;
   p = c.isSatisfied(B);
   TEST_TRUE(p == Proof::Empty)
}
void test_prob2_4()
{
   Constraint c = prob->ctrAt(0);

   IntervalBox B(prob->scope());

   B.set(prob->scope().var(0), Interval(1.1, 2));
   B.set(prob->scope().var(1), Interval(0, 0.1));

   Constraint::SharedRep ctr_rep = prob->ctrAt(0).rep();
   pw = dynamic_cast<PiecewiseCtr *>(ctr_rep.get());
   B.set(pw->binary(1), Interval::one());

   Proof p;
   p = c.isSatisfied(B);
   TEST_TRUE(p == Proof::Empty)
}
int main()
{
   INIT_TEST

   TEST(test_create)

   TEST(testDB_empty1)
   TEST(testDB_empty2)
   TEST(testDB_empty3)
   TEST(testDB_empty4)
   TEST(testDB_empty5)

   TEST(testIB_empty1)
   TEST(testIB_empty2)
   TEST(testIB_empty3)
   TEST(testIB_empty4)
   TEST(testIB_empty5)

   TEST(testDB_inner)
   TEST(testIB_inner)

   TEST(testDB_maybe1)
   TEST(testDB_maybe2)

   TEST(testIB_maybe1)
   TEST(testIB_maybe2)

   problem1();
   TEST(test_prob1_1)
   TEST(test_prob1_2)
   TEST(test_prob1_3)
   TEST(test_prob1_4)

   problem2();
   TEST(test_prob2_1)
   TEST(test_prob2_2)
   TEST(test_prob2_3)
   TEST(test_prob2_4)

   CLEAN_TEST
   END_TEST
}
