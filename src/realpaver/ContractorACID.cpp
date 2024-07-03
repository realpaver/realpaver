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

   kVarCID_.insert(kVarCID_.begin(), learnLength, 0);

   call_ = 0;
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
   LOG_INTER("ACID call " << call_ << " on " << B);

   Proof proof = Proof::Maybe;

   // sorts the variables according to their impact
   ssr_->calculate(B);
   ssr_->sort();

   int mcall = call_ % cycleLength_;

   if (mcall < learnLength_)
   {
      // learning phase
      int nVarCID = std::max(2, 2*numVarCID_);
      std::vector<double> ctcGains(nVarCID);

      int i = 0;
      while ((proof != Proof::Empty) && (i<nVarCID))
      {
         IntervalBox save(B);

         size_t j = i % n_;
         Variable v = ssr_->getVar(j);
         size_t k = scop_.index(v);

         proof = var3BCID_[k]->contract(B);

         LOG_LOW("learning: " << k << "-th var3BCID on " << v.getName());
         LOG_LOW(" -> " << proof << ", " << B);

         if (proof != Proof::Empty)
         {
            ctcGains[i] = B.gainRatio(save);
            ++i;
            LOG_LOW("gain: " << ctcGains[i]);
         }
      }

      // number of contractors that have been applied till a significant gain
      if (proof == Proof::Empty)
      {
         kVarCID_[mcall] = i+1;

      }
      else
      {
         kVarCID_[mcall] = lastSignificantGain(ctcGains, ctRatio_);
      }

      LOG_LOW("kVarCID[" << mcall << "] <- " << kVarCID_[mcall]);

      // end of learning phase
      if (mcall == learnLength_-1)
      {
         numVarCID_ = avgNbVarCID(kVarCID_);

         LOG_LOW("end of learning, nummVarCID <- " << numVarCID_);
      }
   }
   else
   {
      // exploitation phase
      if (numVarCID_ == 0)
      {
         proof = op_->contract(B);

         LOG_LOW("exploitation, HC4 -> " << proof << ", " << B);
      }
      else
      {
         size_t i=0;
         while ((proof != Proof::Empty) && (i<(size_t)numVarCID_))
         {
            size_t j = i % n_;
            Variable v = ssr_->getVar(j);
            size_t k = scop_.index(v);

            proof = var3BCID_[k]->contract(B);
            ++i;

            LOG_LOW("exploitaion: " << k << "-th var3BCID on " << v.getName());
            LOG_LOW(" -> " << proof << ", " << B);
         }
      }
   }

   ++call_;

   LOG_INTER(" -> " << proof << ", " << B);
   LOG_INTER("End of ACID");

   return proof;
}




//~ Proof ContractorACID::contract(IntervalBox& B)
//~ {
   //~ LOG_INTER("ACID call " << call_ << " on " << B);

   //~ Proof proof = Proof::Maybe;

   //~ // sorts the variables according to their impact
   //~ ssr_->calculate(B);
   //~ ssr_->sort();

   //~ int mcall = call_ % cycleLength_;

   //~ if (mcall < learnLength_)
   //~ {
      //~ // learning phase
      //~ int nVarCID = std::max(2, 2*numVarCID_);
      //~ std::vector<double> ctcGains(nVarCID);

      //~ int i = 0;
      //~ while ((proof != Proof::Empty) && (i<nVarCID))
      //~ {
         //~ IntervalBox save(B);

         //~ size_t j = i % n_;
         //~ Variable v = ssr_->getVar(j);
         //~ size_t k = scop_.index(v);

         //~ proof = var3BCID_[k]->contract(B);

         //~ LOG_LOW("learning: " << k << "-th var3BCID on " << v.getName());
         //~ LOG_LOW(" -> " << proof << ", " << B);

         //~ if (proof != Proof::Empty)
         //~ {
            //~ ctcGains[i] = B.gainRatio(save);
            //~ ++i;
            //~ LOG_LOW("gain: " << ctcGains[i]);
         //~ }
      //~ }

      //~ // number of contractors that have been applied till a significant gain
      //~ if (proof == Proof::Empty)
      //~ {
         //~ kVarCID_[mcall] = i+1;

      //~ }
      //~ else
      //~ {
         //~ kVarCID_[mcall] = lastSignificantGain(ctcGains, ctRatio_);
      //~ }

      //~ LOG_LOW("kVarCID[" << mcall << "] <- " << kVarCID_[mcall]);

      //~ // end of learning phase
      //~ if (mcall == learnLength_-1)
      //~ {
         //~ numVarCID_ = avgNbVarCID(kVarCID_);

         //~ LOG_LOW("end of learning, nummVarCID <- " << numVarCID_);
      //~ }
   //~ }
   //~ else
   //~ {
      //~ // exploitation phase
      //~ if (numVarCID_ == 0)
      //~ {
         //~ proof = op_->contract(B);

         //~ LOG_LOW("exploitation, HC4 -> " << proof << ", " << B);
      //~ }
      //~ else
      //~ {
         //~ size_t i=0;
         //~ while ((proof != Proof::Empty) && (i<(size_t)numVarCID_))
         //~ {
            //~ size_t j = i % n_;
            //~ Variable v = ssr_->getVar(j);
            //~ size_t k = scop_.index(v);

            //~ proof = var3BCID_[k]->contract(B);
            //~ ++i;

            //~ LOG_LOW("exploitaion: " << k << "-th var3BCID on " << v.getName());
            //~ LOG_LOW(" -> " << proof << ", " << B);
         //~ }
      //~ }
   //~ }

   //~ ++call_;

   //~ LOG_INTER(" -> " << proof << ", " << B);
   //~ LOG_INTER("End of ACID");

   //~ return proof;
//~ }




void ContractorACID::print(std::ostream& os) const
{
   os << "ACID contractor";
}


int ContractorACID::lastSignificantGain(std::vector<double>& ctcGains,
                                  double ctRatio)
{
   int i = ctcGains.size()-1;

   while ((i>=0) && (ctcGains[i]<=ctRatio))
      --i;

   return i+1;
}

size_t ContractorACID::avgNbVarCID(std::vector<size_t>& v)
{   
   double s = 0;
   for (const size_t& n : v)
      s += n;

   double a = s / v.size(),
          f = std::floor(a),
          d = a - f,
          res = (d <= 0.5) ? f : f+1;

   return (int)res;
}

} // namespace
