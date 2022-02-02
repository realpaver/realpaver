// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONTRACTOR_BC4_HPP
#define REALPAVER_CONTRACTOR_BC4_HPP

#include <vector>
#include "realpaver/contractor_bc3.hpp"
#include "realpaver/contractor_hc4.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of contractors implementing the BC4Revise operator.
 *****************************************************************************/
class Bc4Contractor : public Contractor {
public:
   // constructor of a BC4 contractor associated with the i-th function
   // of the given dag, 0 <= i < dag->nbFun()
   // this contractor does not own the dag
   Bc4Contractor(Dag* dag, size_t i);

   // destructor
   ~Bc4Contractor();

   // copy protection
   Bc4Contractor(const Bc4Contractor&) = delete;
   Bc4Contractor& operator=(const Bc4Contractor&) = delete;

   // Override
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalVector& X);
   void print(std::ostream& os) const;

private:
   DagFun* f_;                         // function in a dag
   Hc4Contractor* hc4_;                // hc4 contractor associated with f_
   std::vector<Bc3Contractor*> bc3_;   // bc3 contractors associated with
                                       // the variables having multiple
                                       // occurrences in f_
};

inline Scope Bc4Contractor::scope() const
{
   return f_->scope();
}

} // namespace

#endif
