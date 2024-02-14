///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorBC4Revise.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorBC4Revise::ContractorBC4Revise(SharedDag dag, size_t i)
      : dag_(dag),
        if_(i),
        hc4_(nullptr),
        bc3_()
{
   hc4_ = new ContractorHC4Revise(dag, i);

   DagFun* f = dag->fun(i);
   Scope s = f->scope();
   for (auto v : s)
   {
      if (f->nbOccurrences(v) > 1)
         bc3_.push_back(new ContractorBC3Revise(dag_, if_, v));
   }
}

ContractorBC4Revise::~ContractorBC4Revise()
{
   delete hc4_;

   for (auto it : bc3_) delete it;
}

Scope ContractorBC4Revise::scope() const
{
   return dag_->fun(if_)->scope();
}

Proof ContractorBC4Revise::contract(IntervalBox& B)
{
   LOG_LOW("BC4Revise contractor @ " << if_ << " on " << B);
   
   // HC4
   Proof proof = hc4_->contract(B);

   if (proof != Proof::Maybe)
   {
      LOG_LOW("BC4Revise -> " << proof);
      return proof;
   }

   // BC3
   for (size_t i=0; i<bc3_.size(); ++i)
   {
      Proof certif = bc3_[i]->contract(B);

      if (certif == Proof::Empty)
      {
         LOG_LOW("BC4Revise -> " << certif);
         return certif;
      }
      else
         proof = std::max(proof, certif);
   }

   LOG_LOW("BC4Revise -> " << proof);
   return proof;
}

void ContractorBC4Revise::print(std::ostream& os) const
{
   os << "BC4Revise contractor #" << if_;
}

} // namespace
