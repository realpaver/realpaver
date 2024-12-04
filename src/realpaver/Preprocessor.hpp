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
 * @date   2024-4-11
 */

#ifndef REALPAVER_PREPROCESSOR_HPP
#define REALPAVER_PREPROCESSOR_HPP

#include <unordered_set>
#include <vector>
#include "realpaver/ConstraintFixer.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Timer.hpp"

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
   /// Type of map Variable -> Variable
   using VarVarMapType = ConstraintFixer::VarVarMapType;

   /// Type of map Variable -> Interval
   using VarIntervalMapType = ConstraintFixer::VarIntervalMapType;

   /// Constructor
   Preprocessor();

   /// No copy
   Preprocessor(const Preprocessor&) = delete;

   /// No assignment
   Preprocessor& operator=(const Preprocessor&) = delete;

   /// Destructor
   ~Preprocessor();

   /**
    * @brief Creates a simplified problem from a source problem.
    * 
    * @param src source problem
    * @param dest simplified problem
    *
    * It removes the fixed variables and the inactive constraints and it
    * creates a new problem with a new set of variables.
    *
    * After a call to apply, it is possible to check if the input problem is
    * not feasible with the method isProblemUnfeasible().
    */
   void apply(const Problem& src, Problem& dest);

   /// Returns true if the domain of v is fixed
   bool hasFixedDomain(Variable v) const;

   /// Gets the domain of a fixed variable v of the source problem
   Interval getFixedDomain(Variable v) const;

   /// Gets a variable of the new problem that is associated with v
   Variable srcToDestVar(Variable v) const;

   /// Returns true if all the variables from the source problem are fixed
   bool allVarsFixed() const;

   /// Returns the fixed variables of the source problem
   Scope fixedScope() const;

   /// Returns the unfixed variables of the source problem
   Scope unfixedScope() const;

   /**
    * @brief Returns the (unfixed) variables of the destination problem.
    *
    * It corresponds to unfixedScope() mapped to the variables of the
    * destination problem
    */
   Scope destScope() const;

   /// Returns the number of inactive constraints detected by this
   size_t nbInactiveCtrs() const;

   /// Gets the i-th inactive constraint in this
   Constraint getInactiveCtr(size_t i) const;

   /// Returns the number of fixed variables detected by this
   size_t nbFixedVars() const;

   /// Returns the number of unfixed variables after preprocessing
   size_t nbUnfixedVars() const;

   /// Gets the i-th fixed variable of the source problem
   Variable getFixedVar(size_t i) const;

   /// Gets the i-th unfixed variable of the source problem
   Variable getUnfixedVar(size_t i) const;

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

   /// Returns true if the source problem is not feasible
   bool isUnfeasible() const;

   /// Returns true if the source problem is solved by this
   bool isSolved() const;

   /// Returns the elapsed time of the preprocessing stage in seconds
   double elapsedTime() const;

private:
   VarVarMapType vvm_;                 // map for non fixed variables
   VarIntervalMapType vim_;            // map for fixed variables

   DomainBox* box_;                    // box used for propagation

   std::vector<Constraint> inactive_;  // constraints inactive
   std::vector<Constraint> active_;    // constraints not inactive
   bool unfeasible_;                   // true if problem unfeasible

   Timer timer_;

   bool occursInActiveConstraint(const Variable& v) const;
   bool propagate(const Problem& problem, DomainBox& box);
   void applyImpl(const Problem& src, Problem& dest);
};

} // namespace

#endif
