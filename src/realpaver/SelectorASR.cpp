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
 * @file   SelectorASR.cpp
 * @brief  Variable selection strategy Affine Sum Relative
 * @author Laurent Granvilliers
 * @date   25 Apr 2025
 */

#include "realpaver/AffineCreator.hpp"
#include "realpaver/SelectorASR.hpp"

namespace realpaver {

SelectorASR::SelectorASR(SharedDag dag)
    : Selector(dag->scope())
    , dag_(dag)
    , asr_()
{
   for (size_t i = 0; i < dag->nbVars(); ++i)
   {
      Item itm = {scop_.var(i), 0.0};
      asr_.push_back(itm);
   }
}

SharedDag SelectorASR::getDag() const
{
   return dag_;
}

void SelectorASR::calculate(const IntervalBox &B)
{
   // creation of affine forms
   AffineCreator creator(dag_, false);
   creator.create(B);

   // creation of the matrix of coefficients of affine forms
   const Scope &scop = dag_->scope();
   RealMatrix M(dag_->nbFuns(), dag_->nbVars() + 1, 0.0);

   for (size_t i = 0; i < creator.nbFuns(); ++i)
   {
      const AffineForm &f = creator.fun(i);

      // sum of coefficients
      double s = 0.0;
      for (auto it = f.cbegin(); it != f.cend(); ++it)
      {
         const Variable &v = dag_->scope().var(it->var);
         s += abs(it->itv).right();
      }
      double e = f.errorTerm().right();
      s += e;

      // assignment of the matrix of relative coefficients
      for (auto it = f.cbegin(); it != f.cend(); ++it)
      {
         const Variable &v = dag_->scope().var(it->var);
         size_t j = scop.index(v);
         double a = abs(it->itv).right();
         M.set(i, j, a / s);
      }
      M.set(i, scop.size(), e / s);
   }

   // calculates the affineSumRel values
   for (size_t j = 0; j < dag_->nbVars(); ++j)
   {
      asr_[j].var = scope().var(j);
      asr_[j].val = 0.0;

      for (size_t i = 0; i < M.nrows(); ++i)
         asr_[j].val += M.get(i, j);
   }
}

void SelectorASR::sort()
{
   std::sort(asr_.begin(), asr_.end(), CompItem());
}

Variable SelectorASR::getMaxVar() const
{
   size_t imax = 0;
   double smax = asr_[0].val;

   for (size_t i = 0; i < nbVars(); ++i)
   {
      double d = asr_[i].val;
      if (d > smax)
      {
         smax = d;
         imax = i;
      }
   }
   return asr_[imax].var;
}

Variable SelectorASR::getVar(size_t i) const
{
   return asr_[i].var;
}

double SelectorASR::getAffineSumRel(size_t i) const
{
   return asr_[i].val;
}

size_t SelectorASR::nbVars() const
{
   return asr_.size();
}

bool SelectorASR::apply(const DomainBox &box)
{
   IntervalBox B(box);
   calculate(B);

   // Selects the variable with the maximum smear value
   Variable v = getMaxVar();
   if (box.isSplitable(v))
   {
      setSelectedVar(v);
      return true;
   }

   // Case when the domain of the maximum variable cannot be split
   for (size_t i = 0; i < nbVars(); ++i)
   {
      v = getVar(i);
      if (box.isSplitable(v))
      {
         setSelectedVar(v);
         return true;
      }
   }

   return false;
}

void SelectorASR::print(std::ostream &os) const
{
   for (size_t i = 0; i < asr_.size(); ++i)
   {
      os << "(" << asr_[i].var.getName() << "," << asr_[i].val << ") ";
   }
}

std::ostream &operator<<(std::ostream &os, const SelectorASR &asr)
{
   asr.print(os);
   return os;
}

} // namespace realpaver
