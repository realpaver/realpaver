// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_THICK_FUN_HPP
#define REALPAVER_THICK_FUN_HPP

#include "realpaver_dag.hpp"
#include "realpaver_uni_fun.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of thick univariate functions created from  DAG.
 *****************************************************************************/
class ThickFun : public UniFun {
public:
   // constructor of a thick function associated with the i-th function
   // of the given dag and the variable of index iv
   ThickFun(Dag* dag, size_t i, size_t iv);

   // returns the function in the dag
   DagFun* getFun() const;

   // returns true if this function depends on a bitset bs
   bool dependsOn(const Bitset& bs) const;

   // Override
   Interval eval(const Interval& x);
   Interval diff(const Interval& x);
   std::pair<Interval,Interval> evalDiff(const Interval& x);

   // fixes all the variables to their domains from B but the variable
   // involved in this and returns the interval evaluation of this on B
   Interval update(const Box& B);

   // returns the variable index of this
   size_t getVarIndex() const;

private:
   DagFun* f_;    // function in the dag
   size_t if_;    // function index in the dag
   size_t iv_;    // variable index
};

inline DagFun* ThickFun::getFun() const
{
   return f_;
}

inline bool ThickFun::dependsOn(const Bitset& bs) const
{
   return f_->dependsOn(bs);
}

} // namespace

#endif
