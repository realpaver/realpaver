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
 * @file   ConstraintPiecewise.cpp
 * @brief  Piecewise constraints
 * @author Laurent Granvilliers
 * @date   29 May 2025
 */

#include "Variable.hpp"
#include "realpaver/ConstraintPiecewise.hpp"
#include "realpaver/ConstraintVisitor.hpp"
#include "realpaver/Problem.hpp"
#include <initializer_list>

namespace realpaver {

size_t ConstraintPiecewise::nb_bins_ = 0;

const std::string ConstraintPiecewise::binary_prefix_ = "__z";

ConstraintPiecewise::ConstraintPiecewise(Variable v)
    : ConstraintRep(ConstraintRelation::Piecewise)
    , v_(v)
    , bins_()
    , itvs_()
    , ctrs_()
    , guard_hull_(Interval::emptyset())
{
   hcode_ = v.hashCode();
   Scope scop({v});
   setScope(scop);
}

void ConstraintPiecewise::addPiece(Variable b, const Interval &guard, Constraint body)
{
   REALPAVER_ASSERT(!guard.isEmpty(), "Empty guard in a piecewise constraint");
   REALPAVER_ASSERT(b.isBinary(), "Non binary variable in a piecewise constraint");

   // checks that the guards do not strictly overlap
   for (const auto &x : itvs_)
   {
      Interval y = x & guard;
      if (Double::prevDouble(y.right()) > Double::nextDouble(y.left()))
         THROW("The guards overlap: " << guard << ", " << x);
   }

   bins_.push_back(b);
   itvs_.push_back(guard);
   ctrs_.push_back(body);
   guard_hull_ |= guard;

   hcode_ = hash4(b.hashCode(), guard.hashCode(), body.hashCode(), hcode_);
   Scope scop = scope();
   scop.insert(body.scope());
   scop.insert(b);
   setScope(scop);
}

const Variable &ConstraintPiecewise::variable() const
{
   return v_;
}

const Variable &ConstraintPiecewise::binary(size_t i) const
{
   return bins_[i];
}

const Interval &ConstraintPiecewise::interval(size_t i) const
{
   return itvs_[i];
}

const Constraint &ConstraintPiecewise::constraint(size_t i) const
{
   return ctrs_[i];
}

size_t ConstraintPiecewise::nbPieces() const
{
   return itvs_.size();
}

bool ConstraintPiecewise::isConstant() const
{
   bool is_constant = true;
   size_t i = 0;
   while (is_constant && i < ctrs_.size())
   {
      is_constant &= ctrs_[i].isConstant();
      i++;
   }
   return is_constant;
}

Proof ConstraintPiecewise::isPieceSatisfied(const IntervalBox &B, const Interval &iv,
                                            size_t i)
{
   Interval bdom = B.get(bins_[i]);

   if (bdom.isZero())
      return Proof::Empty;

   if (!iv.overlaps(itvs_[i]))
      return Proof::Empty;

   return ctrs_[i].isSatisfied(B);
}

Proof ConstraintPiecewise::isPieceSatisfied(const DomainBox &box, const Interval &iv,
                                            size_t i)
{
   Interval bdom = box.get(bins_[i])->intervalHull();

   if (bdom.isZero())
      return Proof::Empty;

   if (!iv.overlaps(itvs_[i]))
      return Proof::Empty;

   return ctrs_[i].isSatisfied(box);
}

Proof ConstraintPiecewise::isSatisfied(const IntervalBox &B)
{
   Interval iv = B.get(v_);
   if (!guard_hull_.overlaps(iv))
      return Proof::Empty;

   Proof proof = Proof::Empty;
   for (size_t i = 0; i < nbPieces(); i++)
   {
      Proof certif = isPieceSatisfied(B, iv, i);
      if (certif != Proof::Empty)
      {
         // the proof can be Inner if only one piece is active and the
         // constraint is completely satisfied; it must be Maybe if two
         // pieces are (possibly) activated
         proof = (proof == Proof::Empty) ? certif : Proof::Maybe;
      }
   }
   return proof;
}

Proof ConstraintPiecewise::isSatisfied(const DomainBox &box)
{
   Interval iv = box.get(v_)->intervalHull();
   if (!guard_hull_.overlaps(iv))
      return Proof::Empty;

   Proof proof = Proof::Empty;
   for (size_t i = 0; i < nbPieces(); i++)
   {
      Proof certif = isPieceSatisfied(box, iv, i);
      if (certif != Proof::Empty)
      {
         // the proof can be Inner if only one piece is active and the
         // constraint is completely satisfied; it must be Maybe if two
         // pieces are (possibly) activated
         proof = (proof == Proof::Empty) ? certif : Proof::Maybe;
      }
   }
   return proof;
}

double ConstraintPiecewise::violation(const IntervalBox &B)
{
   Interval iv = B.get(v_);
   double res = 0.0;

   for (size_t i = 0; i < nbPieces(); i++)
   {
      Interval bdom = B.get(bins_[i]);
      if (bdom.isZero())
      {
         res += 1.0;
      }
      else
      {
         const Interval &itv = itvs_[i];

         if (!iv.overlaps(itv))
            res += iv.distance(itv);

         else
         {
            double cv = ctrs_[i].violation(B);
            if (cv == 0.0)
               return 0.0; // piece satisfied => no violation
            else
               res += cv;
         }
      }
   }
   return res;
}

double ConstraintPiecewise::violation(const DomainBox &box)
{
   Interval iv = box.get(v_)->intervalHull();
   double res = 0.0;

   for (size_t i = 0; i < nbPieces(); i++)
   {
      Interval bdom = box.get(bins_[i])->intervalHull();
      if (bdom.isZero())
      {
         res += 1.0;
      }
      else
      {
         const Interval &itv = itvs_[i];

         if (!iv.overlaps(itv))
            res += iv.distance(itv);

         else
         {
            double cv = ctrs_[i].violation(box);
            if (cv == 0.0)
               return 0.0; // piece satisfied => no violation
            else
               res += cv;
         }
      }
   }
   return res;
}

void ConstraintPiecewise::checkActive(const IntervalBox &B, size_t &nb_active,
                                      size_t &nb_possibly_active,
                                      size_t &num_active) const
{
   Interval iv = B.get(v_);
   nb_active = nb_possibly_active = 0;

   for (size_t i = 0; i < nbPieces(); i++)
   {
      // intersect the i-th interval with current box interval for v_
      Interval iv2 = itvs_[i] & iv;

      // domain of the binary variable associated with this guard
      Interval bdom = B.get(bins_[i]);
      int val = ((bdom.left() == 0) ? ((bdom.right() == 1) ? 0 : 1) : 2) +
                (iv2.isEmpty() ? 0 : 4);

      switch (val)
      {
      case 0: // bdom = [0, 1] and iv2 is empty => inactive
      case 1: // bdom = 0 and iv2 is empty => inactive
      case 5: // bdom = 0 and iv2 is not empty => inactive
         break;
      case 2: // bdom = 1 and iv2 is empty => inconsistent
         nb_active = 2;
         break;
      case 4: // bdom = [0, 1] and iv2 is not empty => unknown
         nb_possibly_active++;
         if (nb_active == 0)
            num_active = i;
         break;
      case 6: // bdom = 1 and iv2 is not empty => active
         nb_active++;
         num_active = i;
         break;
      }
   }
}

void ConstraintPiecewise::checkActive(const DomainBox &box, size_t &nb_active,
                                      size_t &nb_possibly_active,
                                      size_t &num_active) const
{
   Interval iv = box.get(v_)->intervalHull();
   nb_active = nb_possibly_active = 0;
   num_active = nbPieces();

   for (size_t i = 0; i < nbPieces(); i++)
   {
      // intersect the i-th interval with current box interval for v_
      Interval iv2 = itvs_[i] & iv;

      // domain of the binary variable associated with this guard
      Interval bdom = box.get(bins_[i])->intervalHull();

      int val = ((bdom.left() == 0) ? ((bdom.right() == 1) ? 0 : 1) : 2) +
                (iv2.isEmpty() ? 0 : 4);

      switch (val)
      {
      case 0: // bdom = [0, 1] and iv2 is empty => inactive
      case 1: // bdom = 0 and iv2 is empty => inactive
      case 5: // bdom = 0 and iv2 is not empty => inactive
         break;
      case 2: // bdom = 1 and iv2 is empty => inconsistent
         nb_active = 2;
         break;
      case 4: // bdom = [0, 1] and iv2 is not empty => unknown
         nb_possibly_active++;
         if (nb_active == 0)
            num_active = i;
         break;
      case 6: // bdom = 1 and iv2 is not empty => active
         nb_active++;
         num_active = i;
         break;
      }
   }
}

Proof ConstraintPiecewise::contract(IntervalBox &B)
{
   Interval iv = B.get(v_);
   iv &= guard_hull_;
   if (iv.isEmpty())
      return Proof::Empty;

   if (nbPieces() == 0)
      return Proof::Inner;

   // checks the activity of pieces
   size_t nb_active, nb_possibly_active, num_active;
   checkActive(B, nb_active, nb_possibly_active, num_active);

   // at least two binary variables are assigned to 1
   if (nb_active > 1)
      return Proof::Empty;

   // no active or possibly active constraint
   if (nb_active + nb_possibly_active == 0)
      return Proof::Empty;

   if (nb_active == 1)
   {
      // prunes undetermined binaries to 0
      for (size_t i = 0; i < nbPieces(); i++)
      {
         if (i != num_active)
            B.set(bins_[i], Interval::zero());
      }

      // prunes B using the active piece
      iv &= itvs_[num_active];
      B.set(v_, iv);
      return ctrs_[num_active].contract(B);
   }
   else
   {
      if (nb_possibly_active == 1)
      {
         // prunes undetermined binaries to 0
         for (size_t i = 0; i < nbPieces(); i++)
         {
            if (i != num_active)
               B.set(bins_[i], Interval::zero());
         }

         iv &= itvs_[num_active];
         B.set(v_, iv);
         B.set(bins_[num_active], Interval::one());
         return ctrs_[num_active].contract(B);
      }
   }

   return Proof::Maybe;
}

Proof ConstraintPiecewise::contract(DomainBox &box)
{
   Interval iv = box.get(v_)->intervalHull();
   iv &= guard_hull_;
   box.get(v_)->contractInterval(iv);
   if (iv.isEmpty())
      return Proof::Empty;

   if (nbPieces() == 0)
      return Proof::Inner;

   // checks the activity of pieces
   size_t nb_active, nb_possibly_active, num_active;
   checkActive(box, nb_active, nb_possibly_active, num_active);

   // at least two binary variables are assigned to 1
   if (nb_active > 1)
      return Proof::Empty;

   // no active or possibly active constraint
   if (nb_active + nb_possibly_active == 0)
      return Proof::Empty;

   if (nb_active == 1)
   {
      // prunes undetermined binaries to 0
      for (size_t i = 0; i < nbPieces(); i++)
      {
         if (i != num_active)
         {
            BinaryDomain *bdom = dynamic_cast<BinaryDomain *>(box.get(bins_[i]));
            bdom->setVal(ZeroOne::zero());
         }
      }

      // prunes B using the active piece
      iv &= itvs_[num_active];
      box.get(v_)->contractInterval(iv);
      return ctrs_[num_active].contract(box);
   }
   else
   {
      if (nb_possibly_active == 1)
      {
         // prunes undetermined binaries to 0
         for (size_t i = 0; i < nbPieces(); i++)
         {
            if (i != num_active)
            {
               BinaryDomain *bdom = dynamic_cast<BinaryDomain *>(box.get(bins_[i]));
               bdom->setVal(ZeroOne::zero());
            }
         }

         iv &= itvs_[num_active];
         box.get(v_)->contractInterval(iv);
         BinaryDomain *bdom = dynamic_cast<BinaryDomain *>(box.get(bins_[num_active]));
         bdom->setVal(ZeroOne::one());
         return ctrs_[num_active].contract(box);
      }
   }

   return Proof::Maybe;
}

void ConstraintPiecewise::print(std::ostream &os) const
{
   os << "piecewise(" << v_.getName() << ", { ";
   for (size_t i = 0; i < itvs_.size(); i++)
   {
      if (i > 0)
         os << ", ";
      os << itvs_[i] << " : " << ctrs_[i];
   }
   os << " }";
}

void ConstraintPiecewise::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

ConstraintRep *ConstraintPiecewise::cloneRoot() const
{
   ConstraintPiecewise *rep = new ConstraintPiecewise(v_);
   for (size_t i = 0; i < bins_.size(); ++i)
      rep->addPiece(bins_[i], itvs_[i], ctrs_[i]);
   return rep;
}

bool ConstraintPiecewise::isInteger() const
{
   for (const auto &c : ctrs_)
      if (!c.isInteger())
         return false;
   return true;
}

std::string ConstraintPiecewise::getNextBinaryName()
{
   std::stringstream stream;
   stream << binary_prefix_ << nb_bins_++;
   return stream.str();
}

Constraint piecewise(Variable v, const std::vector<Interval> &guards,
                     const std::vector<Constraint> &bodies, Problem &pb)
{
   REALPAVER_ASSERT(guards.size() == bodies.size(), "Bad piecewise constraint");
   REALPAVER_ASSERT(guards.size() > 0, "Bad piecewise constraint");

   std::shared_ptr<ConstraintPiecewise> rep = std::make_shared<ConstraintPiecewise>(v);
   for (size_t i = 0; i < guards.size(); i++)
   {
      std::string name = ConstraintPiecewise::getNextBinaryName();
      Variable b = pb.addBinaryVar(name);
      b.setStatus(VariableStatus::Auxiliary);
      rep->addPiece(b, guards[i], bodies[i]);
   }
   return Constraint(rep);
}

Constraint piecewise(Variable v, const std::initializer_list<Interval> &guards,
                     const std::initializer_list<Constraint> &bodies, Problem &pb)
{
   REALPAVER_ASSERT(guards.size() == bodies.size(), "Bad piecewise constraint");
   REALPAVER_ASSERT(guards.size() > 0, "Bad piecewise constraint");

   std::shared_ptr<ConstraintPiecewise> rep = std::make_shared<ConstraintPiecewise>(v);
   auto it1 = guards.begin();
   auto it2 = bodies.begin();
   for (; it1 != guards.end(); ++it1, ++it2)
   {
      std::string name = ConstraintPiecewise::getNextBinaryName();
      Variable b = pb.addBinaryVar(name);
      b.setStatus(VariableStatus::Auxiliary);
      rep->addPiece(b, *it1, *it2);
   }
   return Constraint(rep);
}

Constraint piecewise(Variable v, const std::vector<Variable> &bins,
                     const std::vector<Interval> &guards,
                     const std::vector<Constraint> &bodies)
{
   REALPAVER_ASSERT(bins.size() == guards.size(), "Bad piecewise constraint");
   REALPAVER_ASSERT(guards.size() == bodies.size(), "Bad piecewise constraint");
   REALPAVER_ASSERT(guards.size() > 0, "Bad piecewise constraint");

   std::shared_ptr<ConstraintPiecewise> rep = std::make_shared<ConstraintPiecewise>(v);
   for (size_t i = 0; i < guards.size(); i++)
   {
      rep->addPiece(bins[i], guards[i], bodies[i]);
   }
   return Constraint(rep);
}

Constraint piecewise(Variable v, const std::initializer_list<Variable> &bins,
                     const std::initializer_list<Interval> &guards,
                     const std::initializer_list<Constraint> &bodies)
{
   REALPAVER_ASSERT(bins.size() == guards.size(), "Bad piecewise constraint");
   REALPAVER_ASSERT(guards.size() == bodies.size(), "Bad piecewise constraint");
   REALPAVER_ASSERT(guards.size() > 0, "Bad piecewise constraint");

   std::shared_ptr<ConstraintPiecewise> rep = std::make_shared<ConstraintPiecewise>(v);
   auto it1 = bins.begin();
   auto it2 = guards.begin();
   auto it3 = bodies.begin();
   for (; it1 != bins.end(); ++it1, ++it2, ++it3)
   {
      rep->addPiece(*it1, *it2, *it3);
   }
   return Constraint(rep);
}

} // namespace realpaver
