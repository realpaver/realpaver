#include "realpaver/Interval.hpp"
#include "realpaver/Range.hpp"
#include "test_config.hpp"

void test_init()
{
   Range e(Range::emptyset()), ee(3, 0), u;
   TEST_TRUE(e.isEmpty());
   TEST_TRUE(ee.isEmpty());
   TEST_TRUE(u.isSetEq(Range::universe()));
}

void test_pos_eq()
{
   Range x(0, 7), y(-1, 3), z(3, 5), u(4, 8), v(8, 9);
   TEST_TRUE(x.isPossiblyEq(y));
   TEST_TRUE(x.isPossiblyEq(z));
   TEST_TRUE(x.isPossiblyEq(u));
   TEST_FALSE(x.isPossiblyEq(v));
   TEST_TRUE(y.isPossiblyEq(y));
}

void test_cert_eq()
{
   Range x(0, 0), y(0, 0), z(3, 5);
   TEST_TRUE(x.isCertainlyEq(y));
   TEST_FALSE(x.isCertainlyEq(z));
}

void test_pos_neq()
{
   Range x(0, 0), y(0, 0), z(3, 5);
   TEST_FALSE(x.isPossiblyNeq(y));
   TEST_TRUE(x.isPossiblyNeq(z));
}

void test_cert_neq()
{
   Range x(0, 7), y(-1, 3), z(8, 9);
   TEST_FALSE(x.isCertainlyNeq(y));
   TEST_TRUE(x.isCertainlyNeq(z));
}

void test_pos_le()
{
   Range x(0, 4), y(2, 8), z(8, 9);
   TEST_TRUE(x.isPossiblyLe(y));
   TEST_TRUE(x.isPossiblyLe(z));
   TEST_TRUE(y.isPossiblyLe(x));
   TEST_TRUE(z.isPossiblyLe(y));
   TEST_FALSE(z.isPossiblyLe(x));
}

void test_cert_le()
{
   Range x(0, 4), y(2, 8), z(8, 9);
   TEST_FALSE(x.isCertainlyLe(y));
   TEST_TRUE(x.isCertainlyLe(z));
   TEST_FALSE(y.isCertainlyLe(x));
   TEST_FALSE(z.isCertainlyLe(y));
}

void test_pos_lt()
{
   Range x(0, 4), y(2, 8), z(8, 9);
   TEST_TRUE(x.isPossiblyLt(y));
   TEST_TRUE(x.isPossiblyLt(z));
   TEST_TRUE(y.isPossiblyLt(x));
   TEST_FALSE(z.isPossiblyLt(y));
   TEST_FALSE(z.isPossiblyLt(x));
}

void test_cert_lt()
{
   Range x(0, 4), y(2, 8), z(8, 9);
   TEST_FALSE(x.isCertainlyLt(y));
   TEST_TRUE(x.isCertainlyLt(z));
   TEST_FALSE(y.isCertainlyLt(z));
   TEST_FALSE(z.isCertainlyLt(y));
}

void test_pos_ge()
{
   Range x(0, 4), y(2, 8), z(8, 9);
   TEST_TRUE(x.isPossiblyGe(y));
   TEST_FALSE(x.isPossiblyGe(z));
   TEST_TRUE(y.isPossiblyGe(x));
   TEST_TRUE(z.isPossiblyGe(y));
   TEST_TRUE(z.isPossiblyGe(x));
}

void test_cert_ge()
{
   Range x(0, 4), y(2, 8), z(8, 9);
   TEST_FALSE(x.isCertainlyGe(y));
   TEST_FALSE(x.isCertainlyGe(z));
   TEST_FALSE(y.isCertainlyGe(z));
   TEST_TRUE(z.isCertainlyGe(y));
   TEST_TRUE(z.isCertainlyGe(x));
}

void test_pos_gt()
{
   Range x(0, 4), y(2, 8), z(8, 9);
   TEST_TRUE(x.isPossiblyGt(y));
   TEST_FALSE(x.isPossiblyGt(z));
   TEST_TRUE(y.isPossiblyGt(x));
   TEST_TRUE(z.isPossiblyGt(y));
   TEST_TRUE(z.isPossiblyGt(x));
}

void test_cert_gt()
{
   Range x(0, 4), y(2, 8), z(8, 9);
   TEST_FALSE(x.isCertainlyGt(y));
   TEST_FALSE(x.isCertainlyGt(z));
   TEST_FALSE(y.isCertainlyGt(x));
   TEST_FALSE(z.isCertainlyGt(y));
   TEST_TRUE(z.isCertainlyGt(x));
}

void test_inter()
{
   Range x(0, 4), y(2, 8), z(8, 9), e(Range::emptyset()),
       xy = x & y, xz = x & z, yz = y & z, xe = x & e, ee = e & e;
   TEST_TRUE(xy.isSetEq(Range(2, 4)));
   TEST_TRUE(xz.isEmpty());
   TEST_TRUE(yz.isSingleton());
   TEST_TRUE(xe.isEmpty());
   TEST_TRUE(ee.isEmpty());
}

void test_hull()
{
   Range x(0, 4), y(2, 8), z(8, 9), e(Range::emptyset()),
       xy = x | y, xz = x | z, yz = y | z, xe = x | e, ee = e | e;
   TEST_TRUE(xy.isSetEq(Range(0, 8)));
   TEST_TRUE(xz.isSetEq(Range(0, 9)));
   TEST_TRUE(yz.isSetEq(Range(2, 9)));
   TEST_TRUE(xe.isSetEq(x));
   TEST_TRUE(ee.isEmpty());
}

void test_round()
{
   Interval x(-1.5, 4.5), y(-2, 10);
   TEST_TRUE(Range::roundInward(x).isSetEq(Range(-1, 4)));
   TEST_TRUE(Range::roundOutward(x).isSetEq(Range(-2, 5)));
   TEST_TRUE(Range::roundInward(y).isSetEq(Range(-2, 10)));
   TEST_TRUE(Range::roundOutward(y).isSetEq(Range(-2, 10)));
}

void test_contains()
{
   Range x(2, 10), e(Range::emptyset());
   TEST_TRUE(x.contains(x));
   TEST_TRUE(x.contains(Range(3, 10)));
   TEST_TRUE(x.contains(Range(2, 9)));
   TEST_TRUE(x.contains(Range(3, 9)));
   TEST_FALSE(x.contains(Range(1, 9)));
   TEST_TRUE(x.contains(e));
   TEST_FALSE(e.contains(x));
}

void test_strictly_contains()
{
   Range x(2, 10), e(Range::emptyset());
   TEST_FALSE(x.strictlyContains(x));
   TEST_FALSE(x.strictlyContains(Range(3, 10)));
   TEST_FALSE(x.strictlyContains(Range(2, 9)));
   TEST_TRUE(x.strictlyContains(Range(3, 9)));
   TEST_FALSE(x.strictlyContains(Range(1, 9)));
   TEST_TRUE(x.strictlyContains(e));
   TEST_FALSE(e.strictlyContains(x));
}

void test_disjoint()
{
   Range x(-3, 4), y(0, 5), z(6, 9), e(Range::emptyset());
   TEST_TRUE(x.isDisjoint(z));
   TEST_TRUE(x.isDisjoint(e));
   TEST_TRUE(e.isDisjoint(x));
   TEST_FALSE(x.isDisjoint(y));
   TEST_TRUE(x.overlaps(y));
   TEST_FALSE(x.overlaps(z));
   TEST_FALSE(x.overlaps(e));
}

void test_add()
{
   Range x(-2, 5), y(1, 8), z(-6, 4), f(-1, 13), px(-2, 3), py(1, 6), pz(-1, 4);
   TEST_TRUE(f.isSetEq(x + y));
   TEST_TRUE(px.isSetEq(addPX(x, y, z)));
   TEST_TRUE(py.isSetEq(addPY(x, y, z)));
   TEST_TRUE(pz.isSetEq(addPZ(x, y, z)));
}

void test_sub()
{
   Range x(-2, 5), y(-8, -1), z(-6, 4), f(-1, 13), px(-2, 3), py(-6, -1), pz(-1, 4);
   TEST_TRUE(f.isSetEq(x - y));
   TEST_TRUE(px.isSetEq(subPX(x, y, z)));
   TEST_TRUE(py.isSetEq(subPY(x, y, z)));
   TEST_TRUE(pz.isSetEq(subPZ(x, y, z)));
}

void test_mul_1()
{
   Range x(1, 3), y(-3, -1), z(-2, 2), f(-9, -1), px(1, 2), py(-2, -1), pz(-2, -1);
   TEST_TRUE(f.isSetEq(x * y));
   TEST_TRUE(px.isSetEq(mulPX(x, y, z)));
   TEST_TRUE(py.isSetEq(mulPY(x, y, z)));
   TEST_TRUE(pz.isSetEq(mulPZ(x, y, z)));
}

void test_mul_2()
{
   Range x(-3, 4), y(1, 7), z(0, 50), f(-21, 28), px(0, 4), py(1, 7), pz(0, 28);
   TEST_TRUE(f.isSetEq(x * y));
   TEST_TRUE(px.isSetEq(mulPX(x, y, z)));
   TEST_TRUE(py.isSetEq(mulPY(x, y, z)));
   TEST_TRUE(pz.isSetEq(mulPZ(x, y, z)));
}

void test_sqr_1()
{
   Range x(-2, 5), y(2, 10), f(0, 25), px(-2, 3), py(2, 10);
   TEST_TRUE(f.isSetEq(sqr(x)));
   TEST_TRUE(px.isSetEq(sqrPX(x, y)));
   TEST_TRUE(py.isSetEq(sqrPY(x, y)));
}

void test_sqr_2()
{
   Range x(-1, 4), y(-3, 5), f(0, 16), px(-1, 2), py(0, 5);
   TEST_TRUE(f.isSetEq(sqr(x)));
   TEST_TRUE(px.isSetEq(sqrPX(x, y)));
   TEST_TRUE(py.isSetEq(sqrPY(x, y)));
}

void test_abs_1()
{
   Range x(3, 6), y(2, 5), f(3, 6), px(3, 5), py(3, 5);
   TEST_TRUE(f.isSetEq(abs(x)));
   TEST_TRUE(px.isSetEq(absPX(x, y)));
   TEST_TRUE(py.isSetEq(absPY(x, y)));
}

void test_abs_2()
{
   Range x(-6, -3), y(2, 5), f(3, 6), px(-5, -3), py(3, 5);
   TEST_TRUE(f.isSetEq(abs(x)));
   TEST_TRUE(px.isSetEq(absPX(x, y)));
   TEST_TRUE(py.isSetEq(absPY(x, y)));
}

void test_min()
{
   Range x(0, 4), y(-2, 8), z(-1, 7), f(-2, 4), px(0, 4), py(-1, 8), pz(-1, 4);
   TEST_TRUE(f.isSetEq(min(x, y)));
   TEST_TRUE(px.isSetEq(minPX(x, y, z)));
   TEST_TRUE(py.isSetEq(minPY(x, y, z)));
   TEST_TRUE(pz.isSetEq(minPZ(x, y, z)));
}

void test_max()
{
   Range x(0, 4), y(-2, 8), z(-1, 7), f(0, 8), px(0, 4), py(-2, 7), pz(0, 7);
   TEST_TRUE(f.isSetEq(max(x, y)));
   TEST_TRUE(px.isSetEq(maxPX(x, y, z)));
   TEST_TRUE(py.isSetEq(maxPY(x, y, z)));
   TEST_TRUE(pz.isSetEq(maxPZ(x, y, z)));
}

void test_div_1()
{
   Range x(1, 8), y(2, 11), z(3, 5), f(1, 4), px(6, 8), py(2, 2), pz(3, 4);
   TEST_TRUE(f.isSetEq(x / y));
   TEST_TRUE(px.isSetEq(divPX(x, y, z)));
   TEST_TRUE(py.isSetEq(divPY(x, y, z)));
   TEST_TRUE(pz.isSetEq(divPZ(x, y, z)));
}

void test_div_2()
{
   Range x(1, 4), y(-2, 3), z(0, 6), f(-4, 4), px(1, 4), py(1, 3), pz(1, 4);
   TEST_TRUE(f.isSetEq(x / y));
   TEST_TRUE(px.isSetEq(divPX(x, y, z)));
   TEST_TRUE(py.isSetEq(divPY(x, y, z)));
   TEST_TRUE(pz.isSetEq(divPZ(x, y, z)));
}

void test_mod()
{
   int n = 7;
   Range x(12, 16), f(0, 6);

   std::pair<Range, Range> p = extMod(x, n);
   TEST_TRUE(p.first.isSetEq(Range(0, 2)));
   TEST_TRUE(p.second.isSetEq(Range(5, 6)));
   TEST_TRUE(f.isSetEq(x % n));

   Range y(0, 3), px(14, 16), py(0, 2);
   TEST_TRUE(px.isSetEq(modPX(x, n, y)));
   TEST_TRUE(py.isSetEq(modPY(x, n, y)));
}

void test_wid()
{
   Range x(-1, 3), y(2);
   TEST_TRUE(x.nbElems() == 5);
   TEST_TRUE(y.nbElems() == 1);
}

int main()
{
   TEST(test_init)
   TEST(test_pos_eq)
   TEST(test_cert_eq)
   TEST(test_pos_neq)
   TEST(test_cert_neq)
   TEST(test_pos_le)
   TEST(test_cert_le)
   TEST(test_pos_lt)
   TEST(test_cert_lt)
   TEST(test_pos_ge)
   TEST(test_cert_ge)
   TEST(test_pos_gt)
   TEST(test_cert_gt)
   TEST(test_inter)
   TEST(test_hull)
   TEST(test_round)
   TEST(test_contains)
   TEST(test_strictly_contains)
   TEST(test_disjoint)
   TEST(test_add)
   TEST(test_sub)
   TEST(test_mul_1)
   TEST(test_mul_2)
   TEST(test_sqr_1)
   TEST(test_sqr_2)
   TEST(test_abs_1)
   TEST(test_abs_2)
   TEST(test_min)
   TEST(test_max)
   TEST(test_div_1)
   TEST(test_div_2)
   TEST(test_mod)
   TEST(test_wid)

   END_TEST
}