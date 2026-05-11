#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/ConstraintPiecewise.hpp"
#include "realpaver/Exception.hpp"
#include "realpaver/Interval.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "test_config.hpp"
#include <memory>

Problem *prob1, *prob2, *prob3;
ConstraintPiecewise *pw1, *pw2, *pw3;
Variable x, y, z1, z2, z3;

void init()
{
   prob1 = new Problem();
   x = prob1->addRealVar(0, 10, "x");
   y = prob1->addRealVar(-10, 10, "y");
   z1 = prob1->addBinaryVar("z1");
   z2 = prob1->addBinaryVar("z2");
   z3 = prob1->addBinaryVar("z3");
   std::shared_ptr<ConstraintPiecewise> rep = std::make_shared<ConstraintPiecewise>(x);
   pw1 = rep.get();

   Interval I0(0, 5), I1(5, 8), I2(9, 11);

   pw1->addPiece(z1, I0, x <= y / 2);
   pw1->addPiece(z2, I1, x == pow(y, 2));
   pw1->addPiece(z3, I2, x == sqrt(y));

   Constraint c(rep);
   prob1->addCtr(c);

   /////////
   std::string pb2_str = R"RP(
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
   prob2 = new Problem();
   Parser parser2;
   parser2.parseStr(pb2_str, *prob2);
   Constraint::SharedRep ctr2_rep = prob2->ctrAt(0).rep();
   pw2 = dynamic_cast<ConstraintPiecewise *>(ctr2_rep.get());

   /////////
   std::string pb3_str = R"RP(
    Variables
        x in [0,2],
        y in [0,10];
    Constraints
        piecewise(x,{
            [0,1] : y <= x,
            [1,2] : y >= x
        });
    )RP";
   prob3 = new Problem();
   Parser parser3;
   parser3.parseStr(pb3_str, *prob3);
   Constraint::SharedRep ctr3_rep = prob3->ctrAt(0).rep();
   pw3 = dynamic_cast<ConstraintPiecewise *>(ctr3_rep.get());
}

void clean()
{
   delete prob1;
   delete prob2;
   delete prob3;
}

// Test creation of problem with 3 additional binary variables
void test_create()
{
   Interval I0(0, 5), I1(5, 8), I2(9, 11);

   TEST_TRUE(prob1->nbVars() == 5);
   TEST_TRUE(pw1->interval(0).isSetEq(I0));
   TEST_TRUE(pw1->interval(1).isSetEq(I1));
   TEST_TRUE(pw1->interval(2).isSetEq(I2));
}

// all binaries = 0 -> inconsistency
void testDB_empty1()
{
   DomainBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0);
   Proof p;

   std::unique_ptr<Domain> z1Dom(new BinaryDomain(ZeroOne::zero()));
   B.set(pw1->binary(0), std::move(z1Dom));
   std::unique_ptr<Domain> z2Dom(new BinaryDomain(ZeroOne::zero()));
   B.set(pw1->binary(1), std::move(z2Dom));
   std::unique_ptr<Domain> z3Dom(new BinaryDomain(ZeroOne::zero()));
   B.set(pw1->binary(2), std::move(z3Dom));

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty);
}

void testIB_empty1()
{
   IntervalBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0);
   Proof p;

   // All binaries = 0 -> inconsistency
   B.set(pw1->binary(0), Interval::zero());
   B.set(pw1->binary(1), Interval::zero());
   B.set(pw1->binary(2), Interval::zero());

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty);
}

// sum(binaries) > 1 -> inconsistency
void testDB_empty2()
{
   DomainBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0);
   Proof p;

   std::unique_ptr<Domain> z1Dom1(new BinaryDomain(ZeroOne::one()));
   B.set(pw1->binary(0), std::move(z1Dom1));
   std::unique_ptr<Domain> z2Dom1(new BinaryDomain(ZeroOne::one()));
   B.set(pw1->binary(1), std::move(z2Dom1));

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty);
}

void testIB_empty2()
{
   IntervalBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0);
   Proof p;

   B.set(pw1->binary(0), Interval::one());
   B.set(pw1->binary(1), Interval::one());

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty);
}

// inconsistency between active binary and interval
void testDB_empty3()
{
   DomainBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0); // The piecewise constraint
   Proof p;

   // b_1 and interval_3 active -> incons
   std::unique_ptr<Domain> xDom3(new IntervalDomain(Interval(8, 10)));
   B.set(x, std::move(xDom3));
   std::unique_ptr<Domain> z1Dom3(new BinaryDomain(ZeroOne::one()));
   B.set(pw1->binary(0), std::move(z1Dom3));

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty);
}

void testIB_empty3()
{
   IntervalBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0);
   Proof p;

   // inconsistency between binaries and guard interval
   B.set(x, Interval(8, 10));
   B.set(pw1->binary(0), Interval::one());

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty);
}

// Inconsistency between guard variable domain and guard intervals
void testDB_empty4()
{
   DomainBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0); // The piecewise constraint
   Proof p;

   // guard variable domain does not intersect guard interval
   std::unique_ptr<Domain> xDom2(new IntervalDomain(Interval(8.1, 8.9)));
   B.set(x, std::move(xDom2));
   std::unique_ptr<Domain> z1Dom2(new BinaryDomain(ZeroOne::one()));
   B.set(pw1->binary(0), std::move(z1Dom2));

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty);
}

void testIB_empty4()
{
   IntervalBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0); // The piecewise constraint
   Proof p;

   B.set(x, Interval(8.1, 8.9));

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty);
}

// inconsistency of activated constraint
void testDB_empty5()
{
   DomainBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0); // The piecewise constraint
   Proof p;

   // guard variable domain does not intersect guard interval
   std::unique_ptr<Domain> xDom2(new IntervalDomain(Interval(0, 5)));
   B.set(x, std::move(xDom2));
   std::unique_ptr<Domain> yDom2(new IntervalDomain(Interval(-5, -1)));
   B.set(y, std::move(yDom2));
   std::unique_ptr<Domain> z1Dom2(new BinaryDomain(ZeroOne::one()));
   B.set(pw1->binary(0), std::move(z1Dom2));

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty);
}

void testIB_empty5()
{
   IntervalBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0); // The piecewise constraint
   Proof p;

   B.set(x, Interval(0, 5));
   B.set(y, Interval(-5, -1));
   B.set(pw1->binary(0), Interval::one());

   p = c.contract(B);
   TEST_TRUE(p == Proof::Empty);
}

// a single piece active and active constraint contraction return inner
void testDB_inner()
{
   DomainBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0);
   Proof p;

   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(1, 1.1)));
   B.set(x, std::move(xDom));
   std::unique_ptr<Domain> yDom(new IntervalDomain(Interval(3, 5)));
   B.set(y, std::move(yDom));
   std::unique_ptr<Domain> z1Dom(new BinaryDomain(ZeroOne::one()));
   B.set(pw1->binary(0), std::move(z1Dom));
   std::unique_ptr<Domain> z2Dom(new BinaryDomain(ZeroOne::universe()));
   B.set(pw1->binary(1), std::move(z2Dom));
   std::unique_ptr<Domain> z3Dom(new BinaryDomain(ZeroOne::zero()));
   B.set(pw1->binary(2), std::move(z3Dom));

   p = c.contract(B);
   TEST_TRUE(p == Proof::Inner);
}

void testIB_inner()
{
   IntervalBox B(prob1->scope());
   Constraint c = prob1->ctrAt(0);
   Proof p;

   B.set(x, Interval(1, 1.1));
   B.set(y, Interval(3, 5));
   B.set(pw1->binary(0), Interval::one());
   B.set(pw1->binary(1), Interval(0, 1));
   B.set(pw1->binary(2), Interval::zero());

   p = c.contract(B);
   TEST_TRUE(p == Proof::Inner);
}

// bo active piece and no inconsistency
void testDB_maybe1()
{
   Constraint c = prob1->ctrAt(0);
   DomainBox B(prob1->scope());
   Proof p;

   p = c.contract(B);
   TEST_TRUE(p == Proof::Maybe);
}
void testIB_maybe1()
{
   Constraint c = prob1->ctrAt(0);
   IntervalBox B(prob1->scope());
   Proof p;

   p = c.contract(B);
   TEST_TRUE(p == Proof::Maybe);
}

// A single constraint active that contract to Maybe
void testDB_maybe2()
{
   Constraint c = prob1->ctrAt(0);
   DomainBox B(prob1->scope());
   Proof p;

   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(0, 5)));
   B.set(x, std::move(xDom));
   std::unique_ptr<Domain> yDom(new IntervalDomain(Interval(0, 5)));
   B.set(y, std::move(yDom));
   std::unique_ptr<Domain> z1Dom(new BinaryDomain(ZeroOne::one()));
   B.set(pw1->binary(0), std::move(z1Dom));

   p = c.contract(B);
   TEST_TRUE(p == Proof::Maybe);
}

void testIB_maybe2()
{
   Constraint c = prob1->ctrAt(0);
   IntervalBox B(prob1->scope());
   Proof p;

   B.set(x, Interval(0, 5));
   B.set(y, Interval(0, 5));
   B.set(pw1->binary(0), Interval::one());

   p = c.contract(B);
   TEST_TRUE(p == Proof::Maybe);
}

void test_prob2_1()
{
   Constraint c = prob2->ctrAt(0);
   DomainBox B(prob2->scope());
   TEST_TRUE(c.isSatisfied(B) == Proof::Maybe);
}

void test_prob2_2()
{
   Constraint c = prob2->ctrAt(0);
   IntervalBox B(prob2->scope());
   TEST_TRUE(c.isSatisfied(B) == Proof::Maybe);
}

void test_prob2_3()
{
   Constraint c = prob2->ctrAt(0);
   IntervalBox B(prob2->scope());
   B.set(prob2->scope().var(0), Interval(-1, -1));
   TEST_TRUE(c.violation(B) > 0.0);
}

void test_prob2_4()
{
   Constraint c = prob2->ctrAt(0);
   DomainBox B(prob2->scope());
   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(-1, -1)));
   B.set(prob2->scope().var(0), std::move(xDom));
   TEST_TRUE(c.violation(B) > 0.0);
}

void test_prob2_5()
{
   Constraint c = prob2->ctrAt(0);
   IntervalBox B(prob2->scope());
   B.set(prob2->scope().var(0), Interval(0, 2));
   TEST_TRUE(c.violation(B) == 0.0);
}

void test_prob2_6()
{
   Constraint c = prob2->ctrAt(0);
   DomainBox B(prob2->scope());
   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(0, 2)));
   B.set(prob2->scope().var(0), std::move(xDom));
   TEST_TRUE(c.violation(B) == 0.0);
}

void test_prob3_1()
{
   Constraint c = prob3->ctrAt(0);
   DomainBox B(prob3->scope());

   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(1.1, 2)));
   B.set(prob3->scope().var(0), std::move(xDom));
   std::unique_ptr<Domain> yDom(new IntervalDomain(Interval(5, 10)));
   B.set(prob3->scope().var(1), std::move(yDom));

   std::unique_ptr<Domain> z2Dom(new BinaryDomain(ZeroOne::one()));
   B.set(pw3->binary(1), std::move(z2Dom));

   Proof p;
   p = c.isSatisfied(B);
   TEST_TRUE(p == Proof::Inner);
}

void test_prob3_2()
{
   Constraint c = prob3->ctrAt(0);
   IntervalBox B(prob3->scope());
   B.set(prob3->scope().var(0), Interval(1.1, 2));
   B.set(prob3->scope().var(1), Interval(5, 10));
   B.set(pw3->binary(1), Interval::one());

   Proof p;
   p = c.isSatisfied(B);
   TEST_TRUE(p == Proof::Inner);
}

void test_prob3_3()
{
   Constraint c = prob3->ctrAt(0);
   DomainBox B(prob3->scope());

   std::unique_ptr<Domain> xDom(new IntervalDomain(Interval(1.1, 2)));
   B.set(prob3->scope().var(0), std::move(xDom));
   std::unique_ptr<Domain> yDom(new IntervalDomain(Interval(0, 0.1)));
   B.set(prob3->scope().var(1), std::move(yDom));

   std::unique_ptr<Domain> z2Dom(new BinaryDomain(ZeroOne::one()));
   B.set(pw3->binary(1), std::move(z2Dom));

   Proof p;
   p = c.isSatisfied(B);
   TEST_TRUE(p == Proof::Empty);
}

void test_prob3_4()
{
   Constraint c = prob3->ctrAt(0);
   IntervalBox B(prob3->scope());

   B.set(prob3->scope().var(0), Interval(1.1, 2));
   B.set(prob3->scope().var(1), Interval(0, 0.1));
   B.set(pw3->binary(1), Interval::one());

   Proof p;
   p = c.isSatisfied(B);
   TEST_TRUE(p == Proof::Empty);
}

void test_create2()
{
   std::string pb_str = R"RP(
     Variables
         x in [0,1],
         y in [-10,10];
     Constraints
         piecewise(x,{
             [0,0.21] : y == 2,
             [0.21,0.6] : y == 4,
             [0.6,1]: y == 8
         });
     )RP";
   Problem pb;
   Parser parser;
   parser.parseStr(pb_str, pb);
   std::cout << pb << std::endl;
   TEST_TRUE(pb.nbVars() == 5)
   TEST_TRUE(pb.nbCtrs() == 1)
}

int main()
{
   INIT_TEST

   TEST(test_create)
   TEST(test_create2)

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

   TEST(test_prob2_1)
   TEST(test_prob2_2)
   TEST(test_prob2_3)
   TEST(test_prob2_4)
   TEST(test_prob2_5)
   TEST(test_prob2_6)

   TEST(test_prob3_1)
   TEST(test_prob3_2)
   TEST(test_prob3_3)
   TEST(test_prob3_4)

   CLEAN_TEST
   END_TEST
}
