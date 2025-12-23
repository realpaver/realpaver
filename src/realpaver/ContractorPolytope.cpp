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
 * @date   11 Feb 2025
 */

#include "RealPoint.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorPolytope.hpp"
#include "realpaver/LPModel.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

LinearizeMethod::~LinearizeMethod()
{
}

bool LinearizeSimple::apply(Linearizer &lzr, LPModel &lpm, IntervalBox &B) const
{
   return lzr.makeLP(lpm, B);
}

LinearizeActive::LinearizeActive(const Bitset &active)
    : LinearizeMethod()
    , active_(active)
{
}

bool LinearizeActive::apply(Linearizer &lzr, LPModel &lpm, IntervalBox &B) const
{
   return lzr.makeLP(lpm, B, active_);
}

/*----------------------------------------------------------------------------*/

ContractorPolytope::ContractorPolytope(std::unique_ptr<Linearizer> lzr)
    : Contractor()
    , lzr_(std::move(lzr))
    , maxseconds_(Params::GetDblParam("LP_TIME_LIMIT"))
    , maxiter_(Params::GetIntParam("LP_ITER_LIMIT"))
    , feastol_(Params::GetDblParam("LP_FEAS_TOL"))
    , loop_(Params::GetStrParam("POLYTOPE_HULL_LOOP") == "YES")
    , looptol_(Params::GetDblParam("POLYTOPE_HULL_LOOP_TOL"))
    , id_var_obj_(-1)
    , has_minimizer_(false)
    , minimizer_(nullptr)
{
   REALPAVER_ASSERT((lzr_ != nullptr), "Empty linearizer in a polytope contractor");

   for (const auto &v : scope())
      if (v.getStatus() == VariableStatus::Objective)
      {
         id_var_obj_ = scope().index(v);
         break;
      }

   minimizer_ = new RealPoint(scope());
}

ContractorPolytope::~ContractorPolytope()
{
   delete minimizer_;
}

double ContractorPolytope::getRelaxTol() const
{
   return lzr_->getRelaxTol();
}

void ContractorPolytope::setRelaxTol(double tol)
{
   REALPAVER_ASSERT(tol >= 0.0, "The relaxation tolerance must be positive: " << tol);

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
   REALPAVER_ASSERT(tol > 0.0, "Bad tolerance in the polytope hull contractor");

   feastol_ = tol;
}

void ContractorPolytope::enforceLoop(bool loop, double tol)
{
   REALPAVER_ASSERT(tol >= 0.0 && tol <= 1.0,
                    "A relative tolerance must belong to [0, 1]");
   loop_ = loop;
   looptol_ = tol;
}

Proof ContractorPolytope::contract(IntervalBox &B)
{
   return contractLoop(B, LinearizeSimple());
}

Proof ContractorPolytope::contract(IntervalBox &B, const Bitset &active)
{
   return contractLoop(B, LinearizeActive(active));
}

Proof ContractorPolytope::contractLoop(IntervalBox &B, const LinearizeMethod &linearize)
{
   if (!loop_)
      return contractImpl(B, linearize);

   bool iter = true;
   IntervalBox prev(B);
   while (iter)
   {
      if (contractImpl(B, linearize) == Proof::Empty)
         return Proof::Empty;
      if (!B.improves(prev, looptol_))
         iter = false;
      else
         prev.setOnScope(B, scope());
   }
   return Proof::Maybe;
}

Proof ContractorPolytope::contractImpl(IntervalBox &B, const LinearizeMethod &linearize)
{
   LOG_INTER("Polytope hull contractor " << B);

   // defines and tunes the LP solver
   LPSolver solver;
   tuneLPSolver(solver);

   // linearizes the constraints
   bool ok = linearize.apply(*lzr_, solver, B);
   if (!ok)
      return Proof::Maybe;

   LOG_LOW("Relaxation " << solver);

   // creates additional structures
   int nv = scope().size(), // number of variables
       nlb,                 // number of left bounds
       nrb,                 // number of right bounds
       *lb,                 // array of indexes of variables for the left bounds
       *rb,                 // array of indexes of variables for the right bounds
       iv;                  // current variable to be handled
   LPSense sense;           // current optimization sense

   // the bounds of all the variables but the objective variable are considered here
   if (id_var_obj_ < 0)
      nlb = nrb = nv;
   else
      nlb = nrb = nv - 1;

   lb = new int[nlb];
   rb = new int[nrb];

   int k = 0;
   for (int i = 0; i < nv; ++i)
   {
      if (i != id_var_obj_)
      {
         lb[k] = rb[k] = i;
         ++k;
      }
   }

   // current variable to be handled
   iv = lb[0];
   sense = LPSense::Min;
   lb[0] = lb[nlb - 1];
   --nlb;

   has_minimizer_ = false;
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
         if (iv == id_var_obj_)
            makeMinimizer(solver);

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
      if (id_var_obj_ < 0 || iv == id_var_obj_)
      {
         iv = -1;
      }
      else
      {
         // last problem: minimize the objective variable
         iv = id_var_obj_;
         sense = LPSense::Min;
      }
      return;
   }

   // Achterberg's heuristics. Each bound that is too close to the current
   // primal solution is discarded. Among the other bounds, the one that is
   // the clothest to the primal solution is selected, which aims at
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
   {
      if (id_var_obj_ < 0)
      {
         iv = -1;
      }
      else
      {
         // last problem: minimize the objective variable
         iv = id_var_obj_;
         sense = LPSense::Min;
      }
   }
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
      if (id_var_obj_ < 0 || iv == id_var_obj_)
      {
         iv = -1;
      }
      else
      {
         iv = id_var_obj_;
         sense = LPSense::Min;
      }
   }
   else
   {
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
   }

   LOG_LOW("Select next var: " << iv);
}

bool ContractorPolytope::hasMinimizer() const
{
   return has_minimizer_;
}

const RealPoint &ContractorPolytope::minimizer() const
{
   return *minimizer_;
}

void ContractorPolytope::makeMinimizer(LPSolver &solver)
{
   RealVector primal = solver.primalSolution();
   for (auto v : scope())
   {
      LinVar lv = solver.getLinVar(lzr_->linVarIndex(v));
      minimizer_->set(v, primal[lv.getIndex()]);
   }

   has_minimizer_ = true;
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
