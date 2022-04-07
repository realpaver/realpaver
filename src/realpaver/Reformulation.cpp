///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Double.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Reformulation.hpp"

namespace realpaver {

void underConvex(LPModel& lm, size_t iy, size_t ix,
                 double a, double b, double c,
                 std::function<Interval(Interval)> f, 
                 std::function<Interval(Interval)> df)
{
   ASSERT(a <= c && c <= b,
          "Bad values for the relaxation of a convex function");

   if (a == b) return;

   LinVar x = lm.getLinVar(ix),
          y = lm.getLinVar(iy);

   Interval C(c), M(df(C));

   // if the slope is null then f has a minimum at x=c and the domain of
   // of y must be already constrained
   if (M.containsZero()) return;

   // calculates the ordinate at the origin
   Interval P(f(C) - M*C);
   double p = P.left();

   // calculates the slope
   double m;
   if (c < 0.0 || c == b)
   {
      m = M.right();
   }
   else if (c > 0.0 || c == a)
   {
      m = M.left();
   }
   else
   {
      // c = 0 and a < c < b
      m = M.left();

      // deviation between f and the line at x=a
      Interval da = f(a) - Interval(m)*a - p;

      // deviation between f and the line at x=b
      Interval db = f(b) - Interval(m)*b - p;

      // deviation
      double d = std::min(0.0, std::min(da.left(), db.left()));
      Double::rndDn();
      p += d;
   }

   // y - mx >= p
   LinExpr e( {1.0, -m}, {y, x} );
   lm.addCtr(p, e);
}

void overConvex(LPModel& lm, size_t iy, size_t ix,
                double a, double b,
                std::function<Interval(Interval)> f)
{
   if (a == b) return;

   LinVar x = lm.getLinVar(ix),
          y = lm.getLinVar(iy);

   Interval fa = f(a),
            fb = f(b);

   // slope null?
   if (fa.overlaps(fb)) return;

   Interval M = (fb - fa) / (Interval(a,b).width()),
            P = fa - M*a;
   double p = P.right();

   //calculates the slope
   double m;

   if (a > 0.0)
   {
      m = M.right();
   }
   else if (b < 0.0)
   {
      m = M.left();
   }
   else
   {
      // a < 0 < b
      m = M.left();

      // deviation between f and the line at x=a
      Interval da = Interval(m)*a + p - f(a);

      // deviation between f and the line at x=b
      Interval db = Interval(m)*b + p - f(b);

      // deviation
      double d = std::min(0.0, std::min(da.left(), db.left()));
      Double::rndUp();
      p -= d;
   }

   // y - mx <= p
   LinExpr e( {1.0, -m}, {y, x} );
   lm.addCtr(e, p);
}

void overConcave(LPModel& lm, size_t iy, size_t ix,
                 double a, double b, double c,
                 std::function<Interval(Interval)> f, 
                 std::function<Interval(Interval)> df)
{
   ASSERT(a <= c && c <= b,
          "Bad values for the relaxation of a concave function");

   if (a == b) return;

   LinVar x = lm.getLinVar(ix),
          y = lm.getLinVar(iy);

   Interval C(c), M(df(C));

   // if the slope is null then f has a maximum at x=c and the domain of
   // of y must be already constrained
   if (M.containsZero()) return;

   // calculates the ordinate at the origin
   Interval P(f(C) - M*C);
   double p = P.right();

   // calculates the slope
   double m;
   if (c < 0.0 || c == b)
   {
      m = M.left();
   }
   else if (c > 0.0 || c == a)
   {
      m = M.right();
   }
   else
   {
      // c = 0 and a < c < b
      m = M.left();

      // deviation between f and the line at x=a
      Interval da = f(a) - Interval(m)*a - p;

      // deviation between f and the line at x=b
      Interval db = f(b) - Interval(m)*b - p;

      // deviation
      double d = std::min(0.0, std::min(da.left(), db.left()));
      Double::rndUp();
      p -= d;
   }

   // y - mx <= p
   LinExpr e( {1.0, -m}, {y, x} );
   lm.addCtr(e, p);
}

void underConcave(LPModel& lm, size_t iy, size_t ix,
                  double a, double b,
                  std::function<Interval(Interval)> f)
{
   if (a == b) return;

   LinVar x = lm.getLinVar(ix),
          y = lm.getLinVar(iy);

   Interval fa = f(a),
            fb = f(b);

   // slope null?
   if (fa.overlaps(fb)) return;

   Interval M = (fb - fa) / (Interval(a,b).width()),
            P = fa - M*a;
   double p = P.left();

   //calculates the slope
   double m;

   if (a > 0.0)
   {
      m = M.left();
   }
   else if (b < 0.0)
   {
      m = M.right();
   }
   else
   {
      // a < 0 < b
      m = M.left();

      // deviation between f and the line at x=a
      Interval da = Interval(m)*a + p - f(a);

      // deviation between f and the line at x=b
      Interval db = Interval(m)*b + p - f(b);

      // deviation
      double d = std::min(0.0, std::min(da.left(), db.left()));
      Double::rndDn();
      p += d;
   }

   // y - mx >= p
   LinExpr e( {1.0, -m}, {y, x} );
   lm.addCtr(p, e);
}

void relaxConcavoConvexCosSin(LPModel& lm, size_t iy, size_t ix,
                              double a, double b,
                              std::function<Interval(Interval)> f,
                              std::function<Interval(Interval)> df)
{
   LinVar y = lm.getLinVar(iy),
          x = lm.getLinVar(ix);

   Interval fa(f(a)), fb(f(b));

   // slope positive with maximum slope = 1 at y=0?
   if (fa.isNegative())
   {
      // overestimation: y <= x + p passing through (a, f(a))
      Interval p1 = fa - a;
      LinExpr e1( {1.0, -1.0}, {y, x} );
      lm.addCtr(e1, p1.right());

      // underestimation: y >= x + p passing through (b, f(b))
      Interval p2 = fb - b;
      LinExpr e2( {1.0, -1.0}, {y, x} );
      lm.addCtr(p2.left(), e2);
   }

   // slope negative with minimum slope = -1 at y=0?
   else
   {
      // underestimation: y >= -x + p passing through (a, f(a))
      Interval p1 = fa + a;
      LinExpr e1( {1.0, 1.0}, {y, x} );
      lm.addCtr(p1.left(), e1);

      // overestimation: y <= -x + p passing through (b, f(b))
      Interval p2 = fb + b;
      LinExpr e2( {1.0, 1.0}, {y, x} );
      lm.addCtr(e2, p2.right());
   }
}

} // namespace
