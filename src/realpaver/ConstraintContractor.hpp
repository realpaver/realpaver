///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONSTRAINT_CONTRACTOR_HPP
#define REALPAVER_CONSTRAINT_CONTRACTOR_HPP

#include "realpaver/Constraint.hpp"
#include "realpaver/Contractor.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor associated with a constraint.
///
/// This contractor simply calls the default contraction of the constraint.
///////////////////////////////////////////////////////////////////////////////
class ConstraintContractor : public Contractor {
public:
   /// Constructor
   /// @param c constraint of this
   ConstraintContractor(Constraint c);

   /// Default copy constructor
   ConstraintContractor(const ConstraintContractor&) = default;

   /// No assignment
   ConstraintContractor& operator=(const ConstraintContractor&) = delete;

   /// Default destructor
   ~ConstraintContractor() = default;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalRegion& reg) override;
   void print(std::ostream& os) const override;
   ///@}

   /// @return the constraint enclosed in this
   Constraint getConstraint() const;

private:
   Constraint c_;
};

} // namespace

#endif
