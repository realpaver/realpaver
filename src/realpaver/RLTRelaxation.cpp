///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/RltRelaxation.hpp"

namespace realpaver {

RltVisitor::RltVisitor(LPModel* lpm, std::unordered_map<size_t, size_t>* mpi)
       : lpm_(lpm),
         mpi_(mpi)
{}

size_t RltVisitor::indexLinVar(const DagNode* node) const
{
   auto it = mpi_->find(node->index());
   return it->second;
}

void RltVisitor::apply(const DagConst* node)
{
   // nothing to do
}

void RltVisitor::apply(const DagVar* node)
{
   if (node->getVar().isInteger())
   {
      LinVar lv = lpm_->getLinVar(indexLinVar(node));
      lv.setInteger();
   }
}

void RltVisitor::apply(const DagAdd* node)
{
   LPModel& lm = *lpm_;

   LinVar z = lm.getLinVar(indexLinVar(node)),
          x = lm.getLinVar(indexLinVar(node->left())),
          y = lm.getLinVar(indexLinVar(node->right()));

   // z = x + y => z - x - y = 0
   LinExpr e( {1.0, -1.0, -1.0}, {z, x, y} );
   lm.addCtr(0.0, e, 0.0);
}

void RltVisitor::apply(const DagSub* node)
{
   LPModel& lm = *lpm_;

   LinVar z = lm.getLinVar(indexLinVar(node)),
          x = lm.getLinVar(indexLinVar(node->left())),
          y = lm.getLinVar(indexLinVar(node->right()));

   // z = x - y => z - x + y = 0
   LinExpr e( {1.0, -1.0, 1.0}, {z, x, y} );
   lm.addCtr(0.0, e, 0.0);
}

void RltVisitor::apply(const DagMul* node)
{
   LPModel& lm = *lpm_;

   LinVar z = lm.getLinVar(indexLinVar(node)),
          x = lm.getLinVar(indexLinVar(node->left())),
          y = lm.getLinVar(indexLinVar(node->right()));

   // z = x*y, a <= x <= b, c <= y <= d
   double a = node->left()->val().left(),
          b = node->left()->val().right(),
          c = node->right()->val().left(),
          d = node->right()->val().right();

   bool xvar = (a != b),   // left subterm not fixed?
        yvar = (c != d);   // right subterm not fixed?

   if (xvar && yvar)
   {
      // Mc Cormick relaxation
      Interval A(a), B(b), C(c), D(d);

      // first constraint: (x-a)*(y-c) >= 0, z - c*x - a*y >= -a*c
      Interval I1 = -A*C;
      LinExpr e1( {1.0, -c, -a}, {z, x, y} );
      lm.addCtr(I1.left(), e1);

      // second constraint: (x-a)*(y-d) <= 0, z - d*x - a*y <= -a*d
      Interval I2 = -A*D;
      LinExpr e2( {1.0, -d, -a}, {z, x, y} );
      lm.addCtr(e2, I2.right());

      // third constraint: (x-b)*(y-c) <= 0, z - c*x - b*y <= -b*c
      Interval I3 = -B*C;
      LinExpr e3( {1.0, -c, -b}, {z, x, y} );
      lm.addCtr(e3, I3.right());

      // fourth constraint: (x-b)*(y-d) >= 0, z - d*x - b*y >= -b*d
      Interval I4 = -B*D;
      LinExpr e4( {1.0, -d, -b}, {z, x, y} );
      lm.addCtr(I4.left(), e4);
   }
   else if (!xvar)
   {
      // z = x*y with x fixed => z - a*y = 0
      LinExpr e( {1.0, -a}, {z, y} );
      lm.addCtr(0.0, e, 0.0);
   }
   else if (!yvar)
   {
      // z = x*y with y fixed => z - c*x = 0
      LinExpr e( {1.0, -c}, {z, x} );
      lm.addCtr(0.0, e, 0.0);      
   }
}

void RltVisitor::apply(const DagDiv* node)
{
   LPModel& lm = *lpm_;

   LinVar z = lm.getLinVar(indexLinVar(node)),
          x = lm.getLinVar(indexLinVar(node->left())),
          y = lm.getLinVar(indexLinVar(node->right()));

   // z = x/y, a <= x <= b, c <= y <= d
   double a = node->left()->val().left(),
          b = node->left()->val().right(),
          c = node->right()->val().left(),
          d = node->right()->val().right(),
          u = node->val().left(),
          v = node->val().right();

   bool xvar = (a != b),   // left subterm not fixed?
        yvar = (c != d);   // right subterm not fixed?

   if (yvar)
   {
      // Mc Cormick relaxation on x = y*z, c <= y <= d, u <= z <= v
      Interval C(c), D(d), U(u), V(v);

      // first constraint: (y-c)*(z-u) >= 0, x -cz -uy >= -cu
      Interval I1 = -C*U;
      LinExpr e1( {1.0, -c, -u}, {x, z, y} );
      lm.addCtr(I1.left(), e1);

      // second constraint: (y-c)*(z-v) <= 0, x -cz -vy <= -cv
      Interval I2 = -C*V;
      LinExpr e2( {1.0, -c, -v}, {x, z, y} );
      lm.addCtr(e2, I2.right());

      // third constraint: (y-d)*(z-u) <= 0, x -dz -uy <= -du
      Interval I3 = -D*U;
      LinExpr e3( {1.0, -d, -u}, {x, z, y} );
      lm.addCtr(e3, I3.right());

      // fourth constraint: (y-d)*(z-v) >= 0, x -dz -vy >= -dv
      Interval I4 = -D*V;
      LinExpr e4( {1.0, -d, -v}, {x, z, y} );
      lm.addCtr(I4.left(), e4);
   }
   else if (xvar)
   {
      // y fixed => x = y*z with y = c => x -cz = 0
      LinExpr e( {1.0, -c}, {x, z} );
      lm.addCtr(0.0, e, 0.0);      
   }
}

void RltVisitor::apply(const DagMin* node)
{
   LPModel& lm = *lpm_;

   LinVar z = lm.getLinVar(indexLinVar(node)),
          x = lm.getLinVar(indexLinVar(node->left())),
          y = lm.getLinVar(indexLinVar(node->right()));

   // z = min(x,y), a <= x <= b, c <= y <= d
   double a = node->left()->val().left(),
          b = node->left()->val().right(),
          c = node->right()->val().left(),
          d = node->right()->val().right();

   if (b < c)
   {
      // z = x => z - x = 0
      LinExpr e( {1.0, -1.0}, {z, x} );
      lm.addCtr(0.0, e, 0.0);
   }
   else if (d < a)
   {
      // z = y => z - y = 0
      LinExpr f( {1.0, -1.0}, {z, y} );
      lm.addCtr(0.0, f, 0.0);
   }
   else
   {
      // z <= x => z - x <= 0
      LinExpr e( {1.0, -1.0}, {z, x} );
      lm.addCtr(e, 0.0);

      // z <= y => z - y <= 0
      LinExpr f( {1.0, -1.0}, {z, y} );
      lm.addCtr(f, 0.0);
   }
}

void RltVisitor::apply(const DagMax* node)
{
   LPModel& lm = *lpm_;

   LinVar z = lm.getLinVar(indexLinVar(node)),
          x = lm.getLinVar(indexLinVar(node->left())),
          y = lm.getLinVar(indexLinVar(node->right()));

   // z = min(x,y), a <= x <= b, c <= y <= d
   double a = node->left()->val().left(),
          b = node->left()->val().right(),
          c = node->right()->val().left(),
          d = node->right()->val().right();

   if (d < a)
   {
      // z = x => z - x = 0
      LinExpr e( {1.0, -1.0}, {z, x} );
      lm.addCtr(0.0, e, 0.0);
   }
   else if (b < c)
   {
      // z = y => z - y = 0
      LinExpr f( {1.0, -1.0}, {z, y} );
      lm.addCtr(0.0, f, 0.0);
   }
   else
   {
      // z >= x => z - x >= 0
      LinExpr e( {1.0, -1.0}, {z, x} );
      lm.addCtr(0.0, e);
      
      // z >= y => z - y >= 0
      LinExpr f( {1.0, -1.0}, {z, y} );
      lm.addCtr(0.0, f);
   }
}

void RltVisitor::apply(const DagUsb* node)
{
   LPModel& lm = *lpm_;

   LinVar y = lm.getLinVar(indexLinVar(node)),
          x = lm.getLinVar(indexLinVar(node->child()));

   // y = -x => y + x = 0
   LinExpr e( {1.0, 1.0}, {y, x} );
   lm.addCtr(0.0, e, 0.0);
}

void RltVisitor::apply(const DagAbs* node)
{
   LPModel& lm = *lpm_;

   size_t iy = indexLinVar(node),
          ix = indexLinVar(node->child());

   LinVar y = lm.getLinVar(iy),
          x = lm.getLinVar(ix);

   double a = node->child()->val().left(),
          b = node->child()->val().right();

   if (a >= 0.0)
   {
      // y = x => y - x = 0
      LinExpr e( {1.0, -1.0}, {y, x} );
      lm.addCtr(0.0, e, 0.0);
   }
   else if (b <= 0.0)
   {
      // y = -x => y + x = 0
      LinExpr e( {1.0, 1.0}, {y, x} );
      lm.addCtr(0.0, e, 0.0);
   }
   else
   {
      // underestimation: y >= x <=> y - x >= 0
      LinExpr e1( {1.0, -1.0}, {y, x} );
      lm.addCtr(0.0, e1);

      // underestimation: y >= -x <=> y + x >= 0
      LinExpr e2( {1.0, 1.0}, {y, x} );
      lm.addCtr(0.0, e2);

      // overestimation
      auto f  = [](const Interval& x) { return abs(x); };
      overConvex(lm, iy, ix, a, b, f);
   }
}

void RltVisitor::apply(const DagSgn* node)
{
   // nothing to do
}

void RltVisitor::apply(const DagSqr* node)
{
   LPModel& lm = *lpm_;

   size_t iy = indexLinVar(node),
          ix = indexLinVar(node->child());

   double a = node->child()->val().left(),
          b = node->child()->val().right();

   auto f  = [](const Interval& x) { return sqr(x); };
   auto df = [](const Interval& x) { return 2.0*x; };

   underConvex(lm, iy, ix, a, b, a, f, df);
   underConvex(lm, iy, ix, a, b, b, f, df);
   underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

   overConvex(lm, iy, ix, a, b, f);
}

void RltVisitor::apply(const DagSqrt* node)
{
   LPModel& lm = *lpm_;

   size_t iy = indexLinVar(node),
          ix = indexLinVar(node->child());

   double a = node->child()->val().left(),
          b = node->child()->val().right();

   if (a < 0.0) return;

   auto f  = [](const Interval& x) { return sqrt(x); };
   auto df = [](const Interval& x) { return 1.0/(2.0*sqrt(x)); };

   if (a > 0.0)
      overConcave(lm, iy, ix, a, b, a, f, df);

   overConcave(lm, iy, ix, a, b, b, f, df);

   double c = Interval(a, b).midpoint();
   if (c > 0.0)
      overConcave(lm, iy, ix, a, b, c, f, df);

   underConcave(lm, iy, ix, a, b, f);
}

void RltVisitor::apply(const DagPow* node)
{
   LPModel& lm = *lpm_;

   size_t iy = indexLinVar(node),
          ix = indexLinVar(node->child());

   double a = node->child()->val().left(),
          b = node->child()->val().right();

   int n = node->exponent();

   auto f  = [&n](const Interval& x) { return pow(x, n); };
   auto df = [&n](const Interval& x) { return n*pow(x, n-1); };

   if (n % 2 == 0 || a >= 0.0)
   {
      // convex function
      underConvex(lm, iy, ix, a, b, a, f, df);
      underConvex(lm, iy, ix, a, b, b, f, df);
      underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      overConvex(lm, iy, ix, a, b, f);
   }
   else
   {
      if (b <= 0.0)
      {
         // odd power, concave function
         overConcave(lm, iy, ix, a, b, a, f, df);
         overConcave(lm, iy, ix, a, b, b, f, df);
         overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

         underConcave(lm, iy, ix, a, b, f);         
      }
      else
      {
         // odd power, concave over [a, 0] and convex in [0, b]
         LinVar y = lm.getLinVar(iy),
                x = lm.getLinVar(ix);

         // TODO
         // best way, not implemented: find a tangent at point c in [0, b]
         // (c unknown) passing through (a, f(a)); find another tangent
         // at point c' in [a, 0] (c unknown) passing through (b, f(b))

         // underestimation
         underLine(lm, iy, ix, a, f(a).left(), b, 0.0);

         // overestimation
         overLine(lm, iy, ix, a, 0.0, b, f(b).right());
      }
   }
}

void RltVisitor::apply(const DagExp* node)
{
   LPModel& lm = *lpm_;

   size_t iy = indexLinVar(node),
          ix = indexLinVar(node->child());

   double a = node->child()->val().left(),
          b = node->child()->val().right();

   auto f  = [](const Interval& x) { return exp(x); };
   auto df = [](const Interval& x) { return exp(x); };

   underConvex(lm, iy, ix, a, b, a, f, df);
   underConvex(lm, iy, ix, a, b, b, f, df);
   underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

   overConvex(lm, iy, ix, a, b, f);
}

void RltVisitor::apply(const DagLog* node)
{
   LPModel& lm = *lpm_;

   if (node->val().isInf()) return;

   size_t iy = indexLinVar(node),
          ix = indexLinVar(node->child());

   double a = node->child()->val().left(),
          b = node->child()->val().right();

   auto f  = [](const Interval& x) { return log(x); };
   auto df = [](const Interval& x) { return 1.0/x; };

   overConcave(lm, iy, ix, a, b, a, f, df);
   overConcave(lm, iy, ix, a, b, b, f, df);
   overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

   underConcave(lm, iy, ix, a, b, f);
}

void RltVisitor::apply(const DagCos* node)
{
   LPModel& lm = *lpm_;
   Interval val = node->val();

   size_t iy = indexLinVar(node),
          ix = indexLinVar(node->child());

   double a = node->child()->val().left(),
          b = node->child()->val().right();

   auto f  = [](const Interval& x) { return cos(x); };
   auto df = [](const Interval& x) { return -sin(x); };

   if (val.isPositive())
   {
      // concave function
      overConcave(lm, iy, ix, a, b, a, f, df);
      overConcave(lm, iy, ix, a, b, b, f, df);
      overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      underConcave(lm, iy, ix, a, b, f);      
   }
   else if (val.isNegative())
   {
      // convex function
      underConvex(lm, iy, ix, a, b, a, f, df);
      underConvex(lm, iy, ix, a, b, b, f, df);
      underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      overConvex(lm, iy, ix, a, b, f);
   }
   else if (Interval::minusOnePlusOne().strictlyContains(val))
   {
      // concavo-convex function
      relaxConcavoConvexCosSin(lm, iy, ix, a, b, f, df);
   }
   // else there is a stationaty point => no relaxation
}

void RltVisitor::apply(const DagSin* node)
{
   LPModel& lm = *lpm_;
   Interval val = node->val();

   size_t iy = indexLinVar(node),
          ix = indexLinVar(node->child());

   double a = node->child()->val().left(),
          b = node->child()->val().right();

   auto f  = [](const Interval& x) { return sin(x); };
   auto df = [](const Interval& x) { return cos(x); };

   if (val.isPositive())
   {
      // concave function
      overConcave(lm, iy, ix, a, b, a, f, df);
      overConcave(lm, iy, ix, a, b, b, f, df);
      overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      underConcave(lm, iy, ix, a, b, f);      
   }
   else if (val.isNegative())
   {
      // convex function
      underConvex(lm, iy, ix, a, b, a, f, df);
      underConvex(lm, iy, ix, a, b, b, f, df);
      underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      overConvex(lm, iy, ix, a, b, f);
   }
   else if (Interval::minusOnePlusOne().strictlyContains(val))
   {
      // concavo-convex function
      relaxConcavoConvexCosSin(lm, iy, ix, a, b, f, df);
   }
   // else there is a stationary point => no relaxation
}

void RltVisitor::apply(const DagTan* node)
{
   LPModel& lm = *lpm_;
   Interval val = node->val();

   if (val.isInf()) return;

   size_t iy = indexLinVar(node),
          ix = indexLinVar(node->child());

   double a = node->child()->val().left(),
          b = node->child()->val().right();

   auto f  = [](const Interval& x) { return tan(x); };
   auto df = [](const Interval& x) { return 1.0/sqr(cos(x)); };

   if (val.isNegative())
   {
      // convex function
      underConvex(lm, iy, ix, a, b, a, f, df);
      underConvex(lm, iy, ix, a, b, b, f, df);
      underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      overConvex(lm, iy, ix, a, b, f);
   }
   else if (val.isPositive())
   {
      // concave function
      overConcave(lm, iy, ix, a, b, a, f, df);
      overConcave(lm, iy, ix, a, b, b, f, df);
      overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      underConcave(lm, iy, ix, a, b, f);         
   }
   else
   {
      // concavo-convex function
      LinVar x = lm.getLinVar(ix),
             y = lm.getLinVar(iy);

      // underestimation: under the line passing through
      // (b, tan(b)) with slope 1, i.e. y <= x + p
      Interval p1(node->val().right() - Interval(b));
      LinExpr e1( {1.0, -1.0}, {y, x} );
      lm.addCtr(e1, p1.right());

      // overestimation: over the line passing through
      // (a, tan(a)) with slope 1, i.e. y >= x + p
      Interval p2(node->val().left() - Interval(a));
      LinExpr e2( {1.0, -1.0}, {y, x} );
      lm.addCtr(p2.left(), e2);
   }
}

///////////////////////////////////////////////////////////////////////////////

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

   return overLine(lm, iy, ix, a, fa.right(), b, fb.right());
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
   Interval fa = f(a),
            fb = f(b);

   return underLine(lm, iy, ix, a, fa.left(), b, fb.left());
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

void overLine(LPModel& lm, size_t iy, size_t ix,
              double x1, double y1, double x2, double y2)
{
   if (x1 == x2) return;

   if (x2 < x1)
   {
      std::swap(x1, x2);
      std::swap(y1, y2);
   }

   LinVar x = lm.getLinVar(ix),
          y = lm.getLinVar(iy);

   // slope
   Interval M((Interval(y2) - y1) / (Interval(x2) - x1));

   if (M.containsZero())
   {
      LinExpr e( {1.0}, {y} );
      lm.addCtr(e, std::max(y1, y2));
   }
   else
   {
      Interval P(y1 - M*x1);
      double p = P.right(), m;

      if (x2 <= 0.0)
      {
         m = M.left();
      }
      else if (x1 >= 0.0)
      {
         m = M.right();         
      }
      else
      {
         m = M.left();

         Interval v1(y1 - Interval(m)*x1 - P),
                  v2(y2 - Interval(m)*x2 - P);

         double d = std::max(v1.right(), v2.right());

         if (d > 0.0)
         {
            Double::rndUp();
            p += d;
         }
      }

      // y <= mx + p
      LinExpr e( {1.0, -m}, {y, x} );
      lm.addCtr(e, p);
   }
}

void underLine(LPModel& lm, size_t iy, size_t ix,
              double x1, double y1, double x2, double y2)
{
   if (x1 == x2) return;

   if (x2 < x1)
   {
      std::swap(x1, x2);
      std::swap(y1, y2);
   }

   LinVar x = lm.getLinVar(ix),
          y = lm.getLinVar(iy);

   // slope
   Interval M((Interval(y2) - y1) / (Interval(x2) - x1));

   if (M.containsZero())
   {
      LinExpr e( {1.0}, {y} );
      lm.addCtr(std::min(y1, y2), e);
   }
   else
   {
      Interval P(y1 - M*x1);
      double p = P.left(), m;

      if (x2 <= 0.0)
      {
         m = M.right();
      }
      else if (x1 >= 0.0)
      {
         m = M.left();         
      }
      else
      {
         m = M.left();

         Interval v1(Interval(m)*x1 + P - y1),
                  v2(Interval(m)*x2 + P - y2);

         double d = std::max(v1.right(), v2.right());

         if (d > 0.0)
         {
            Double::rndDn();
            p -= d;
         }
      }

      // y >= mx + p
      LinExpr e( {1.0, -m}, {y, x} );
      lm.addCtr(p, e);         
   }
}

} // namespace
