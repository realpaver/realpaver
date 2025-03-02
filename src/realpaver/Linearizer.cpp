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

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Linearizer.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

Linearizer::Linearizer(SharedDag dag)
      : dag_(dag),
        scop_(dag->scope()),
        mvv_(),
        lfun_(),
        tol_(Param::GetDblParam("RELAXATION_EQ_TOL"))
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");

   for (size_t i=0; i<dag->nbFuns(); ++i)
      lfun_.push_back(i);

   scop_ = ScopeBank::getInstance()->insertScope(scop_);
}

Linearizer::Linearizer(SharedDag dag, const IndexList& lfun)
      : dag_(dag),
        scop_(),
        lfun_(),
        tol_(Param::GetDblParam("RELAXATION_TOL"))
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");
   ASSERT(!lfun.empty(), "No list of functions in a polytope maker");

   for (size_t i : lfun)
   {
      ASSERT(i<dag_->nbFuns(), "Bad function index in a polytope maker");

      lfun_.push_back(i);
      scop_.insert(dag->fun(i)->scope());
   }

   scop_ = ScopeBank::getInstance()->insertScope(scop_);
}

Linearizer::~Linearizer()
{}

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

double Linearizer::getRelaxTol() const
{
   return tol_;
}

void Linearizer::setRelaxTol(double tol)
{
   ASSERT(tol >= 0.0,
          "The relaxation tolerance must be positive: " << tol);

   tol_ = tol;
}

/*----------------------------------------------------------------------------*/

LinearizerTaylor::LinearizerTaylor(SharedDag dag, bool hansen,
                                   CornerStyle style)
      : Linearizer(dag),
        hansen_(hansen),
        style_(style),
        corner_(scope().size()),
        gen_(SEED)
{
   if (style == CornerStyle::Random) gen_ = IntRandom();
}

LinearizerTaylor::LinearizerTaylor(SharedDag dag, const IndexList& lfun,
                                   bool hansen, CornerStyle style)
      : Linearizer(dag, lfun),
        hansen_(hansen),
        style_(style),
        corner_(scope().size()),
        gen_(SEED)
{
   if (style == CornerStyle::Random) gen_ = IntRandom();
}

void LinearizerTaylor::useHansenDerivatives(bool b)
{
   hansen_ = b;
}

void LinearizerTaylor::useRandomCorners(unsigned seed)
{
   if (seed == 0)
   {
      style_ = CornerStyle::Random;
      gen_ = IntRandom();
   }
   else
   {
      style_ = CornerStyle::RandomSeed;
      gen_ = IntRandom(seed);
   }
}

void LinearizerTaylor::fixFirstCorner(const Bitset& corner)
{
   ASSERT(corner_.size() == corner.size(), "Bad corner");

   style_ = CornerStyle::User;
   corner_ = corner;
}

void LinearizerTaylor::selectCorner(const IntervalBox& B)
{
   if (style_ == CornerStyle::User) return;

   for (size_t i=0; i<corner_.size(); ++i) {
      if (gen_.nextBool())
         corner_.setOne(i);
      else
         corner_.setZero(i);
   }
}

bool LinearizerTaylor::make(LPModel& lpm, const IntervalBox& B)
{
   // selects the first corner
   selectCorner(B);

   // creates the linear variables
   for (const auto& v : scop_)
   {
      Interval val = B.get(v);
      LinVar lv = lpm.makeVar(val.left(), val.right());
      lv.setName(v.getName());
      mvv_.insert(std::make_pair(v.id(), lv.getIndex()));
   }

   // makes the two opposite corners
   RealPoint c1(scop_), c2(scop_);
   for (const auto& v : scop_)
   {
      Interval dom = B.get(v);
      if (corner_.get(scop_.index(v)))
      {
         c1.set(v, dom.right());
         c2.set(v, dom.left());
      }
      else
      {
         c1.set(v, dom.left());
         c2.set(v, dom.right());
      }
   }

   // evaluates the functions at both corners
   IntervalVector fc1(lfun_.size()),
                  fc2(lfun_.size());
   for (size_t i : lfun_)
   {
      DagFun* f = dag_->fun(i);
      Interval x1 = f->iEval(c1),
               x2 = f->iEval(c2);

      if (x1.isEmpty() || x2.isEmpty()) return false;

      fc1.set(i, x1);
      fc2.set(i, x2);
   }

   // generates the constraints
   for (size_t i : lfun_)
   {
      DagFun* f = dag_->fun(i);
      Interval img = f->getImage();

      // transforms an equation into an inequality
      if (img.isSingleton())
         img += Interval(-tol_, tol_);

      // differentiates the function
      IntervalVector G(f->nbVars());
      if (hansen_)
         f->iDiffHansen(B, G);
      else
         f->iDiff(B, G);

      // lower bounding constraints
      // assumes that the right bound of the image of the function is finite
      // we generate two linear constraints, one per corner
      // the first one has the form lo1 <= u1 where lo1 is the non constant
      // part of the constraint and u1 is the constant part
      // the second one lo2 <= u2 is built similarly
      if (!Double::isInf(img.right()))
      {
         Interval u1 = img.right() - fc1.get(i),  // U - f(c1)
                  u2 = img.right() - fc2.get(i);  // U - f(c2)

         LinExpr lo1, lo2;

         for (const auto& v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));

            // derivative of f wrt. v
            Interval z = G[f->scope().index(v)];
            if (z.isEmpty() || z.isInf()) return false;

            if (corner_.get(scop_.index(v)))
            {
               // right bound used for this variable (bit = 1)
               // first corner => right bound of the derivative
               lo1.addTerm(z.right(), lv);
               u1 += z.right() * Interval(c1.get(v));

               // second (opposite) corner => left bound of the derivative
               lo2.addTerm(z.left(), lv);
               u2 += z.left() * Interval(c2.get(v));
            }
            else
            {
               // left bound used for this variable (bit = 0)
               // first corner => left bound of the derivative
               lo1.addTerm(z.left(), lv);
               u1 += z.left() * Interval(c1.get(v));

               // second (opposite) corner => right bound of the derivative
               lo2.addTerm(z.right(), lv);
               u2 += z.right() * Interval(c2.get(v));
            }
         }
         lpm.addCtr(lo1, u1.right());
         lpm.addCtr(lo2, u2.right());
      }

      // upper bounding constraints
      // assumes that the left bound of the image of the function is finite
      // we generate two linear constraints, one per corner
      // the first one has the form up1 >= l1 where up1 is the non constant
      // part of the constraint and l1 is the constant part
      // the second one up2 >= l2 is built similarly
      if (!Double::isInf(img.left()))
      {
         Interval l1 = img.left() - fc1.get(i),
                  l2 = img.left() - fc2.get(i);

         LinExpr up1, up2;

         for (const auto& v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));

            // derivative of f wrt. v
            Interval z = G[f->scope().index(v)];

            if (corner_.get(scop_.index(v)))
            {
               // right bound used for this variable (bit = 1)
               // first corner => left bound of the derivative
               up1.addTerm(z.left(), lv);
               l1 += z.left() * Interval(c1.get(v));

               // second (opposite) corner => right bound of the derivative
               up2.addTerm(z.right(), lv);
               l2 += z.right() * Interval(c2.get(v));
            }
            else
            {
               // left bound used for this variable (bit = 1)
               // first corner => right bound of the derivative
               up1.addTerm(z.right(), lv);
               l1 += z.right() * Interval(c1.get(v));

               // second (opposite) corner => left bound of the derivative
               up2.addTerm(z.left(), lv);
               l2 += z.left() * Interval(c2.get(v));
            }
         }
         lpm.addCtr(l1.left(), up1);
         lpm.addCtr(l2.left(), up2);
      }
   }
   return true;
}

bool LinearizerTaylor::areEquals(const LinExpr& e1, const LinExpr& e2)
{
   if (e1.getNbTerms() != e2.getNbTerms()) return false;
   for (int i=0; i<e1.getNbTerms(); ++i)
   {
      if (e1.getIndexVar(i) != e2.getIndexVar(i)) return false;
      if (e1.getCoef(i) != e2.getCoef(i)) return false;
   }
   return true;
}

} // namespace
