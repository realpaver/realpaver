// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONTRACTOR_HC4_HPP
#define REALPAVER_CONTRACTOR_HC4_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/dag.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of contractors implementing the HC4Revise operator.
 *****************************************************************************/
class Hc4Contractor : public Contractor {
public:
   // constructor of an HC4 contractor associated with the i-th function
   // of the given dag, 0 <= i < dag->nbFun()
   // this contractor does not own the dag
   Hc4Contractor(Dag* dag, size_t i);

   // Override
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalVector& X);
   void print(std::ostream& os) const;

   // returns the function enclosed
   DagFun* getFun() const;

   // returns the dag
   Dag* getDag() const;

private:
   DagFun* f_;
};

inline DagFun* Hc4Contractor::getFun() const
{
   return f_;
}

inline Dag* Hc4Contractor::getDag() const
{
   return f_->dag();
}

inline Scope Hc4Contractor::scope() const
{
   return f_->scope();
}

} // namespace

#endif
