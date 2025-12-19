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
 * @file   Objective.hpp
 * @brief  Objective function
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_OBJECTIVE_HPP
#define REALPAVER_OBJECTIVE_HPP

#include "realpaver/Term.hpp"

namespace realpaver {

/// Shared representation of an objective function
class ObjectiveRep {
public:
   /**
    * @brief Constructor.
    *
    * Creates the objective function Min z = f(...) if minimization=true,
    * Max z = f(...) otherwise.
    */
   ObjectiveRep(Variable z, Term f, bool minimization);

   /// No copy
   ObjectiveRep(const ObjectiveRep &) = delete;

   /// No assignment
   ObjectiveRep &operator=(const ObjectiveRep &) = delete;

   /// Default destructor
   ~ObjectiveRep() = default;

   /// Returns the expression of this
   Term getTerm() const;

   /// Returns true if this is constant (variable free)
   bool isConstant() const;

   /// Returns true if v occurs in this
   bool dependsOn(const Variable &v) const;

   /// Returns true if this is linear
   bool isLinear() const;

   /// Returns true if this has to be minimized
   bool isMinimization() const;

   /// Returns true if this has to be maximized
   bool isMaximization() const;

   /// Returns the objective variable
   Variable getVar() const;

private:
   Term f_;            // objective function f(...)
   Variable z_;        // objective variable z = f(...)
   bool minimization_; // true for MIN, false for MAX
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Objective function.
 *
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class Objective {
public:
   /**
    * @brief Default constructor.
    *
    * This constructor is used when an Objective object must be initialized
    * without having an objective function. The resulting object obj is such
    * that obj.hasNullRep() == true.
    *
    * This constructor should not be used directly.
    */
   Objective();

   /// Default copy constructor
   Objective(const Objective &) = default;

   /// Default assignment operator
   Objective &operator=(const Objective &) = default;

   /// Default destructor
   ~Objective() = default;

   /// Returns the expression of this
   Term getTerm() const;

   /// Returns true if this is constant (variable free)
   bool isConstant() const;

   /// Returns true if v occurs in this
   bool dependsOn(const Variable &v) const;

   /// Returns true if this is linear
   bool isLinear() const;

   /// Returns true if this has to be minimized
   bool isMinimization() const;

   /// Returns true if this has to be maximized
   bool isMaximization() const;

   /// Returns the objective variable
   Variable getVar() const;

   /// Returns true if this has no representation
   bool hasNullRep() const;

   friend Objective minimize(Variable z, Term f);
   friend Objective maximize(Variable z, Term f);

private:
   std::shared_ptr<ObjectiveRep> rep_;

   Objective(Variable z, Term f, bool minimization);
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, Objective obj);

/**
 * @brief Creates an objective function.
 *
 * If f is a constant expression then an exception is thrown. Otherwise, it returns
 * the objective 'minimize z = f'
 */
Objective minimize(Variable z, Term f);

/**
 * @brief Creates an objective function.
 *
 * If f is a constant expression then an exception is thrown. Otherwise, it returns
 * the objective 'maximize z = f'
 */
Objective maximize(Variable z, Term f);

} // namespace realpaver

#endif
