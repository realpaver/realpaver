/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   ContractorPolytope.cpp
 * @brief  Polytope Hull contractor
 * @author Laurent Granvilliers
 * @date   2025-fev-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorPolytope.hpp"
#include "realpaver/LPModel.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

ContractorPolytope::ContractorPolytope(std::unique_ptr<Linearizer> lzr)
    : Contractor()
    , lzr_(std::move(lzr))
    , maxseconds_(Params::GetDblParam("LP_TIME_LIMIT"))
    , maxiter_(Params::GetIntParam("LP_ITER_LIMIT"))
    , feastol_(Params::GetDblParam("LP_FEAS_TOL"))
    , loop_(Params::GetStrParam("POLYTOPE_HULL_LOOP") == "YES")
    , looptol_(Params::GetDblParam("POLYTOPE_HULL_LOOP_TOL"))
{
   ASSERT((lzr_ != nullptr), "Empty linearizer in a polytope contractor");
}

double ContractorPolytope::getRelaxTol() const
{
   return lzr_->getRelaxTol();
}

void ContractorPolytope::setRelaxTol(double tol)
{
   ASSERT(tol >= 0.0, "The relaxation tolerance must be positive: " << tol);

   lzr_->setRelaxTol(tol);
}

Scope ContractorPolytope::scope() const
{
   return lzr_->scope();
}

void ContractorPolytope::setMaxSeconds(double s)
{
   maxseconds_ = s;
}

double ContractorPolytope::getMaxSeconds() const
{
   return maxseconds_;
}

void ContractorPolytope::setMaxIter(size_t n)
{
   maxiter_ = n;
}

size_t ContractorPolytope::getMaxIter() const
{
   return maxiter_;
}

double ContractorPolytope::getFeasTol() const
{
   return feastol_;
}

void ContractorPolytope::setFeasTol(double tol)
{
   ASSERT(tol > 0.0, "Bad tolerance in the polytope hull contractor");

   feastol_ = tol;
}

void ContractorPolytope::enforceLoop(bool loop, double tol)
{
   ASSERT(tol >= 0.0 && tol <= 1.0, "A relative tolerance must belong to [0, 1]");
   loop_ = loop;
   looptol_ = tol;
}

Proof ContractorPolytope::contract(IntervalBox &B)
{
   if (!loop_)
      return contractImpl(B);

   bool iter = true;
   IntervalBox prev(B);
   while (iter)
   {
      if (contractImpl(B) == Proof::Empty)
         return Proof::Empty;
      if (!B.improves(prev, looptol_))
         iter = false;
      else
         prev.setOnScope(B, scope());
   }
   return Proof::Maybe;
}

Proof ContractorPolytope::contractImpl(IntervalBox &B)
{
   LOG_INTER("Polytope hull contractor " << B);

   // defines and tunes the LP solver
   LPSolver solver;
   tuneLPSolver(solver);

   // linearizes the constraints
   bool ok = lzr_->make(solver, B);
   if (!ok)
      return Proof::Maybe;

   LOG_LOW("Relaxation " << solver);

   // creates additional structures
   int nv = scope().size(),
       nlb = nv - 1,               // number of left bounds
       nrb = nv,                   // number of right bounds
           *lb = new int[nlb],     // array of indexes of variables (left bounds)
               *rb = new int[nrb], // array of indexes of variables (right bounds)
       iv = 0;                     // current variable t be handled
   LPSense sense = LPSense::Min;   // current optimization sense

   // lb: 1, 2,, ..., n-1 (no need to record the left bound of the first var)
   for (int i = 0; i < nlb; ++i)
      lb[i] = i + 1;

   // rb: 0, 1, 2, ..., n-1 (right bounds of all the variables)
   for (int i = 0; i < nrb; ++i)
      rb[i] = i;

   Proof proof = run(solver, B, lb, nlb, rb, nrb, iv, sense);

   delete[] lb;
   delete[] rb;

   return proof;
}

Proof ContractorPolytope::run(LPSolver &solver, IntervalBox &B, int *lb, int &nlb,
                              int *rb, int &nrb, int &iv, LPSense &sense)
{
   Proof proof = Proof::Maybe;
   LPStatus status = LPStatus::Other;
   while (iv >= 0)
   {
      // changes the cost
      Variable v = scope().var(iv);
      LinVar lv = solver.getLinVar(lzr_->linVarIndex(v));
      LinExpr e({1.0}, {lv});
      solver.setCost(e);
      solver.setSense(sense);

      LOG_LOW("LP solved: " << sense << " " << v.getName());

      // optimization
      status = (status != LPStatus::Optimal) ? solver.optimize() : solver.reoptimize();

      LOG_LOW("Status : " << status);

      switch (status)
      {
      case LPStatus::Optimal: {
         double bnd = solver.certifiedCostSolution();
         LOG_LOW("Certified cost: " << bnd);
         LOG_LOW("Primal: " << solver.primalSolution());

         Interval x = B.get(v);
         if (sense == LPSense::Min)
         {
            if (bnd > x.right())
            {
               iv = -1;
               proof = Proof::Empty;
               LOG_LOW("Empty box");
            }
            else if (bnd > x.left())
            {
               Interval dom(bnd, x.right());
               B.set(v, dom);
               lv.setLB(bnd);
               LOG_LOW("New domain: " << dom);
            }
         }
         else
         {
            if (bnd < x.left())
            {
               iv = -1;
               proof = Proof::Empty;
               LOG_LOW("Empty box => stop");
            }
            else if (bnd < x.right())
            {
               Interval dom(x.left(), bnd);
               B.set(v, dom);
               lv.setUB(bnd);
               LOG_LOW("New domain: " << dom);
            }
         }
         break;
      }
      case LPStatus::Infeasible: {
         iv = -1; // infeasible (proved or not) => stop
         if (solver.isCertifiedInfeasible())
         {
            proof = Proof::Empty;
            LOG_LOW("Proved infeasible")
         }
         break;
      }
      case LPStatus::StopOnIterLimit:
      case LPStatus::StopOnTimeLimit: {
         iv = -1; // too expensive => stop
         break;
      }
      default: {
         // other status => select the next variable
         break;
      }
      }

      if (iv >= 0)
      {
         if (status == LPStatus::Optimal)
            selectAchterberg(solver, B, lb, nlb, rb, nrb, iv, sense);

         else
            selectNext(lb, nlb, rb, nrb, iv, sense);
      }
   }

#if LOG_ON
   LOG_INTER("Proof: " << proof);
   if (proof != Proof::Empty)
      LOG_INTER("New box: " << B);
#endif

   return proof;
}

void ContractorPolytope::selectAchterberg(LPSolver &solver, IntervalBox &B, int *lb,
                                          int &nlb, int *rb, int &nrb, int &iv,
                                          LPSense &sense)
{
   LOG_LOW("Achterberg's heuristics");

   if (nlb + nrb == 0)
   {
      iv = -1;
      return;
   }

   // Achterberg's heuristics. Each bound that is too close to the current
   // primal solution is discarded. Among the other bounds, the one that is
   // the clothest to the primal solution is selected., which aims at
   // minimizing the number of iterations of the Simplex algorithm.
   double dist = Double::greatest();
   int idx = -1;
   LPSense sns = LPSense::Min;

   RealVector primal = solver.primalSolution();

   // examines the left bounds
   int j = 0;
   while (j < nlb)
   {
      int jv = lb[j]; // index of variable in the scope
      Variable aux = scope().var(jv);
      LinVar lin = solver.getLinVar(lzr_->linVarIndex(aux));
      double bnd = B.get(aux).left(), sol = primal[lin.getIndex()],
             delta = Double::abs(sol - bnd);

      LOG_LOW("Var: " << aux.getName() << ", left: " << bnd << ", sol: " << sol
                      << ", delta: " << delta);

      if ((Double::abs(bnd) < 1.0 && delta < feastol_) ||
          (Double::abs(bnd) >= 1.0 && Double::abs(delta / bnd) < feastol_))
      {
         // the primal solution is close enough to the left bound
         // this bound can be discarded
         LOG_LOW("Left bound of " << aux.getName() << " discarded");
         std::swap(lb[j], lb[--nlb]);
      }
      else
      {
         if (delta < dist)
         {
            // this bound minimizes the distance with its primal solution
            // (but it is not close enough acording to the tolerance)
            dist = delta;
            idx = j;
            sns = LPSense::Min;
         }
         j++;
      }
   }

   // examines the right bounds
   j = 0;
   while (j < nrb)
   {
      int jv = rb[j]; // index of variable in the scope
      Variable aux = scope().var(jv);
      LinVar lin = solver.getLinVar(lzr_->linVarIndex(aux));
      double bnd = B.get(aux).right(), sol = primal[lin.getIndex()],
             delta = Double::abs(sol - bnd);

      LOG_LOW("Var: " << aux.getName() << ", right: " << bnd << ", sol: " << sol
                      << ", delta: " << delta);

      if ((Double::abs(bnd) < 1.0 && delta < feastol_) ||
          (Double::abs(bnd) >= 1.0 && Double::abs(delta / bnd) < feastol_))
      {
         // the primal solution is close enough to the right bound
         // this bound can be discarded
         LOG_LOW("Right bound of " << aux.getName() << " discarded");
         std::swap(rb[j], rb[--nrb]);
      }
      else
      {
         if (delta < dist)
         {
            // this bound minimizes the distance to the primal solution
            // (but it is not close enough acording to the tolerance)
            dist = delta;
            idx = j;
            sns = LPSense::Max;
         }
         j++;
      }
   }

   if (idx == -1)
      iv = -1;
   else
   {
      sense = sns;
      if (sns == LPSense::Min)
      {
         iv = lb[idx];
         std::swap(lb[idx], lb[--nlb]);
      }
      else
      {
         iv = rb[idx];
         std::swap(rb[idx], rb[--nrb]);
      }
   }
}

void ContractorPolytope::selectNext(int *lb, int &nlb, int *rb, int &nrb, int &iv,
                                    LPSense &sense)
{
   if (nlb + nrb == 0)
   {
      iv = -1;
      return;
   }

   if ((nrb == 0) || (nlb > 0 && sense == LPSense::Max))
   {
      iv = lb[0];
      sense = LPSense::Min;
      std::swap(lb[0], lb[--nlb]);
   }
   else
   {
      iv = rb[0];
      sense = LPSense::Max;
      std::swap(rb[0], rb[--nrb]);
   }
   LOG_LOW("Select next var: " << iv);
}

void ContractorPolytope::print(std::ostream &os) const
{
   os << "Polytope hull contractor";
}

void ContractorPolytope::tuneLPSolver(LPSolver &solver)
{
   solver.setMaxIter(maxiter_);
   solver.setMaxSeconds(maxseconds_);
   solver.setFeasTol(feastol_);
}

} // namespace realpaver
