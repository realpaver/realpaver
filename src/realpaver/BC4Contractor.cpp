///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BC4Contractor.hpp"

namespace realpaver {

BC4Contractor::BC4Contractor(Dag* dag, size_t i)
      : f_(dag->fun(i)),
        hc4_(nullptr),
        bc3_()
{
   hc4_ = new HC4Contractor(f_->dag(), f_->index());

   Scope s = f_->scope();
   for (auto v : s)
   {
      if (s.count(v) > 1)
         bc3_.push_back(new BC3Contractor(f_->dag(), f_->index(), v.getId()));
   }
}

BC4Contractor::~BC4Contractor()
{
   delete hc4_;

   for (auto it : bc3_) delete it;
}

Scope BC4Contractor::scope() const
{
   return f_->scope();
}

bool BC4Contractor::dependsOn(const Bitset& bs) const
{
  return f_->dependsOn(bs); 
}

Proof BC4Contractor::contract(IntervalVector& X)
{
   // HC4
   HC4Contractor hc4(f_->dag(), f_->index());
   Proof proof = hc4.contract(X);

   if (proof != Proof::Maybe) return proof;

   // BC3
   for (size_t i=0; i<bc3_.size(); ++i)
   {
      Proof certif = bc3_[i]->contract(X);

      if (certif == Proof::Empty)
         return certif;      
      else
         proof = std::max(proof, certif);
   }

   return proof;
}

void BC4Contractor::print(std::ostream& os) const
{
   os << "BC4 contractor #" << f_->index();
}

} // namespace
