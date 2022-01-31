// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONTRACTOR_INT_HPP
#define REALPAVER_CONTRACTOR_INT_HPP

#include <vector>
#include "realpaver/contractor.hpp"
#include "realpaver/Variable.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of contractors for integral variables.
 *****************************************************************************/
class IntContractor : public Contractor {
public:
   // constructors
   IntContractor();
   IntContractor(const Variable& v);
   IntContractor(const std::initializer_list<Variable>& l);

   // insertion of an integral variable
   void insert(const Variable& v);

   // Override
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(Box& B);
   void print(std::ostream& os) const;

private:
   Scope s_;
   Bitset b_;
};

inline Scope IntContractor::scope() const
{
   return s_;
}

} // namespace

#endif
