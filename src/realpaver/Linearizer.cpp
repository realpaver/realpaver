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
 * @file   Linearizer.cpp
 * @brief  Linearization of nonlinear problems
 * @author Laurent Granvilliers
 * @date   2025-fev-11
 */

#include "realpaver/AffineCreator.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Linearizer.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

Linearizer::Linearizer(SharedDag dag)
    : dag_(dag)
    , scop_(dag->scope())
    , lfun_()
    , tol_(Params::GetDblParam("RELAXATION_EQ_TOL"))
    , mvv_()
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");

   for (size_t i = 0; i < dag->nbFuns(); ++i)
      lfun_.push_back(i);

   scop_ = ScopeBank::getInstance()->insertScope(scop_);
}

Linearizer::Linearizer(SharedDag dag, const IndexList &lfun)
    : dag_(dag)
    , scop_()
    , lfun_()
    , tol_(Params::GetDblParam("RELAXATION_TOL"))
    , mvv_()
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");
   ASSERT(!lfun.empty(), "No list of functions in a polytope maker");

   for (size_t i : lfun)
   {
      ASSERT(i < dag_->nbFuns(), "Bad function index in a polytope maker");

      lfun_.push_back(i);
      scop_.insert(dag->fun(i)->scope());
   }

   scop_ = ScopeBank::getInstance()->insertScope(scop_);
}

Linearizer::~Linearizer()
{
}

SharedDag Linearizer::dag() const
{
   return dag_;
}

Scope Linearizer::scope() const
{
   return scop_;
}

size_t Linearizer::linVarIndex(Variable v) const
{
   auto it = mvv_.find(v.id());
   return it->second;
}

size_t Linearizer::linVarIndex(size_t id) const
{
   auto it = mvv_.find(id);
   return it->second;
}

void Linearizer::setLinVarIndex(size_t id, size_t k)
{
   auto it = mvv_.find(id);
   if (it != mvv_.end())
      it->second = k;
   else
      mvv_.insert(std::make_pair(id, k));
}

double Linearizer::getRelaxTol() const
{
   return tol_;
}

void Linearizer::setRelaxTol(double tol)
{
   ASSERT(tol >= 0.0, "The relaxation tolerance must be positive: " << tol);

   tol_ = tol;
}

/*----------------------------------------------------------------------------*/

LinearizerTaylor::LinearizerTaylor(SharedDag dag, bool hansen, CornerStyle style,
                                   unsigned seed)
    : Linearizer(dag)
    , hansen_(hansen)
    , style_(style)
    , corner_(scope().size())
    , gen_()
{
   if (seed == 0)
      gen_ = IntRandom();
   else
      gen_ = IntRandom(seed);
}

LinearizerTaylor::LinearizerTaylor(SharedDag dag, const IndexList &lfun, bool hansen,
                                   CornerStyle style, unsigned seed)
    : Linearizer(dag, lfun)
    , hansen_(hansen)
    , style_(style)
    , corner_(scope().size())
    , gen_()
{
   if (seed == 0)
      gen_ = IntRandom();
   else
      gen_ = IntRandom(seed);
}

void LinearizerTaylor::useHansenDerivatives(bool b)
{
   hansen_ = b;
}

void LinearizerTaylor::fixCorner(const Bitset &corner, bool opposite)
{
   ASSERT(corner_.size() == corner.size(), "Bad corner");

   style_ = opposite ? CornerStyle::UserOpposite : CornerStyle::User;
   corner_ = corner;
}

void LinearizerTaylor::selectCorner(const IntervalBox &B)
{
   switch (style_)
   {
   case CornerStyle::Random:
   case CornerStyle::RandomOpposite:
      for (size_t i = 0; i < corner_.size(); ++i)
      {
         if (gen_.nextBool())
            corner_.setOne(i);
         else
            corner_.setZero(i);
      }
      break;
   case CornerStyle::Left:
      corner_.setAllZero();
      break;
   case CornerStyle::Right:
      corner_.setAllOne();
      break;
   case CornerStyle::User:
   case CornerStyle::UserOpposite:
      // nothing
      break;
   }
}

void LinearizerTaylor::makeVars(LPModel &lpm, const IntervalBox &B)
{
   for (const auto &v : scop_)
   {
      Interval val = B.get(v);
      LinVar lv = lpm.makeVar(val.left(), val.right());
      lv.setName(v.getName());
      setLinVarIndex(v.id(), lv.getIndex());
   }
}

bool LinearizerTaylor::makeOne(LPModel &lpm, const IntervalBox &B, const Bitset &corner)
{
   // makes the corner of the box
   RealPoint c(scop_);
   for (const auto &v : scop_)
   {
      Interval dom = B.get(v);
      if (corner.get(scop_.index(v)))
         c.set(v, dom.right());
      else
         c.set(v, dom.left());
   }

   // evaluates the functions at c
   IntervalVector fc(lfun_.size());
   for (size_t i : lfun_)
   {
      DagFun *f = dag_->fun(i);
      Interval x = f->iEval(c);
      if (x.isEmpty())
         return false;
      fc.set(i, x);
   }

   // generates the constraints
   for (size_t i : lfun_)
   {
      DagFun *f = dag_->fun(i);
      Interval img = f->getImage();

      // transforms an equation into an inequality
      if (img.isSingleton())
         img += Interval(-tol_, tol_);

      // differentiates the function
      IntervalVector G(f->nbVars());
      if (hansen_)
         f->iDiffHansen(B, c, G);
      else
         f->iDiff(B, G);

      if (G.isEmpty())
         return false;

      if (G.isInf())
         continue;

      // lower bounding constraint of the form lo <= u generated if the right
      // bound of the image of the function is finite
      if (!Double::isInf(img.right()))
      {
         Interval u = img.right() - fc.get(i);
         LinExpr lo;

         for (const auto &v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));

            // derivative of f wrt. v
            Interval z = G[f->scope().index(v)];

            if (corner.get(scop_.index(v)))
            {
               // right bound used for this variable (bit = 1)
               // => right bound of the derivative
               lo.addTerm(z.right(), lv);
               u += z.right() * Interval(c.get(v));
            }
            else
            {
               // left bound used for this variable (bit = 0)
               // => left bound of the derivative
               lo.addTerm(z.left(), lv);
               u += z.left() * Interval(c.get(v));
            }
         }
         lpm.addCtr(lo, u.right());
      }

      // upper bounding constraint of the form up >= l generated if the left
      // bound of the image of the function is finite
      if (!Double::isInf(img.left()))
      {
         Interval l = img.left() - fc.get(i);

         LinExpr up;

         for (const auto &v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));

            // derivative of f wrt. v
            Interval z = G[f->scope().index(v)];

            if (corner.get(scop_.index(v)))
            {
               // right bound used for this variable (bit = 1)
               // => left bound of the derivative
               up.addTerm(z.left(), lv);
               l += z.left() * Interval(c.get(v));
            }
            else
            {
               // left bound used for this variable (bit = 1)
               // => right bound of the derivative
               up.addTerm(z.right(), lv);
               l += z.right() * Interval(c.get(v));
            }
         }
         lpm.addCtr(l.left(), up);
      }
   }
   return true;
}

bool LinearizerTaylor::makeCtrs(LPModel &lpm, const IntervalBox &B)
{
   // selects the first corner
   selectCorner(B);

   // generates the constraints for this corner
   bool b = makeOne(lpm, B, corner_);
   if (!b)
      return false;

   // manages the opposite corner if necessary
   if (style_ == CornerStyle::RandomOpposite || style_ == CornerStyle::UserOpposite)
   {
      corner_.flipAll();
      return makeOne(lpm, B, corner_);
   }
   else
      return true;
}

bool LinearizerTaylor::make(LPModel &lpm, const IntervalBox &B)
{
   makeVars(lpm, B);
   return makeCtrs(lpm, B);
}

/*----------------------------------------------------------------------------*/

LinearizerAffine::LinearizerAffine(SharedDag dag, bool minrange)
    : Linearizer(dag)
    , minrange_(minrange)
{
}

LinearizerAffine::LinearizerAffine(SharedDag dag, const IndexList &lfun, bool minrange)
    : Linearizer(dag, lfun)
    , minrange_(minrange)
{
}

void LinearizerAffine::useMinrange(bool minrange)
{
   minrange_ = minrange;
}

bool LinearizerAffine::make(LPModel &lpm, const IntervalBox &B)
{
   // creates the affine forms
   AffineCreator creator(dag_, lfun_, minrange_);
   creator.create(B);

   // creates the linear variables for the variables of the DAG
   for (const auto &v : scop_)
   {
      Interval val = B.get(v);
      LinVar lv = lpm.makeVar(val.left(), val.right());
      lv.setName(v.getName());
      setLinVarIndex(v.id(), lv.getIndex());
   }

   // creates the linear variables for the variables of the affine forms
   for (const auto &v : scop_)
   {
      // creates an affine variable associated with v
      LinVar ev = lpm.makeVar(-1.0, 1.0);
      ev.setName("e!" + v.getName());

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

      if (f.isEmpty())
         return false;
      if (f.isInf() || f.nbLinearTerms() == 0)
         continue;

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
         LinVar ev = lpm.getLinVar(linVarIndex(AffineForm::var(it)) + scop_.size());

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

/*----------------------------------------------------------------------------*/

LinearizerAffineTaylor::LinearizerAffineTaylor(SharedDag dag,
                                               std::unique_ptr<LinearizerAffine> affine,
                                               std::unique_ptr<LinearizerTaylor> taylor)
    : Linearizer(dag)
    , affine_(std::move(affine))
    , taylor_(std::move(taylor))
{
   ASSERT(affine_ != nullptr, "Null pointer (affine linearizer)");
   ASSERT(taylor_ != nullptr, "Null pointer (taylor linearizer)");
}

bool LinearizerAffineTaylor::make(LPModel &lpm, const IntervalBox &B)
{
   bool b = affine_->make(lpm, B);

   for (const auto &v : scop_)
   {
      // copy the association variable / linear variable from the affine-based
      // linearizer in this and in the taylor-based linearizer
      setLinVarIndex(v.id(), affine_->linVarIndex(v.id()));
      taylor_->setLinVarIndex(v.id(), affine_->linVarIndex(v.id()));
   }

   if (b)
      b = taylor_->makeCtrs(lpm, B);

   return b;
}

} // namespace realpaver
