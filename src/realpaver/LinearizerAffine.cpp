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
#include "realpaver/LinearizerAffine.hpp"

namespace realpaver {

LinearizerAffine::LinearizerAffine(SharedDag dag, bool minrange)
    : Linearizer(dag)
    , minrange_(minrange)
    , mve_()
{
}

void LinearizerAffine::useMinrange(bool minrange)
{
   minrange_ = minrange;
}

bool LinearizerAffine::makeLPImpl(LPModel &lpm, const IntervalBox &B,
                                  const Bitset &active)
{
   // creates the affine forms
   AffineCreator creator(dag_, minrange_);
   bool ok = creator.create(B, active);

   if (!ok)
      return false;

   // creates the linear variables for the variables of the DAG
   for (const auto &v : scop_)
   {
      Interval val = B.get(v);
      LinVar lv = lpm.makeVar(val.left(), val.right());
      lv.setName(v.getName());
      setLinVarIndex(v.id(), lv.getIndex());
   }

   // creates the linear variables for the variables of the affine forms
   mve_.clear();
   for (const auto &v : scop_)
   {
      // no form if the variable does not occur in an active constraint
      if (!creator.hasFun(v))
         continue;

      // creates an affine variable associated with v
      LinVar ev = lpm.makeVar(-1.0, 1.0);
      ev.setName("e!" + v.getName());
      mve_.insert(std::make_pair(scop_.index(v), ev.getIndex()));

      // inserts the constraint v = m + r*ev <=> v - r*ev = m
      const AffineForm &f = creator.fun(v);
      double r = AffineForm::itv(f.cbegin()).left();
      Interval m = f.constantTerm() + Interval(-tol_, tol_);

      LinExpr e({1.0, -r}, {lpm.getLinVar(linVarIndex(v)), ev});
      lpm.addCtr(m.left(), e, m.right());
   }

   // inserts the affine forms as constraints in the linear model
   for (int i = 0; i < creator.nbFuns(); ++i)
   {
      const AffineForm &f = creator.fun(i);
      const Interval &cst = f.constantTerm();
      const Interval &err = f.errorTerm();
      Interval img = dag_->fun(i)->getImage();

      // let img be the interval [L, U]
      // the constraint is: L <= a_0 + sum_i a_i*e_i + a_err*e_err <= U
      // where a_err>=0 is the magnitude of the error term and -1<=e_err<=1
      // Let E be the right bound of a_err. It follows:
      // L - a_0 - E <= sum_i a_i*e_i <= U - a_0 + E

      Interval ac(0.0);
      LinExpr expr;

      Double::rndUp();
      auto it = f.cbegin();

      while (it != f.cend())
      {
         std::pair<double, double> p = AffineForm::itv(it).midrad();

         auto jt = mve_.find(AffineForm::var(it));

         LinVar ev = lpm.getLinVar(jt->second);
         expr.addTerm(p.first, ev);
         ac += Interval(p.second);
         ++it;
      }

      Interval low = img.left() - cst - err - ac, up = img.right() - cst + err + ac;

      if (Double::isInf(img.left()))
         lpm.addCtr(expr, up.right());

      else if (Double::isInf(img.right()))
         lpm.addCtr(low.left(), expr);

      else
         lpm.addCtr(low.left(), expr, up.right());
   }

   return true;
}

} // namespace realpaver
