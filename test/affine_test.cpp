#include "realpaver/AffineForm.hpp"
#include "test_config.hpp"

void test_var()
{
   AffineForm f(1, Interval(-1, 2)), res(0.5, {1.5}, {1});
   TEST_TRUE(f.equals(res));
}

void test_eval()
{
   AffineForm f(-1.0, {2.0, -3.0}, {0, 1}); // -1.0 + 2v0 - 3v1
   Interval res(-6.0, 4.0);
   TEST_TRUE(res.isSetEq(f.eval()));
}

void test_push_1()
{
   AffineForm f(-1.0), res(-1.0, {2.0}, {1});
   f.push(2.0, 1);
   TEST_TRUE(res.equals(f));
}

void test_push_2()
{
   AffineForm f(-1.0, {2.0, -3.0, -1.0}, {1, 2, 4}), // -1.0 + 2v1 - 3v2 - v4
       res(-1.0, {-1.0, 2.0, -3.0, -1.0}, {0, 1, 2, 4});
   f.push(-1.0, 0);
   TEST_TRUE(res.equals(f));
}

void test_push_3()
{
   AffineForm f(-1.0, {2.0, -3.0, -1.0}, {1, 2, 4}), // -1.0 + 2v1 - 3v2 - v4
       res(-1.0, {2.0, -3.0, -4.0, -1.0}, {1, 2, 3, 4});
   f.push(-4.0, 3);
   TEST_TRUE(res.equals(f));
}

void test_push_4()
{
   AffineForm f(-1.0, {2.0, -3.0, -1.0}, {1, 2, 4}), // -1.0 + 2v1 - 3v2 - v4
       res(-1.0, {2.0, -3.0, -1.0, -4.0}, {1, 2, 4, 5});
   f.push(-4.0, 5);
   TEST_TRUE(res.equals(f));
}

void test_push_5()
{
   AffineForm f(-1.0, {2.0, -3.0, -1.0}, {1, 2, 4}), // -1.0 + 2v1 - 3v2 - v4
       res(-1.0, {2.0, -7.0, -1.0}, {1, 2, 4});
   f.push(-4.0, 2);
   TEST_TRUE(res.equals(f));
}

void test_push_6()
{
   AffineForm f(-1.0, {2.0, -3.0, -1.0}, {1, 2, 4}), // -1.0 + 2v1 - 3v2 - v4
       res(-1.0, {2.0, -1.0}, {1, 4});
   f.push(3.0, 2);
   TEST_TRUE(res.equals(f));
}

void test_add_1()
{
   AffineForm f(1.0, {-2.0}, {0}), // 1 - 2v0
       g(1.5, {3.0}, {1}),         // 1.5 + 3v1
       h = f + g, res(2.5, {-2.0, 3.0}, {0, 1});
   TEST_TRUE(h.equals(res));
}

void test_add_2()
{
   AffineForm f(1.0, {-2.0}, {0}, 1.0), // 1   - 2v0 + ve
       g(1.5, {3.0}, {0}, 0.5),         // 1.5 + 3v0 + 0.5ve
       h = f + g, res(2.5, {1.0}, {0}, 1.5);
   TEST_TRUE(h.equals(res));
}

void test_add_3()
{
   AffineForm f(1.0, {-2.0, 2.0}, {0, 1}), // 1   - 2v0 + 2v1
       g(1.5, {2.0, -2.0}, {0, 1}),        // 1.5 + 2v0 - 2v1
       h = f + g, res(2.5, {}, {});
   TEST_TRUE(h.equals(res));
}

void test_add_4()
{
   AffineForm f(1.0, {-2.0}, {1}), // 1 - 2v1
       g(1.5, {3.0}, {0}),         // 1.5 + 3v0
       h = f + g, res(2.5, {3.0, -2.0}, {0, 1});
   TEST_TRUE(h.equals(res));
}

void test_sub_1()
{
   AffineForm f(1.0, {-2.0}, {0}, 1.0), // 1 - 2v0 + ve
       g(1.5, {3.0}, {1}, 0.5),         // 1.5 + 3v1 + 0.5ve
       h = f - g, res(-0.5, {-2.0, -3.0}, {0, 1}, 1.5);
   TEST_TRUE(h.equals(res));
}

void test_sub_2()
{
   AffineForm f(1.0, {-2.0}, {0}), // 1   - 2v0
       g(1.5, {3.0}, {0}),         // 1.5 + 3v0
       h = f - g, res(-0.5, {-5.0}, {0});
   TEST_TRUE(h.equals(res));
}

void test_sub_3()
{
   AffineForm f(1.0, {-2.0, 2.0}, {0, 1}), // 1   - 2v0 + 2v1
       g(1.5, {-2.0, 2.0}, {0, 1}),        // 1.5 - 2v0 + 2v1
       h = f - g, res(-0.5, {}, {});
   TEST_TRUE(h.equals(res));
}

void test_sub_4()
{
   AffineForm f(1.0, {-2.0}, {1}), // 1 - 2v1
       g(1.5, {3.0}, {0}),         // 1.5 + 3v0
       h = f - g, res(-0.5, {-3.0, -2.0}, {0, 1});
   TEST_TRUE(h.equals(res));
}

void test_usub()
{
   AffineForm f(1.0, {3.0, -2.0}, {0, 1}, 2.0), // 1 + 3v0 - 2v1 + 2ve
       h = -f, res(-1.0, {-3.0, 2.0}, {0, 1}, 2.0);
   TEST_TRUE(h.equals(res));
}

void test_mul_1()
{
   AffineForm f(1.5, {3.0}, {0}), // 1.5 + 3v0
       g(2.0, {-2.0}, {1}),       // 2 - 2v1
       h = f * g, res(3.0, {6.0, -3.0}, {0, 1}, 6.0);
   TEST_TRUE(h.equals(res));
}

void test_mul_2()
{
   AffineForm f(1.5, {3.0}, {0}), // 1.5 + 3v0
       g(2.0, {-2.0}, {0}),       // 2 - 2v0
       h = f * g, res(3.0, {3.0}, {0}, 6.0);
   TEST_TRUE(h.equals(res));
}

void test_mul_3()
{
   AffineForm f(1.5, {3.0}, {0}), // 1.5 + 3v0
       g(2.0, {-2.0, 4}, {0, 1}), // 2 - 2v0 + 4v1
       h = f * g, res(3.0, {3.0, 6.0}, {0, 1}, 18.0);
   TEST_TRUE(h.equals(res));
}

void test_mul_4()
{
   AffineForm f(1.5, {3.0, -1.0}, {0, 1}), // 1.5 + 3v0 - v1
       g(2.0, {-2.0, 4}, {0, 1}),          // 2 - 2v0 + 4v1
       h = f * g, res(3.0, {3.0, 4.0}, {0, 1}, 24.0);
   TEST_TRUE(h.equals(res));
}

void test_mul_5()
{
   AffineForm f(1.5, {3.0, -1.0}, {0, 1}, 2.0), // 1.5 + 3v0 - v1 + 2v2
       g(2.0, {-2.0, 4}, {0, 1}, 0.5),          // 2 - 2v0 + 4v1 + 0.5v2
       h = f * g, res(3.0, {3.0, 4.0}, {0, 1}, 43.75);
   TEST_TRUE(h.equals(res));
}

void test_sqr_1()
{
   AffineForm f(1.5, {-2.0}, {0}), // 1.5 - 2v0
       h = sqr(f), res(4.25, {-6.0}, {0}, 2.0);
   TEST_TRUE(h.equals(res));
}

void test_sqr_2()
{
   AffineForm f(1.5, {-2.0, 0.5}, {0, 1}), // 1.5 - 2v0 + 0.5v1
       h = sqr(f), res(5.375, {-6.0, 1.5}, {0, 1}, 3.125);
   TEST_TRUE(h.equals(res));
}

void test_sqr_3()
{
   AffineForm f(1.5, {-2.0, 0.5}, {0, 1}, 10.0), // 1.5 - 2v0 + 0.5v1 + 10ve
       h = sqr(f), res(80.375, {-6.0, 1.5}, {0, 1}, 108.125);
   TEST_TRUE(h.equals(res));
}

void test_udiv_1()
{
   AffineForm f(0, Interval(1, 2)), h = udiv(f), res(0.75, {-0.125}, {0}, 0.125);
   TEST_TRUE(h.equals(res));
}

void test_udiv_2()
{
   AffineForm f(0, Interval(-2, -1)), h = udiv(f), res(-0.75, {-0.125}, {0}, 0.125);
   TEST_TRUE(h.equals(res));
}

void test_udiv_3()
{
   AffineForm f(1.5, {0.25}, {0}, 0.25), h = udiv(f), res(0.75, {-0.0625}, {0}, 0.1875);
   TEST_TRUE(h.equals(res));
}

void test_udiv_4()
{
   AffineForm f(1.0, {2.0}, {0}), h = udiv(f);
   TEST_TRUE(h.errorTerm().isInf());
}

void test_udiv_5()
{
   AffineForm f(0.0, {}, {}), h = udiv(f);
   TEST_TRUE(h.isEmpty());
}

void test_pow()
{
   AffineForm f(1.5, {-2.0}, {0}), // 1.5 - 2v0
       h = pow(f, 3), res(6.375, {-17.5}, {0}, 19.0);
   TEST_TRUE(h.equals(res));
}

void test_min_1()
{
   AffineForm f(1.5, {-2.0}, {0}), // 1.5 - 2v0 in [-0.5, 3.5]
       g(1.0, {1.0}, {1}),         // 1 + v1 in [0, 2]
       h = min(f, g), res(-0.5, {}, {}, Interval(0, 2.5));
   TEST_TRUE(h.equals(res));
}

void test_min_2()
{
   AffineForm f(1.5, {-2.0}, {0}), // 1.5 - 2v0 in [-0.5, 3.5]
       g(5.0, {1.0}, {1}),         // 5 + v1 in [4, 6]
       h = min(f, g), res(f);
   TEST_TRUE(h.equals(res));
}

void test_min_3()
{
   AffineForm f(1.5, {-2.0}, {0}), // 1.5 - 2v0 in [-0.5, 3.5]
       g(-3.0, {1.0}, {1}),        // -3 + v1 in [-4, -2]
       h = min(f, g), res(g);
   TEST_TRUE(h.equals(res));
}

void test_max_1()
{
   AffineForm f(1.5, {-2.0}, {0}), // 1.5 - 2v0 in [-0.5, 3.5]
       g(1.0, {1.0}, {1}),         // 1 + v1 in [0, 2]
       h = max(f, g), res(0.0, {}, {}, Interval(0, 3.5));
   TEST_TRUE(h.equals(res));
}

void test_max_2()
{
   AffineForm f(1.5, {-2.0}, {0}), // 1.5 - 2v0 in [-0.5, 3.5]
       g(5.0, {1.0}, {1}),         // 5 + v1 in [4, 6]
       h = max(f, g), res(g);
   TEST_TRUE(h.equals(res));
}

void test_max_3()
{
   AffineForm f(1.5, {-2.0}, {0}), // 1.5 - 2v0 in [-0.5, 3.5]
       g(-3.0, {1.0}, {1}),        // -3 + v1 in [-4, -2]
       h = max(f, g), res(f);
   TEST_TRUE(h.equals(res));
}

void test_abs_1()
{
   AffineForm f(1.5, {-2.0}, {0}), // 1.5 - 2v0 in [-0.5, 3.5]
       h = abs(f), res(0.0, {}, {}, Interval(0, 3.5));
   TEST_TRUE(h.equals(res));
}

void test_abs_2()
{
   AffineForm f(2.5, {-2.0}, {0}), // 1.5 - 2v0 in [0.5, 4.5]
       h = abs(f), res(f);
   TEST_TRUE(h.equals(res));
}

void test_abs_3()
{
   AffineForm f(-2.5, {-2.0}, {0}), // 1.5 - 2v0 in [-4.5, -0.5]
       h = abs(f), res(-f);
   TEST_TRUE(h.equals(res));
}

void test_sgn_1()
{
   AffineForm f(1.5, {-2.0}, {0}), // 1.5 - 2v0 in [-0.5, 3.5]
       h = sgn(f), res(-1.0, {}, {}, Interval(0, 2.0));
   TEST_TRUE(h.equals(res));
}

void test_sgn_2()
{
   AffineForm f(2.5, {-2.0}, {0}), // 2.5 - 2v0 in [0.5, 4.5]
       h = sgn(f), res(1.0, {}, {});
   TEST_TRUE(h.equals(res));
}

void test_sgn_3()
{
   AffineForm f(-2.5, {-2.0}, {0}), // -2.5 - 2v0 in [-4.5, -0.5]
       h = sgn(f), res(-1.0, {}, {});
   TEST_TRUE(h.equals(res));
}

void test_sqrt_1()
{
   AffineForm f(-2.5, {-2.0}, {0}), // -2.5 - 2v0 in [-4.5, -0.5]
       h = sqrt(f);
   TEST_TRUE(h.isEmpty());
}

void test_sqrt_2()
{
   AffineForm f(2.5, {1.5}, {0}), // 2.5 + 1.5v0 in [1, 4]
       h = sqrt(f), res(1.5, {0.375}, {0}, 0.125);
   TEST_TRUE(h.equals(res));
}

void test_exp()
{
   AffineForm f(1.0, {-1.0}, {0}), // 1.0 - v0 in [0, 2]
       h = exp(f);

   Interval x = Interval::minusOne() + Interval(-1.0e-12, 1.0e-12);
   TEST_TRUE(x.contains(h.cbegin()->itv));

   Interval y = 4.194528049 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(h.constantTerm()));

   Interval z = 2.194528049 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(h.errorTerm()));
}

void test_log()
{
   AffineForm f(1.5, {-0.5}, {0}), // 1.5 - 0.5v0 in [1, 2]
       h = log(f);

   Interval x = -0.25 + Interval(-1.0e-12, 1.0e-12);
   TEST_TRUE(x.contains(h.cbegin()->itv));

   Interval y = 0.34657359 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(h.constantTerm()));

   Interval z = 0.09657359 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(h.errorTerm()));
}

void test_cos_1()
{
   AffineForm f(2.0, {1.0}, {0}), // 2.0 + v0 in [1, 3]
       h = cos(f);

   Interval x = -0.141120008 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(h.cbegin()->itv));

   Interval y = -0.224845095 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(h.constantTerm()));

   Interval z = 0.624027393 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(h.errorTerm()));
}

void test_cos_2()
{
   AffineForm f(-2.0, {1.0}, {0}), // -2.0 + v0 in [-3, -1]
       h = cos(f);

   Interval x = 0.141120008 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(h.cbegin()->itv));

   Interval y = -0.224845095 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(h.constantTerm()));

   Interval z = 0.624027393 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(h.errorTerm()));
}

void test_cos_3()
{
   AffineForm f(0.0, {4.0}, {0}), // 4v0
       h = cos(f), res(-1.0, {}, {}, Interval(0, 2));
   TEST_TRUE(h.equals(res));
}

void test_sin_1()
{
   AffineForm f(3.0, {-1.0}, {0}), // 3.0 - v0 in [2, 4]
       h = sin(f);

   Interval x = 0.416146836 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(h.cbegin()->itv));

   Interval y = 0.076247465 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(h.constantTerm()));

   Interval z = 0.416903124 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(h.errorTerm()));
}

void test_sin_2()
{
   AffineForm f(-3.0, {-1.0}, {0}), // 3.0 - v0 in [-4, -2]
       h = sin(f);

   Interval x = 0.416146836 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(h.cbegin()->itv));

   Interval y = -0.076247465 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(h.constantTerm()));

   Interval z = 0.416903124 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(h.errorTerm()));
}

void test_sin_3()
{
   AffineForm f(1.57, {4.0}, {0}), // 4v0
       h = sin(f), res(-1.0, {}, {}, Interval(0, 2));
   TEST_TRUE(h.equals(res));
}

void test_tan_1()
{
   AffineForm f(-0.25, {0.5}, {0}), // -0.25 + 0.5v0 in [-0.75, 0.25]
       h = tan(f);

   Interval x = 0.5 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(h.cbegin()->itv));

   Interval y = -0.3381272693 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(h.constantTerm()));

   Interval z = 0.0934691905 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(h.errorTerm()));
}

void test_tan_2()
{
   AffineForm f(1.0, {1.0}, {0}), // 1 + v0 in [0, 2]
       h = tan(f), res(0.0, {}, {}, Interval::positive());
   TEST_TRUE(h.equals(res));
}

void test_tan_3()
{
   AffineForm f(1.25, {0.5}, {0}), // 1.25 + 0.5v0 in [0.75, 1.75]
       g = tan(f);
   TEST_TRUE(g.isInf());
}

void test_cosh()
{
   AffineForm f(3.5, {2.0}, {0}), // 3.5 + 2v0 in [1.5, 5.5]
       h = cosh(f);

   Interval x = 4.25855891 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(h.cbegin()->itv));

   Interval y = 62.35020956 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(h.constantTerm()));

   Interval z = 55.73924104 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(h.errorTerm()));
}

void test_sinh()
{
   AffineForm f(-1.0, {2.5}, {0}), // -1 + 2.5v0 in [-3.5, 1.5]
       h = sinh(f);

   Interval x = 2.5 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(h.cbegin()->itv));

   Interval y = -7.20667391 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(h.constantTerm()));

   Interval z = 6.83595337 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(h.errorTerm()));
}

void test_tanh()
{
   AffineForm f(-1.0, {2.5}, {0}), // -1 + 2.5v0 in [-3.5, 1.5]
       h = tanh(f);

   Interval x = 0.0091022118 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(h.cbegin()->itv));

   Interval y = -0.0465148219 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(h.constantTerm()));

   Interval z = 0.94256086 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(h.errorTerm()));
}

void test_udiv_chebyshev_1()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevUdiv(Interval(1, 2), alpha, dzeta, delta);

   Interval x = -0.5;
   TEST_TRUE(alpha.isSetEq(x));

   Interval y = 1.45710678118 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.0428932188 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_udiv_chebyshev_2()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevUdiv(Interval(-2, -1), alpha, dzeta, delta);

   Interval x = -0.5;
   TEST_TRUE(alpha.isSetEq(x));

   Interval y = -1.45710678118 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.0428932188 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sqrt_chebyshev()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSqrt(Interval(1, 4), alpha, dzeta, delta);

   Interval x = 0.3333333333 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = 0.70833333333 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.04166666666 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_log_chebyshev()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevLog(Interval(0.5, 4), alpha, dzeta, delta);

   Interval x = 0.5941261547 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = -0.7347733287 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.2554369291 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_exp_chebyshev()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevExp(Interval(-2, 3), alpha, dzeta, delta);

   Interval x = 3.9900403279 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = 3.29201656109 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 4.82339937811 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_cos_chebyshev_1()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevCos(Interval(0.2, 1.2), alpha, dzeta, delta);
   Interval x = -0.617708823365 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 1.15065088172 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0470425392035 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_cos_chebyshev_2()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevCos(Interval(12.7663706144, 13.7663706144), alpha, dzeta, delta);
   Interval x = -0.617708823365 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 8.91300888788 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0470425392035 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_cos_chebyshev_3()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevCos(Interval(1.7, 2.9), alpha, dzeta, delta);
   Interval x = -0.701761392378 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 1.00525463198 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0588952407714 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_cos_chebyshev_4()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevCos(Interval(14.2663706144, 15.4663706144), alpha, dzeta, delta);
   Interval x = -0.701761392378 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 9.82384837145 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0588952407714 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_cos_chebyshev_5()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevCos(Interval(-1.2, -0.2), alpha, dzeta, delta);
   Interval x = 0.617708823365 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 1.15065088172 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0470425392035 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_cos_chebyshev_6()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevCos(Interval(11.3663706144, 12.3663706144), alpha, dzeta, delta);
   Interval x = 0.617708823365 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = -6.61170712444 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0470425392035 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_cos_chebyshev_7()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevCos(Interval(-2.9, -1.7), alpha, dzeta, delta);
   Interval x = 0.701761392378 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 1.00525463198 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0588952407714 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_cos_chebyshev_8()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevCos(Interval(9.66637061436, 10.8663706144), alpha, dzeta, delta);
   Interval x = 0.701761392378 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = -7.8133391075 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0588952407714 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sin_chebyshev_1()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSin(Interval(-1.2, -0.2), alpha, dzeta, delta);
   Interval x = 0.733369755172 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = -0.091804448864 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0398090691034 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sin_chebyshev_2()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSin(Interval(11.3663706144, 12.3663706144), alpha, dzeta, delta);
   Interval x = 0.733369755172 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = -9.30760058972 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0398090691033 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sin_chebyshev_3()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSin(Interval(0.2, 1.2), alpha, dzeta, delta);
   Interval x = 0.733369755172 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 0.091804448864 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0398090691034 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sin_chebyshev_4()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSin(Interval(12.7663706144, 13.7663706144), alpha, dzeta, delta);
   Interval x = 0.733369755172 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = -9.12399169199 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0398090691033 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sin_chebyshev_5()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSin(Interval(1.7, 2.9), alpha, dzeta, delta);
   Interval x = -0.627012901032 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 2.12321991592 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0656331737162 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sin_chebyshev_6()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSin(Interval(14.2663706144, 15.4663706144), alpha, dzeta, delta);
   Interval x = -0.627012901032 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 10.0024964103 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.0656331737162 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sin_chebyshev_7()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSin(Interval(3.4, 4.6), alpha, dzeta, delta);
   Interval x = -0.615124918006 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 1.76930713658 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.066576482609 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sin_chebyshev_8()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSin(Interval(15.9663706144, 17.1663706144), alpha, dzeta, delta);
   Interval x = -0.615124918006 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));
   Interval y = 9.49919483037 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));
   Interval z = 0.066576482609 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_tan_chebyshev_1()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevTan(Interval(0.5, 1.25), alpha, dzeta, delta);

   Interval x = 3.284356245 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = -1.411890251 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.3160146181 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_tan_chebyshev_2()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevTan(Interval(3.64, 4.39), alpha, dzeta, delta);

   Interval x = 3.26585551 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = -11.6565703 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.3130925423 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_tan_chebyshev_3()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevTan(Interval(-2.64, -1.89), alpha, dzeta, delta);

   Interval x = 3.303056928 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = 8.949467649 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.3189749093 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_tan_chebyshev_4()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevTan(Interval(-1.25, -0.5), alpha, dzeta, delta);

   Interval x = 3.284356245 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = 1.411890251 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.3160146181 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_tan_chebyshev_5()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevTan(Interval(-4.39, -3.64), alpha, dzeta, delta);

   Interval x = 3.26585551 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = 11.6565703 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.3130925423 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_tan_chebyshev_6()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevTan(Interval(1.89, 2.64), alpha, dzeta, delta);

   Interval x = 3.303056928 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = -8.949467649 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.3189749093 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_tan_chebyshev_7()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevTan(Interval(9.92, 10.67), alpha, dzeta, delta);

   Interval x = 3.229441961 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = -31.80331004 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.3073604383 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_cosh_chebyshev_1()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevCosh(Interval(1, 3), alpha, dzeta, delta);

   Interval x = 4.26229068 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = -3.766277156 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 1.047067109 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_cosh_chebyshev_2()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevCosh(Interval(-3, -1), alpha, dzeta, delta);

   Interval x = 4.26229068 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = -3.766277156 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 1.047067109 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sinh_chebyshev_1()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSinh(Interval(1, 3), alpha, dzeta, delta);

   Interval x = 4.421336866 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = -4.259204578 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 1.013068904 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_sinh_chebyshev_2()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevSinh(Interval(-3, -1), alpha, dzeta, delta);

   Interval x = 4.421336866 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = 4.259204577 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 1.013068904 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_tanh_chebyshev_1()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevTanh(Interval(1, 3), alpha, dzeta, delta);

   Interval x = 0.1167302988 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = 0.6909905433 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.04612668624 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

void test_tanh_chebyshev_2()
{
   Interval alpha, dzeta, delta;
   AffineForm::chebyshevTanh(Interval(-3, -1), alpha, dzeta, delta);

   Interval x = 0.1167302988 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(x.contains(alpha));

   Interval y = -0.6909905433 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(y.contains(dzeta));

   Interval z = 0.04612668624 + Interval(-1.0e-8, 1.0e-8);
   TEST_TRUE(z.contains(delta));
}

int main()
{
   AffineForm::useMinrange();

   TEST(test_var)
   TEST(test_eval)

   TEST(test_push_1)
   TEST(test_push_2)
   TEST(test_push_3)
   TEST(test_push_4)
   TEST(test_push_5)
   TEST(test_push_6)

   TEST(test_add_1)
   TEST(test_add_2)
   TEST(test_add_3)
   TEST(test_add_4)

   TEST(test_sub_1)
   TEST(test_sub_2)
   TEST(test_sub_3)
   TEST(test_sub_4)
   TEST(test_usub)

   TEST(test_mul_1)
   TEST(test_mul_2)
   TEST(test_mul_3)
   TEST(test_mul_4)
   TEST(test_mul_5)

   TEST(test_sqr_1)
   TEST(test_sqr_2)
   TEST(test_sqr_3)

   TEST(test_udiv_1)
   TEST(test_udiv_2)
   TEST(test_udiv_3)
   TEST(test_udiv_4)
   TEST(test_udiv_5)

   TEST(test_pow)

   TEST(test_min_1)
   TEST(test_min_2)
   TEST(test_min_3)

   TEST(test_max_1)
   TEST(test_max_2)
   TEST(test_max_3)

   TEST(test_abs_1)
   TEST(test_abs_2)
   TEST(test_abs_3)

   TEST(test_sgn_1)
   TEST(test_sgn_2)
   TEST(test_sgn_3)

   TEST(test_sqrt_1)
   TEST(test_sqrt_2)

   TEST(test_exp)
   TEST(test_log)

   TEST(test_cos_1)
   TEST(test_cos_2)
   TEST(test_cos_3)

   TEST(test_sin_1)
   TEST(test_sin_2)
   TEST(test_sin_3)

   TEST(test_tan_1)
   TEST(test_tan_2)
   TEST(test_tan_3)

   TEST(test_cosh)
   TEST(test_sinh)
   TEST(test_tanh)

   AffineForm::useChebyshev();

   TEST(test_udiv_chebyshev_1)
   TEST(test_udiv_chebyshev_2)

   TEST(test_sqrt_chebyshev)

   TEST(test_log_chebyshev)

   TEST(test_exp_chebyshev)

   TEST(test_cos_chebyshev_1)
   TEST(test_cos_chebyshev_2)
   TEST(test_cos_chebyshev_3)
   TEST(test_cos_chebyshev_4)
   TEST(test_cos_chebyshev_5)
   TEST(test_cos_chebyshev_6)
   TEST(test_cos_chebyshev_7)
   TEST(test_cos_chebyshev_8)

   TEST(test_sin_chebyshev_1)
   TEST(test_sin_chebyshev_2)
   TEST(test_sin_chebyshev_3)
   TEST(test_sin_chebyshev_4)
   TEST(test_sin_chebyshev_5)
   TEST(test_sin_chebyshev_6)
   TEST(test_sin_chebyshev_7)
   TEST(test_sin_chebyshev_8)

   TEST(test_tan_chebyshev_1)
   TEST(test_tan_chebyshev_2)
   TEST(test_tan_chebyshev_3)
   TEST(test_tan_chebyshev_4)
   TEST(test_tan_chebyshev_5)
   TEST(test_tan_chebyshev_6)
   TEST(test_tan_chebyshev_7)

   TEST(test_cosh_chebyshev_1)
   TEST(test_cosh_chebyshev_1)

   TEST(test_sinh_chebyshev_1)
   TEST(test_sinh_chebyshev_2)

   TEST(test_tanh_chebyshev_1)
   TEST(test_tanh_chebyshev_2)

   END_TEST
}
