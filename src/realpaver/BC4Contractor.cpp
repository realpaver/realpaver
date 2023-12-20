///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/BC4Contractor.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

BC4Contractor::BC4Contractor(SharedDag dag, size_t i)
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

BC4Contractor::~BC4Contractor()
{
   delete hc4_;

   for (auto it : bc3_) delete it;
}

Scope BC4Contractor::scope() const
{
   return dag_->fun(if_)->scope();
}

Proof BC4Contractor::contract(IntervalBox& box)
{
   LOG_LOW("BC4 contractor @ " << if_ << " on " << box);
   
   // HC4
   Proof proof = hc4_->contract(box);

   if (proof != Proof::Maybe)
   {
      LOG_LOW("BC4 -> " << proof);
      return proof;
   }

   // BC3
   for (size_t i=0; i<bc3_.size(); ++i)
   {
      Proof certif = bc3_[i]->contract(box);

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

void BC4Contractor::print(std::ostream& os) const
{
   os << "BC4 contractor #" << if_;
}

} // namespace
