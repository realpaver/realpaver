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

// underestimation y >= mx + p of a convex function f tangent at x=c
// slope m = df(c)
// ordinate at the origin p = f(c) - mc
void underConvex(LPModel& lm, size_t iy, size_t ix,
                 double a, double b, double c, double tol,
                 std::function<Interval(Interval)> f, 
                 std::function<Interval(Interval)> df)
{
   ASSERT(a <= c && c <= b,
          "Bad values for the underestimation of a convex function");

   if (a == b) return;

   LinVar x = lm.getLinVar(ix),
          y = lm.getLinVar(iy);

   Interval C(c), M(df(C));

   if (c == 0.0 || M.containsZero())
   {
      LOG_FULL("Perturbed approximation point for the underestimation of a "
               << "convex function");

      // perturbation
      c = (c-a > b-c) ? c - tol : c + tol;;
      C = c;
      M = df(C);

      if (c < a || c > b || c == 0.0 || M.containsZero())
      {
         LOG_FULL("Generation of an underestimation of a convex function "
                  << "impossible in " << Interval(a, b));
         return;
      }
   }

   Interval P(f(C) - M*C);
   double m = (c > 0.0) ? M.left() : M.right();

   LinExpr e( {1.0, -m}, {y, x} );
   lm.addCtr(P.left(), e);
}

} // namespace
