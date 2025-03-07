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
 * @file   ContractorACID.cpp
 * @brief  ACID contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorVarCID.hpp"
#include "realpaver/ContractorACID.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorACID::ContractorACID(std::shared_ptr<IntervalSmearSumRel> ssr,
                               SharedContractor op, int ns3B, int nsCID,
                               int learnLength, int cycleLength, double ctRatio,
                               double varMinWidth)
      : Contractor(),
        ssr_(ssr),
        op_(op)
{
   ASSERT(ssr_ != nullptr, "No smear sum rel object in ACID");
   ASSERT(op_ != nullptr, "No operator in ACID");
   ASSERT(op->scope().contains(ssr->scope()), "Bad scopes in ACID");

   ASSERT(ns3B >= 2, "Bad number of slices for 3B contractors");
   ASSERT(nsCID >= 2, "Bad number of slices for CID contractors");
   ASSERT(learnLength >= 2, "Bad learning length in ACID");
   ASSERT(cycleLength > learnLength, "Bad cycle length in ACID");
   ASSERT(ctRatio > 0.0 && ctRatio < 1.0, "Bad ctRatio in ACID");

   scop_ = op_->scope();
   n_ = op_->scope().size();
   numVarCID_ = n_;

   ASSERT(n_ > 0, "No variable in ACID");

   for (size_t i=0; i<n_; ++i)
   {
      Variable v = scop_.var(i);
      ContractorVar3BCID* c3bc = new ContractorVar3BCID(op_, v, ns3B, nsCID);
      c3bc->setVarMinWidth(varMinWidth);
      var3BCID_.push_back(c3bc);
   }

   sumGood_ = 0.0;
   nbCalls_ = 0;
   cycleLength_ = cycleLength;
   learnLength_ = learnLength;
   ctRatio_ = ctRatio;
}

Scope ContractorACID::scope() const
{
   return ssr_->scope();
}

int ContractorACID::learnLength() const
{
   return learnLength_;
}

int ContractorACID::cycleLength() const
{
   return cycleLength_;
}

double ContractorACID::ctRatio() const
{
   return ctRatio_;
}

SharedContractor ContractorACID::sliceContractor() const
{
   return op_;
}

Proof ContractorACID::contract(IntervalBox& B)
{
   LOG_INTER("ACID call " << nbCalls_ << " on " << B);
   Proof proof = Proof::Maybe;

   int nbvarmax = 5*scop_.size();
   double* ctcGains = new double[nbvarmax];

   IntervalBox initbox(B);
   int vhandled;

   int mcall = nbCalls_ % cycleLength_;

   // learning phase ?
   if (mcall < learnLength_)
   {
      // first learning phase: one var3BCID per variable
      // next phases: two times the number of the previous number of var3BCID
      vhandled = (nbCalls_ < learnLength_) ? n_ : 2*numVarCID_;

      if (vhandled < 2) vhandled = 2;

      for (int i =0; i<nbvarmax; i++)
         ctcGains[i] = 0;
   }
   else
   {
      // exploitation phase
      vhandled = numVarCID_;
   }

   if (vhandled > nbvarmax)
      vhandled = nbvarmax;

   if (vhandled > 0)
   {
      // sorts the variables according to their impact
      ssr_->calculate(B);
      ssr_->sort();
   }

   IntervalBox save(B);

   int stop = 0;

   for (int i=0; i<vhandled; ++i)
   {
      size_t j = i % n_;
      Variable v = ssr_->getVar(j);
      size_t k = scop_.index(v);

      proof = var3BCID_[k]->contract(B);

      LOG_LOW(k << "-th var3BCID on " << v.getName());
      LOG_LOW(" -> " << proof << ", " << B);

      if (proof == Proof::Empty)
      {
         stop = i;
         break;
      }

      // learning phase: gains
      if (mcall < learnLength_)
      {
         ctcGains[i] = gainRatio(save, B, scop_);
      }

      save = B;
   }

   // learning phase: calculates the number of interesting variables
   if (mcall < learnLength_)
   {
      sumGood_ += (proof == Proof::Empty) ?
                     (stop+1) :
                     lastSignificantGain(ctcGains, ctRatio_, vhandled-1);
   }

   // end of learning phase
   if (mcall == learnLength_-1)
   {
      // fixes the number of var cided for the next exploitation phases
      numVarCID_ = (int)(std::ceil((double)sumGood_ / learnLength_));
      sumGood_ = 0;

      LOG_LOW("end of learning, nummVarCID <- " << numVarCID_);
   }

   delete[] ctcGains;
   ++nbCalls_;

   LOG_INTER(" -> " << proof << ", " << B);
   LOG_INTER("End of ACID");

   return proof;
}

void ContractorACID::print(std::ostream& os) const
{
   os << "ACID contractor";
}

int ContractorACID::lastSignificantGain(double* ctcGains,
                                        double ctRatio,
                                        int imax)
{
   int i = imax;

   while ((i>=0) && (ctcGains[i]<=ctRatio))
      --i;

   return i+1;
}

double ContractorACID::gainRatio(const IntervalBox& prev,
                                 const IntervalBox& next,
                                 const Scope& scop)
{
   double s = 0.0;

   for (const auto& v : scop)
   {
      Interval x = next.get(v),
               y = prev.get(v);

      if (!x.isInf() && !y.isSingleton())
         s += (1.0 - x.width() / y.width());
   }

   return s / scop.size();
}

} // namespace
