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
 * @file   IntervalSmearSumRel.cpp
 * @brief  Smear sum relative strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include <algorithm>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalSmearSumRel.hpp"

namespace realpaver {

IntervalSmearSumRel::IntervalSmearSumRel(IntervalFunctionVector F)
      : F_(F),
        scop_(F.scope()),
        ssr_()
{
   for (size_t i=0; i<F_.nbVars(); ++i)
   {
      Item itm = { scop_.var(i), 0.0 };
      ssr_.push_back(itm);
   }
}

Scope IntervalSmearSumRel::scope() const
{
   return scop_;
}

IntervalFunctionVector IntervalSmearSumRel::getFun() const
{
   return F_;
}

Variable IntervalSmearSumRel::getMaxVar() const
{
   size_t imax = 0;
   double smax = ssr_[0].val;

   for (size_t i=0; i<F_.nbVars(); ++i)
   {
      double d = ssr_[i].val;
      if (d > smax)
      {
         smax = d;
         imax = i;
      }
   }
   return ssr_[imax].var;
}

void IntervalSmearSumRel::calculate(const IntervalBox& B)
{
   IntervalMatrix jac(F_.nbFuns(), F_.nbVars());
   RealMatrix S(F_.nbFuns(), F_.nbVars(), 0.0);

   // calculates the partial derivatives
   F_.diff(B, jac);

   // calculates the relative smear values
   for (size_t i=0; i<F_.nbFuns(); ++i)
   {
      double sum = 0.0;
      for (size_t j=0; j<F_.nbVars(); ++j)
      {
         const auto& v = scop_.var(j);
         double smear = jac.get(i, j).mag() * B.get(v).width();
         S.set(i, j, smear);
         sum += smear;
      }
      if (sum != 0.0)
      {
         for (size_t j=0; j<F_.nbVars(); ++j)
            S.set(i, j, S.get(i, j) / sum);
      }
   }

   // calculates the smearRelSum values
   for (size_t j=0; j<F_.nbVars(); ++j)
   {
      ssr_[j].var = scop_.var(j);
      ssr_[j].val = 0.0;

      for (size_t i=0; i<F_.nbFuns(); ++i)
         ssr_[j].val += S.get(i, j);
   }
}

void IntervalSmearSumRel::sort()
{
   std::sort(ssr_.begin(), ssr_.end(), CompItem());
}

Variable IntervalSmearSumRel::getVar(size_t i) const
{
   return ssr_[i].var;
}

double IntervalSmearSumRel::getSmearSumRel(size_t i) const
{
   return ssr_[i].val;
}

size_t IntervalSmearSumRel::nbVars() const
{
   return ssr_.size();
}

void IntervalSmearSumRel::print(std::ostream& os) const
{
   for (size_t i=0; i<ssr_.size(); ++i)
   {
      os << "(" << ssr_[i].var.getName() << "," << ssr_[i].val << ") ";
   }
}

std::ostream& operator<<(std::ostream& os, const IntervalSmearSumRel& ssr)
{
   ssr.print(os);
   return os;
}

} // namespace
