///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorBC4.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorBC4::ContractorBC4(SharedDag dag, size_t i)
      : dag_(dag),
        if_(i),
        hc4_(nullptr),
        bc3_()
{
   hc4_ = new ContractorHC4(dag, i);

   DagFun* f = dag->fun(i);
   Scope s = f->scope();
   for (auto v : s)
   {
      if (f->nbOccurrences(v) > 1)
         bc3_.push_back(new ContractorBC3(dag_, if_, v));
   }
}

ContractorBC4::~ContractorBC4()
{
   delete hc4_;

   for (auto it : bc3_) delete it;
}

Scope ContractorBC4::scope() const
{
   return dag_->fun(if_)->scope();
}

Proof ContractorBC4::contract(IntervalBox& B)
{
   LOG_LOW("BC4 contractor @ " << if_ << " on " << B);
   
   // HC4
   Proof proof = hc4_->contract(B);

   if (proof != Proof::Maybe)
   {
      LOG_LOW("BC4 -> " << proof);
      return proof;
   }

   // BC3
   for (size_t i=0; i<bc3_.size(); ++i)
   {
      Proof certif = bc3_[i]->contract(B);

      if (certif == Proof::Empty)
      {
         LOG_LOW("BC4 -> " << certif);
         return certif;
      }
      else
         proof = std::max(proof, certif);
   }

   LOG_LOW("BC4 -> " << proof);
   return proof;
}

void ContractorBC4::print(std::ostream& os) const
{
   os << "BC4 contractor #" << if_;
}

} // namespace
