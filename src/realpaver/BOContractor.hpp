///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BOCONTRACTOR_HPP
#define REALPAVER_BOCONTRACTOR_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/dag.hpp"

namespace realpaver {
   
   // TODO : Assumes that we have to minimize the objective function

///////////////////////////////////////////////////////////////////////////////
/// This is contractor for bound-constrained optimization problems.
///
/// Given a function f and a variable v, a BO contractor reduces a box with
/// respect to the constraint df / dv = 0, i.e. it aims at finding stationary
/// points but it also checks the bounds of the initial region.
///////////////////////////////////////////////////////////////////////////////
class BOContractor : public Contractor {
public:
   /// Creates a contractor associated with a partial derivative df/dv
   /// @param dag dag representong a BO problem
   /// @param i index of the partial derivative in the dag
   /// @param v variable considered
   /// @param op contractor for the equation df/dv = 0
   /// @param init initial region of the BO problem
   BOContractor(Dag* dag, size_t i, const Variable& v,
                const SharedContractor& op, const SharedIntervalVector& init);

   /// Default copy constructor
   BOContractor(const BOContractor&) = default;

   /// No assignment
   BOContractor& operator=(const BOContractor&) = delete;

   /// Default destructor
   ~BOContractor() = default;

   ///@{
   /// Overrides the methods of Contractor
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(IntervalVector& X);
   void print(std::ostream& os) const;
   ///@}

private:
   DagFun* f_;                   // derivative df/dv in a dag
   Variable v_;                  // the considered variable
   SharedContractor op_;         // contractor for the equation df/dv = 0
   SharedIntervalVector init_;   // initial region
};

} // namespace

#endif
