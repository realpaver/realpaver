/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   ConstraintRelaxor.hpp
 * @brief  Relaxation of equations
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#ifndef REALPAVER_CONSTRAINT_RELAXOR_HPP
#define REALPAVER_CONSTRAINT_RELAXOR_HPP

#include "realpaver/Constraint.hpp"

namespace realpaver {

/**
 * @brief Visitor used to relax a constraint
 * 
 * Given a real number nu>0, every equation f(x) = 0 is relaxed as
 * f(x) in [-nu, +nu]. Every other constraint is not handled and the apply
 * method must raise an exception.
 */
class ConstraintRelaxor : public ConstraintVisitor {
public:
   /**
    * @brief Constructor
    * @param nu relaxation value
    */
   ConstraintRelaxor(double nu);

   /** @brief Default copy constructor */
   ConstraintRelaxor(const ConstraintRelaxor&) = default;

   /** @brief No assignment */
   ConstraintRelaxor& operator=(const ConstraintRelaxor&) = delete;

   /** @brief Default destructor */
   ~ConstraintRelaxor() = default;

   void apply(const ArithCtrEq* c) override;

   /** @return the relaxed constraint after a visit */
   Constraint getRelaxedCtr() const;

private:
   double nu_;             // relaxation value
   Constraint relaxed_;    // new constraint after a visit
};

} // namespace

#endif
