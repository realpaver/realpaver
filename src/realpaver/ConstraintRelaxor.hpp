///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONSTRAINT_RELAXOR_HPP
#define REALPAVER_CONSTRAINT_RELAXOR_HPP

#include "realpaver/Constraint.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor used to relax a constraint.
///
/// Given a real number nu>0, every equation f(x) = 0 is relaxed as
/// f(x) in [-nu, +nu]. Every other constraint is not handled and the apply
/// method must raise an exception.
///////////////////////////////////////////////////////////////////////////////
class ConstraintRelaxor : public ConstraintVisitor {
public:
   /// Creates a visitor
   /// @param nu relaxation value
   ConstraintRelaxor(double nu);

   /// Default copy constructor
   ConstraintRelaxor(const ConstraintRelaxor&) = default;

   /// No assignment
   ConstraintRelaxor& operator=(const ConstraintRelaxor&) = delete;

   /// Default destructor
   ~ConstraintRelaxor() = default;

   ///@{
   /// Overrides
   void apply(const ConstraintEq* c);
   ///@}

   /// @return the relaxed constraint after a visit
   Constraint getRelaxedCtr() const;

private:
   double nu_;
   Constraint relaxed_;
};

} // namespace

#endif
