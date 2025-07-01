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
 * @file   Preprocessor.hpp
 * @brief  Preprocessor of problems
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_PREPROCESSOR_HPP
#define REALPAVER_PREPROCESSOR_HPP

#include "realpaver/Problem.hpp"
#include "realpaver/ProblemBridge.hpp"
#include "realpaver/Timer.hpp"
#include <memory>
#include <vector>

namespace realpaver {

/**
 * @brief Preprocessor of problems.
 *
 * It simplifies a problem by removing the fixed variables and the inactive
 * constraints. A new problem is created with a new set of variables. To this
 * end, two maps are maintained. The first one maps a variable of the source
 * problem to the corresponding variable in the new problem. The second one maps
 * a variable of the source problem to its value (it contains the fixed
 * variable).
 */
class Preprocessor {
public:
   /// Constructor given the source problem
   Preprocessor(SharedProblem src);

   /// No copy
   Preprocessor(const Preprocessor &) = delete;

   /// No assignment
   Preprocessor &operator=(const Preprocessor &) = delete;

   /// Default destructor
   ~Preprocessor() = default;

   /**
    * @brief Creates a simplified problem from a source problem.
    *
    * If the boolean on is false, then the simplified problem corresponds to the source.
    * Otherwise, the fixed variables and the inactive constraints are removed and a new
    * problem with a new set of variables is created.
    *
    * After a call to apply, it is possible to check if the input problem is
    * not feasible with the method isProblemUnfeasible().
    */
   void apply(bool on = true);

   /// Returns the number of inactive constraints detected by this
   size_t nbInactiveCtrs() const;

   /// Gets the i-th inactive constraint in this
   Constraint getInactiveCtr(size_t i) const;

   /// Returns true if the source problem is not feasible
   bool isUnfeasible() const;

   /// Returns true if the source problem is solved by this
   bool isSolved() const;

   /// Returns the elapsed time of the preprocessing stage in seconds
   double elapsedTime() const;

   /// Returns true if all the variables from the source problem are fixed
   bool allVarsFixed() const;

   /// Gets a variable of the new problem that is associated with v
   Variable srcToDestVar(const Variable &v) const;

   /// Returns the number of fixed variables
   size_t nbFixedVars() const;

   /// Returns the number of unfixed variables
   size_t nbUnfixedVars() const;

   /// Gets the i-th fixed variable of the source problem
   Variable getFixedVar(size_t i) const;

   /// Gets the i-th unfixed variable of the source problem
   Variable getUnfixedVar(size_t i) const;

   /// Gets the domain of a fixed variable v of the source problem
   Interval getFixedDomain(const Variable &v) const;

   /// Returns true if the domain of v is fixed
   bool isFixed(const Variable &v) const;

   /// Returns true if the domain of v is not fixed
   bool isUnfixed(const Variable &v) const;

   /**
    * @brief Returns the domains of the fixed variables if nbFixedVars() > 0.
    *
    * The scope of this region is equal to fixedScope() having only variables
    * of the source problem.
    */
   DomainBox fixedRegion() const;

   /**
    * @brief Returns the domains of the non fixed variables.
    *
    * The scope of this region is the scope of the destination problem.
    */
   DomainBox destRegion() const;

   /**
    * @brief Returns the (unfixed) variables of the destination problem.
    *
    * It corresponds to unfixedScope() mapped to the variables of the
    * destination problem
    */
   Scope destScope() const;

   /// Returns the fixed variables of the source problem
   Scope fixedScope() const;

   /// Returns the unfixed variables of the source problem
   Scope unfixedScope() const;

   /// Display on a stream after preprocessing
   void print(std::ostream &os) const;

   /// Returns the source problem
   SharedProblem srcProblem() const;

   /// Returns the target problem
   SharedProblem destProblem() const;

private:
   SharedProblem src_;                // problem before preprocessing
   SharedProblem dest_;               // problem after preprocessing
   SharedProblemBridge bridge_;       // bridge between these two problems
   std::vector<Constraint> inactive_; // constraints inactive
   std::vector<Constraint> active_;   // constraints not inactive
   bool unfeasible_;                  // true if problem unfeasible
   Timer timer_;                      // timer

   bool occursInActiveConstraint(const Variable &v) const;
   bool propagate(DomainBox &box);
   void applyImpl();
};

/// Type of shared pointers on a preprocessor
using SharedPreprocessor = std::shared_ptr<Preprocessor>;

} // namespace realpaver

#endif
