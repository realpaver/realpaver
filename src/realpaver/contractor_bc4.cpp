// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/contractor_bc4.hpp"

namespace realpaver {

Bc4Contractor::Bc4Contractor(Dag* dag, size_t i) :
   f_(dag->fun(i)), hc4_(nullptr), bc3_()
{
   hc4_ = new Hc4Contractor(f_->dag(), f_->index());

   Scope s = f_->scope();
   for (auto v : s)
   {
      if (s.count(v) > 1)
         bc3_.push_back(new Bc3Contractor(f_->dag(), f_->index(), v.getId()));
   }
}

Bc4Contractor::~Bc4Contractor()
{
   delete hc4_;

   for (auto it : bc3_)
      delete it;
}

bool Bc4Contractor::dependsOn(const Bitset& bs) const
{
  return f_->dependsOn(bs); 
}

Proof Bc4Contractor::contract(Box& B)
{
   // HC4
   Hc4Contractor hc4(f_->dag(), f_->index());
   Proof proof = hc4.contract(B);

   if (proof != Proof::Maybe)
      return proof;

   // BC3
   for (size_t i=0; i<bc3_.size(); ++i)
   {
      Proof certif = bc3_[i]->contract(B);

      if (certif == Proof::Empty)
         return certif;      
      else
         proof = std::max(proof, certif);
   }

   return proof;
}

void Bc4Contractor::print(std::ostream& os) const
{
   os << "BC4 contractor " << f_->index();
}

} // namespace
