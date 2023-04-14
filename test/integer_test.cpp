#include "test_config.hpp"
#include "realpaver/Integer.hpp"

Integer MIN = Integer::MIN(),
        MAX = Integer::MAX(),
        ZRO = 0;

void test_init()
{
   TEST_TRUE( Integer(0) == 0);
   TEST_TRUE( Integer(1) != 0);
   TEST_TRUE( Integer(1) > 0);
   TEST_TRUE( Integer(1) >= 0);
   TEST_TRUE( Integer(-1) <= 0);
   TEST_TRUE( Integer(-1) < 0);
   TEST_TRUE(MIN < 0);
   TEST_TRUE(MAX > 0);
}

void test_overflow_add()
{
   Integer x;
   TEST_THROW( x = MAX + 1 );
}

void test_overflow_sub()
{
   Integer x;
   TEST_THROW( x = MIN - 1 );
}

void test_usub()
{
   Integer x = -7, y = 7;
   TEST_TRUE( (-x) == y );
   TEST_TRUE( (-y) == x );
}

void test_overflow_mul()
{
   Integer x;
   TEST_THROW( x = MAX * 2 );
   TEST_THROW( x = MIN * 2 );
}

void test_add()
{
   TEST_TRUE( MIN + ZRO  == MIN );
   TEST_TRUE( MAX + ZRO  == MAX );      
}

void test_sub()
{
   TEST_TRUE( MIN - ZRO  == MIN );
   TEST_TRUE( MAX - ZRO  == MAX );      
}

void test_mul()
{
   TEST_TRUE( MIN * ZRO  == ZRO );
   TEST_TRUE( MAX * ZRO  == ZRO );      
}

void test_div()
{
   Integer x;
   TEST_THROW( x = MAX / 0 );
   TEST_TRUE( MIN / MIN  == 1 );
   TEST_TRUE( MAX / MAX  == 1 );      
}

void test_mod()
{
   Integer x;
   TEST_THROW( x = MAX % 0 );

   if (MIN.isEven())
   {
      TEST_TRUE(MIN % 2 == 0);
   }
   else
   {
      TEST_TRUE(MIN % 2 != 0);
   }

   if (MAX.isEven())
   {
      TEST_TRUE(MAX % 2 == 0);
   }
   else
   {
      TEST_TRUE(MAX % 2 != 0);
   }
}

void test_abs()
{
   Integer x = 1, y = -1;
   TEST_TRUE( abs(y) == x );
   TEST_TRUE( abs(x) == x );
}

void test_sqr()
{
   Integer x = 8, y = 64;
   TEST_TRUE( sqr(ZRO) == 0 );
   TEST_TRUE( sqr(x) == y);
}

void test_div_floor()
{
   Integer x = 13, y = 4, u = -13, v = -4;
   TEST_TRUE( divFloor(x,y) == 3);
   TEST_TRUE( divFloor(u,v) == 3);
   TEST_TRUE( divFloor(x,v) == -4);
   TEST_TRUE( divFloor(u,y) == -4);
   TEST_TRUE( divFloor(x,u) == -1);      
}

void test_div_ceil()
{
   Integer x = 13, y = 4, u = -13, v = -4;
   TEST_TRUE( divCeil(x,y) == 4);
   TEST_TRUE( divCeil(u,v) == 4);
   TEST_TRUE( divCeil(x,v) == -3);
   TEST_TRUE( divCeil(u,y) == -3);
   TEST_TRUE( divCeil(x,u) == -1);            
}

void test_sqrt_floor()
{
   Integer x = 13, y = 16;
   TEST_TRUE( sqrtFloor(x) == 3);
   TEST_TRUE( sqrtFloor(y) == 4);
}

void test_sqrt_ceil()
{
   Integer x = 13, y = 16;
   TEST_TRUE( sqrtCeil(x) == 4);
   TEST_TRUE( sqrtCeil(y) == 4);      
}

void test_min()
{
   Integer x = 3, y = 4, u = -3, v = -4;
   TEST_TRUE( min(x,y) == x );
   TEST_TRUE( min(x,u) == u );
   TEST_TRUE( min(u,v) == v );
}

void test_max()
{
   Integer x = 3, y = 4, u = -3, v = -4;
   TEST_TRUE( max(x,y) == y );
   TEST_TRUE( max(x,u) == x );
   TEST_TRUE( max(u,v) == u );      
}

void test_floor()
{
   double x = 1.5, y = -1.5, z = 2.0;
   TEST_TRUE( Integer::floor(x) == 1 );
   TEST_TRUE( Integer::floor(y) == -2 );
   TEST_TRUE( Integer::floor(z) == 2 );

   Integer si;
   double u;

   u = Integer::MAX() * 2.0;
   TEST_THROW( si = Integer::floor(u) );

   u = Integer::MIN() * 2.0;
   TEST_THROW( si = Integer::floor(u) );
}

void test_ceil()
{
   double x = 1.5, y = -1.5, z = 2.0;
   TEST_TRUE( Integer::ceil(x) == 2 );
   TEST_TRUE( Integer::ceil(y) == -1 );
   TEST_TRUE( Integer::ceil(z) == 2 );

   Integer si;
   double u;

   u = Integer::MAX() * 2.0;
   TEST_THROW( si = Integer::ceil(u) );

   u = Integer::MIN() * 2.0;
   TEST_THROW( si = Integer::ceil(u) );
}

void test_cast_double ()
{
   TEST_TRUE( MIN.toDouble() == MIN.get() );
   TEST_TRUE( MAX.toDouble() == MAX.get() );
}  

int main()
{
   TEST(test_init)
   TEST(test_overflow_add)
   TEST(test_overflow_sub)
   TEST(test_usub)
   TEST(test_overflow_mul)
   TEST(test_add)
   TEST(test_sub)
   TEST(test_mul)
   TEST(test_div)
   TEST(test_mod)
   TEST(test_abs)
   TEST(test_sqr)
   TEST(test_div_floor)
   TEST(test_div_ceil)
   TEST(test_sqrt_floor)
   TEST(test_sqrt_ceil)
   TEST(test_min)
   TEST(test_max)
   TEST(test_floor)
   TEST(test_ceil)
   TEST(test_cast_double)

   END_TEST
}