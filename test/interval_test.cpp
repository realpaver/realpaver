#include "realpaver/Interval.hpp"
#include "test_config.hpp"

void test_add_inv_1()
{
   Interval x(-2, 3), y(-3, 4), z(0, 8);
   TEST_TRUE(addPX(x, y, z).isSetEq(Interval(-2, 3)));
   TEST_TRUE(addPY(x, y, z).isSetEq(Interval(-3, 4)));
   TEST_TRUE(addPZ(x, y, z).isSetEq(Interval(0, 7)));
}

void test_add_inv_2()
{
   Interval x(Interval::moreThan(-2)), y(-3, 4), z(Interval::lessThan(8));
   TEST_TRUE(addPX(x, y, z).isSetEq(Interval(-2, 11)));
   TEST_TRUE(addPY(x, y, z).isSetEq(Interval(-3, 4)));
   TEST_TRUE(addPZ(x, y, z).isSetEq(Interval(-5, 8)));
}

void test_add_inv_3()
{
   Interval x(Interval::moreThan(-2)), y(Interval::lessThan(0)), z(Interval::universe());
   TEST_TRUE(addPZ(x, y, z).isUniverse());
}

void test_add_inv_4()
{
   Interval x(0, 1), y(-1, 2), z(4, 7);
   TEST_TRUE(addPZ(x, y, z).isEmpty());
}

void test_add_inv_5()
{
   Interval x(Interval::emptyset()), y(-1, 2), z(4, 7);
   TEST_TRUE(addPZ(x, y, z).isEmpty());
}

void test_min_inv_1()
{
   Interval x(0, 5), y(1, 4), z(2, 6);
   TEST_TRUE(minPX(x, y, z).isSetEq(Interval(2, 5)));
   TEST_TRUE(minPY(x, y, z).isSetEq(Interval(2, 4)));
   TEST_TRUE(minPZ(x, y, z).isSetEq(Interval(2, 4)));
}

void test_min_inv_2()
{
   Interval x(0, 3), y(4, 6), z(4, 6);
   TEST_TRUE(minPZ(x, y, z).isEmpty());
}

void test_min_inv_3()
{
   Interval x(0, 3), y(5, 6), z(-1, 7);
   TEST_TRUE(minPX(x, y, z).isSetEq(x));
   TEST_TRUE(minPY(x, y, z).isSetEq(y));
   TEST_TRUE(minPZ(x, y, z).isSetEq(x));
}

void test_min_inv_4()
{
   Interval x(Interval::universe()), y(Interval::lessThan(2)), z(Interval::universe());
   TEST_TRUE(minPX(x, y, z).isSetEq(x));
   TEST_TRUE(minPY(x, y, z).isSetEq(y));
   TEST_TRUE(minPZ(x, y, z).isSetEq(y));
}

void test_max_inv_1()
{
   Interval x(0, 7), y(1, 4), z(2, 6);
   TEST_TRUE(maxPX(x, y, z).isSetEq(Interval(0, 6)));
   TEST_TRUE(maxPY(x, y, z).isSetEq(Interval(1, 4)));
   TEST_TRUE(maxPZ(x, y, z).isSetEq(Interval(2, 6)));
}

void test_max_inv_2()
{
   Interval x(0, 3), y(1, 4), z(-2, 2);
   TEST_TRUE(maxPX(x, y, z).isSetEq(Interval(0, 2)));
   TEST_TRUE(maxPY(x, y, z).isSetEq(Interval(1, 2)));
   TEST_TRUE(maxPZ(x, y, z).isSetEq(Interval(1, 2)));
}

void test_max_inv_3()
{
   Interval x(0, 3), y(1, 4), z(5, 6);
   TEST_TRUE(maxPZ(x, y, z).isEmpty());
}

void test_max_inv_4()
{
   Interval x(Interval::lessThan(3)), y(Interval::moreThan(2)), z(1, 8);
   TEST_TRUE(maxPX(x, y, z).isSetEq(x));
   TEST_TRUE(maxPY(x, y, z).isSetEq(Interval(2, 8)));
   TEST_TRUE(maxPZ(x, y, z).isSetEq(Interval(2, 8)));
}

void test_sgn_inv_1()
{
   Interval x(1, 7), y(-1, 1);
   TEST_TRUE(sgnPX(x, y).isSetEq(x));
   TEST_TRUE(sgnPY(x, y).isSetEq(Interval(1, 1)));
}

void test_sgn_inv_2()
{
   Interval x(0, 7), y(-1, 1);
   TEST_TRUE(sgnPX(x, y).isSetEq(x));
   TEST_TRUE(sgnPY(x, y).isSetEq(Interval(0, 1)));
}

void test_sgn_inv_3()
{
   Interval x(-1, 7), y(0, 1);
   TEST_TRUE(sgnPX(x, y).isSetEq(Interval(0, 7)));
   TEST_TRUE(sgnPY(x, y).isSetEq(Interval(0, 1)));
}

void test_sgn_inv_4()
{
   Interval x(-1, 0), y(1);
   TEST_TRUE(sgnPX(x, y).isEmpty());
}

void test_sgn_inv_5()
{
   Interval x(0, 2), y(-1);
   TEST_TRUE(sgnPX(x, y).isEmpty());
}

void test_ext_div_1()
{
   Interval x(4, 8), y(-1, 2);
   std::pair<Interval, Interval> z = extDiv(x, y);
   TEST_TRUE(z.first.isSetEq(Interval::lessThan(-4)));
   TEST_TRUE(z.second.isSetEq(Interval::moreThan(2)));
}

void test_ext_div_2()
{
   Interval x(-8, -4), y(-1, 2);
   std::pair<Interval, Interval> z = extDiv(x, y);
   TEST_TRUE(z.first.isSetEq(Interval::lessThan(-2)));
   TEST_TRUE(z.second.isSetEq(Interval::moreThan(4)));
}

void test_ext_div_3()
{
   Interval x(-8, 2), y(-1, 2);
   std::pair<Interval, Interval> z = extDiv(x, y);
   TEST_TRUE(z.first.isUniverse());
   TEST_TRUE(z.second.isEmpty());
}

void test_ext_div_4()
{
   Interval x(-8, 2), y(0, 2);
   std::pair<Interval, Interval> z = extDiv(x, y);
   TEST_TRUE(z.first.isUniverse());
   TEST_TRUE(z.second.isEmpty());
}

void test_ext_div_5()
{
   Interval x(-8, 2), y(-4, -2);
   std::pair<Interval, Interval> z = extDiv(x, y);
   TEST_TRUE(z.first.isSetEq(x / y));
   TEST_TRUE(z.second.isEmpty());
}

void test_ext_div_6()
{
   Interval x(Interval::emptyset()), y(-4, -2);
   std::pair<Interval, Interval> z = extDiv(x, y);
   TEST_TRUE(z.first.isEmpty());
   TEST_TRUE(z.second.isEmpty());
}

void test_set_minus_1()
{
   Interval x(0, 4), y(1, 4);
   std::pair<Interval, Interval> z = setminus(x, y);
   TEST_TRUE(z.first.isSetEq(Interval(0, 1)));
   TEST_TRUE(z.second.isEmpty());
}

void test_set_minus_2()
{
   Interval x(0, 4), y(4, 4);
   std::pair<Interval, Interval> z = setminus(x, y);
   TEST_TRUE(z.first.isSetEq(x));
   TEST_TRUE(z.second.isEmpty());
}

void test_set_minus_3()
{
   Interval x(0, 4), y(-1, 4);
   std::pair<Interval, Interval> z = setminus(x, y);
   TEST_TRUE(z.first.isEmpty());
   TEST_TRUE(z.second.isEmpty());
}

void test_set_minus_4()
{
   Interval x(0, 4), y(1, 3);
   std::pair<Interval, Interval> z = setminus(x, y);
   TEST_TRUE(z.first.isSetEq(Interval(0, 1)));
   TEST_TRUE(z.second.isSetEq(Interval(3, 4)));
}

void test_set_minus_5()
{
   Interval x(0, 4), y(1, 5);
   std::pair<Interval, Interval> z = setminus(x, y);
   TEST_TRUE(z.first.isSetEq(Interval(0, 1)));
   TEST_TRUE(z.second.isEmpty());
}

int main()
{
   TEST(test_add_inv_1)
   TEST(test_add_inv_2)
   TEST(test_add_inv_3)
   TEST(test_add_inv_4)
   TEST(test_add_inv_5)
   TEST(test_min_inv_1)
   TEST(test_min_inv_2)
   TEST(test_min_inv_3)
   TEST(test_min_inv_4)
   TEST(test_max_inv_1)
   TEST(test_max_inv_2)
   TEST(test_max_inv_3)
   TEST(test_max_inv_4)
   TEST(test_sgn_inv_1)
   TEST(test_sgn_inv_2)
   TEST(test_sgn_inv_3)
   TEST(test_sgn_inv_4)
   TEST(test_sgn_inv_5)
   TEST(test_ext_div_1)
   TEST(test_ext_div_2)
   TEST(test_ext_div_3)
   TEST(test_ext_div_4)
   TEST(test_ext_div_5)
   TEST(test_ext_div_6)
   TEST(test_set_minus_1)
   TEST(test_set_minus_2)
   TEST(test_set_minus_3)
   TEST(test_set_minus_4)
   TEST(test_set_minus_5)

   END_TEST
}
