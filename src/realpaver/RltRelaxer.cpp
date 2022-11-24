///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/RltRelaxer.hpp"

namespace realpaver {

RltRelaxer::RltRelaxer(SharedDag dag) : dag_(dag), mpi_()
{
   ASSERT(dag != nullptr && dag->nbNodes() > 0,
          "No dag given as input of a RLT relaxer");
}

bool RltRelaxer::make(LPModel& lpm, const IntervalRegion& reg)
{

   if (!dag_->intervalEval(reg)) return false;

   for (size_t i=0; i<dag_->nbNodes(); ++i)
   {
      DagNode* node = dag_->node(i);
      Interval val = node->val();

      // creates a linear variable for this node
      LinVar v = lpm.makeVar(val.left(), val.right());
      mpi_.insert(std::make_pair(node->index(), v.getIndex()));

      // constrains this variable
      RltVisitor vis(&lpm, &mpi_);
      node->acceptVisitor(vis);
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

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
   if (node->getVar().isDiscrete())
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

} // namespace
