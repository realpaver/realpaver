///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BC4Contractor.hpp"

namespace realpaver {

BC4Contractor::BC4Contractor(SharedDag dag, size_t i)
      : dag_(dag),
        if_(i),
        hc4_(nullptr),
        bc3_()
{
   hc4_ = new HC4Contractor(dag, i);

   Scope s = dag->fun(i)->scope();
   for (auto v : s)
   {
      if (s.count(v) > 1)
         bc3_.push_back(new BC3Contractor(dag_, if_, v));
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

bool BC4Contractor::dependsOn(const Bitset& bs) const
{
  return dag_->fun(if_)->dependsOn(bs); 
}

Proof BC4Contractor::contract(IntervalRegion& reg)
{
   // HC4
   Proof proof = hc4_->contract(reg);

   if (proof != Proof::Maybe) return proof;

   // BC3
   for (size_t i=0; i<bc3_.size(); ++i)
   {
      Proof certif = bc3_[i]->contract(reg);

      if (certif == Proof::Empty)
         return certif;      
      else
         proof = std::max(proof, certif);
   }

   return proof;
}

void BC4Contractor::print(std::ostream& os) const
{
   os << "BC4 contractor #" << if_;
}

} // namespace
