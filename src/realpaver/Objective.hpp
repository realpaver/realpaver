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
 * @date   2024-4-11
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
    * @param f expression of the function
    * @param minimization sense
    *        - true for minimization
    *        - false for maximization
    */
   ObjectiveRep(Term f, bool minimization);

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

private:
   Term f_;
   bool minimization_;
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

   friend Objective MIN(Term f);
   friend Objective MAX(Term f);

private:
   std::shared_ptr<ObjectiveRep> rep_;

   Objective(Term f, bool minimization);
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, Objective obj);

/// Returns the objective: minimize f
Objective MIN(Term f);

/// Returns the objective: maximize f
Objective MAX(Term f);

} // namespace realpaver

#endif
