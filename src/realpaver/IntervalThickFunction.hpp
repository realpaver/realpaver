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
 * @file   IntervalThickFunction.hpp
 * @brief  Thick interval function
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_INTERVAL_THICK_FUNCTION_HPP
#define REALPAVER_INTERVAL_THICK_FUNCTION_HPP

#include "realpaver/Dag.hpp"
#include "realpaver/FlatFunction.hpp"
#include "realpaver/IntervalFunctionUni.hpp"

namespace realpaver {

/**
 * @brief Thick interval function.
 *
 * Given a function f(x1, ..., xn) in a DAG, a variable xi and a box B,
 * the thick function is an univariate function obtained from f by replacing
 * each variable xj but xi by its domain in B.
 *
 * The update method must be called first. Then the evaluation and
 * differentiation methods can be called.
 */
class IntervalThickFunction : public IntervalFunctionUni {
public:
   /**
    * @brief Constructor.
    *
    * Creates a thick function from the i-th function of a dag with v as
    * argument.
    */
   IntervalThickFunction(SharedDag dag, size_t i, Variable v);

   /// Default copy constructor
   IntervalThickFunction(const IntervalThickFunction &) = default;

   /// No assignment
   IntervalThickFunction &operator=(const IntervalThickFunction &) = delete;

   /// Destructor
   ~IntervalThickFunction();

   /**
    * @brief Creates the univariate function.
    *
    * A flat function is created such that the sub-terms that do not depend on
    * the variable enclosed in this are evaluated once.
    */
   void update(const IntervalBox &B);

   /// Returns the evaluation of this on x
   Interval eval(const Interval &x) override;

   /// Returns the derivative of this on x
   Interval diff(const Interval &x) override;

   /// Returns the variable enclosed in this
   Variable getVar() const;

   /// Returns the function in the dag
   DagFun *getFun() const;

private:
   DagFun *f_;          // function in the dag
   Variable v_;         // variable of this univariate function
   FlatFunction *flat_; // flat function
   IntervalBox B_;      // auxiliary box used to evaluate the function
   IntervalVector G_;   // auxiliary gradient vector
};

} // namespace realpaver

#endif
