///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalGaussSeidel.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

IntervalGaussSeidel::IntervalGaussSeidel()
      : maxiter_(Param::GetIntParam("GAUSS_SEIDEL_ITER_LIMIT")),
        tol_(Param::GetDblParam("GAUSS_SEIDEL_REL_TOL"), 0.0)
{}

size_t IntervalGaussSeidel::getMaxIter() const
{
   return maxiter_;
}

void IntervalGaussSeidel::setMaxIter(size_t n)
{
   ASSERT(n > 0, "Bad parameter in the interval Gauss Seidel method");

   maxiter_ = n;
}

Tolerance IntervalGaussSeidel::getTol() const
{
   return tol_;
}

void IntervalGaussSeidel::setTol(const Tolerance& tol)
{
   tol_ = tol;
}

Proof IntervalGaussSeidel::contractPrecond(const IntervalMatrix& A,
                                           IntervalVector& x,
                                           const IntervalVector& b)
{
   int dim = A.ncols();
   RealMatrix M = A.midpoint();
   RealMatrix P(dim, dim);

   bool inv = M.inverse(P);
   if (!inv) return Proof::Maybe;

   return contract(P*A, x, P*b);
}

Proof IntervalGaussSeidel::contract(const IntervalMatrix& A, IntervalVector& x,
                                    const IntervalVector& b)
{
   ASSERT(A.nrows() == x.size(),
          "Bad interval linear system as input of the Gauss Seidel method");

   ASSERT(A.nrows() == b.size(),
          "Bad interval linear system as input of the Gauss Seidel method");

   ASSERT(A.nrows() == A.ncols(),
          "Bad interval linear system as input of the Gauss Seidel method");

   LOG_LOW("Interval Gauss-Seidel on A:\n" <<
           A << "\nx: " << x << "\nb: " << b);

   LOG_LOW("Tol: " << tol_);

   Proof proof = Proof::Maybe;

   bool iter = true;
   size_t nb_steps = 0;

   do
   {
      ++nb_steps;

      // inner step
      int res = innerStep(A, x, b);

      LOG_LOW("  step > " << ((res == 0) ? "empty " : "maybe ") << x);

      if (res == 0)
      {
         proof = Proof::Empty;
         iter = false;
      }
   
      else if (res == 1)
         iter = false;

      else if (nb_steps > maxiter_)
         iter = false;
   }
   while (iter);

   LOG_LOW("> " << proof << " " << x);

   return proof;
}

int IntervalGaussSeidel::innerStep(const IntervalMatrix& A, IntervalVector& x,
                                   const IntervalVector& b)
{
   int res = 1;

   for (size_t i=0; i<x.size(); ++i)
   {
      if (A.get(i, i).containsZero()) continue;

      // inversion of the i-th row: sum{ij} A{ij} x{j} = b{i}
      // I := b{i} - sum{ij, j!=i} A{ij} x{j}
      Interval I = b.get(i);

      for (size_t j=0; j<i; ++j)
         I -= A.get(i, j)*x.get(j);

      for (size_t j=i+1; j<x.size(); ++j)
         I -= A.get(i, j)*x.get(j);
     
      // projection of I = x{i}*A{i,i} onto x{i}
      Interval z = mulPX(x.get(i), A.get(i, i), I);

      if (z.isEmpty()) return 0;
      else
      {
         if (tol_.isImproved(x.get(i), z))
            res = 2;    // contraction large enough to iterate

         x.set(i, z);
      }
   }

   return res;
}

} // namespace
