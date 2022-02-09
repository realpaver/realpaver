// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONTRACTOR_BCO_HPP
#define REALPAVER_CONTRACTOR_BCO_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/dag.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of contractors used in Bound-Constrained Optimization.
 * Assumes that we have to minimize the objective function.
 * 
 * Given a function f and a variable v, a BCO contractor reduces a box
 * with respect to the constraint df / dv = 0, i.e. it aims at finding
 * stationary points but it also checks the bounds of the initial region.
 *****************************************************************************/
class BcoContractor : public Contractor {
public:
   // constructor
   BcoContractor(Dag* dag, size_t i, const Variable& v,
                 const SharedContractor& op, const SharedIntervalVector& init);

   // Override
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalVector& X);
   void print(std::ostream& os) const;

private:
   DagFun* f_;             // df/dv in a dag
   Variable v_;            // the considered variable
   SharedContractor op_;   // contractor for the equation df/dv = 0
   SharedIntervalVector init_;        // initial region
};

} // namespace

#endif
