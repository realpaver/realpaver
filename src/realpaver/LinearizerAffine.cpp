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
 * @file   LinearizerAffine.cpp
 * @brief  Linearization of nonlinear problems based on affine forms
 * @author Laurent Granvilliers
 * @date   11 Feb 2025
 */

#include "realpaver/AffineCreator.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/DagExtended.hpp"
#include "realpaver/LinearizerAffine.hpp"

namespace realpaver {

LinearizerAffine::LinearizerAffine(SharedDagExtended edag, bool minrange)
    : Linearizer(edag)
    , minrange_(minrange)
{
   REALPAVER_ASSERT(edag != nullptr, "empty DAG");
}

void LinearizerAffine::useMinrange(bool minrange)
{
   minrange_ = minrange;
}

bool LinearizerAffine::makeLPImpl(LPModel &lpm, const IntervalBox &B,
                                  const Bitset &active)
{
   // creates the affine forms
   AffineCreator creator(edag_, minrange_);
   bool ok = creator.create(B, active);

   if (!ok)
      return false;

   // creates the linear variables for the variables of the DAG
   for (const auto &v : scop_)
   {
      Interval val = B.get(v);
      LinVar lv = lpm.makeVar(val.left(), val.right());
      lv.setName(v.getName()); // TODO : inutile
      setLinVarIndex(v.id(), lv.getIndex());
   }

   // inserts the affine forms as constraints in the linear model
   for (size_t k = 0; k < creator.nbFuns(); ++k)
   {
      const AffineForm &f = creator.fun(k);
      Interval cst = f.constantTerm();
      const Interval &err = f.errorTerm();
      const Interval &img = creator.dagFun(k)->getImage();

      // let img be the interval [L, U]
      // the constraint is: L <= a_0 + sum_i a_i*e_i + a_err*e_err <= U
      // where a_err>=0 is the magnitude of the error term and -1<=e_err<=1
      // Let E be the right bound of a_err. It follows:
      // L - a_0 - E <= sum_i a_i*e_i <= U - a_0 + E

      // the variables e_i do not need to be introduced
      // given the affine form of x_i = m_i + r_i*e_i we just replace
      // e_i by (x_i - m_i) / r_i

      Interval ac(0.0); // manages the rounding errors
      LinExpr expr;     // sum_i (a_i / r_i)*x_i

      Double::rndUp();
      auto it = f.cbegin();

      while (it != f.cend())
      {
         // index of e_i
         size_t i = AffineForm::var(it);

         // variable associated with e_i
         const Variable &xi = scop_.var(i);

         // coefficient a_i
         const Interval &ai = AffineForm::itv(it);

         // affine form of the variable associated with e_i
         AffineForm &fi = const_cast<AffineForm &>(creator.funEpsilon(i));

         // fi = mi + ri*e_i
         const Interval &mi = fi.constantTerm(), ri = AffineForm::itv(fi.begin());

         // term a_i*e_i -> a_i * (x_i - mi) / ri
         Interval ai_ri = ai / ri;
         std::pair<double, double> p = ai_ri.midrad();

         cst -= ai_ri * mi;
         expr.addTerm(p.first, lpm.getLinVar(linVarIndex(xi)));
         ac += Interval(p.second) * B.get(xi).width();

         ++it;
      }

      // lower bounding function
      if (!img.isInfRight())
      {
         Interval up = img.right() - cst + err + ac;
         lpm.addCtr(expr, up.right());
      }

      // upper bounding function
      if (!img.isInfLeft())
      {
         Interval lo = img.left() - cst - err - ac;
         lpm.addCtr(lo.left(), expr);
      }
   }
   return true;
}

} // namespace realpaver
