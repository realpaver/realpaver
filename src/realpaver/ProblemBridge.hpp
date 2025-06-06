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
 * @file   ProblemBridge.hpp
 * @brief  Bridge between problems managed by the preprocessor
 * @author Laurent Granvilliers
 * @date   3 june 2025
 */

#ifndef REALPAVER_PROBLEM_BRIDGE_HPP
#define REALPAVER_PROBLEM_BRIDGE_HPP

#include "realpaver/Problem.hpp"
#include <memory>
#include <unordered_map>

namespace realpaver {

class ProblemBridge {
public:
   /// Creates a bridge between src and dest
   ProblemBridge(Problem &src, Problem &dest);

   /// No copy
   ProblemBridge(const ProblemBridge &) = delete;

   /// No assignment
   ProblemBridge &operator=(const ProblemBridge &) = delete;

   /// Destructor
   ~ProblemBridge();

   /// Returns true if all the variables from the source problem are fixed
   bool allVarsFixed() const;

   /// Returns true if the domain of v is fixed
   bool isFixed(const Variable &v) const;

   /// Returns true if the domain of v is not fixed
   bool isUnfixed(const Variable &v) const;

   /// Gets the domain of a fixed variable v of the source problem
   Interval getFixedDomain(const Variable &v) const;

   /// Gets a variable of the new problem that is associated with v
   Variable srcToDestVar(const Variable &v) const;

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

   /// Returns the number of fixed variables
   size_t nbFixedVars() const;

   /// Returns the number of unfixed variables
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

   /**
    * @brief Inserts an unfixed variable of the source problem.
    *
    * Createsa clone of v in the destination problem and returns it.
    */
   Variable insertVar(Variable v, const Domain &dom);

   /**
    * @brief Inserts a fixed variable.
    *
    * The variable v is a variable of the source problem. The domain dom
    * is its canonical domain.
    */
   void insertFixed(Variable v, const Domain &dom);

   /// Returns the domains of the source problem
   DomainBox &srcRegion();

   /// Removes v as a fixed variable
   void removeFixedVar(const Variable &v);

private:
   /// Map type Variable -> Variable
   using VarMap = std::unordered_map<Variable, Variable, VariableHasher>;

   /// Map type Variable -> Interval
   using FixedMap = std::unordered_map<Variable, Interval, VariableHasher>;

   Problem *src_;    // source problem
   DomainBox *box_;  // domains of the source problems
   Problem *dest_;   // destination problem
   VarMap varmap_;   // map Variable of src -> Variable of dest
   FixedMap fixmap_; // map Variable of src -> Interval (its fixed value)
};

/// Shared pointer to problem bridge
using SharedProblemBridge = std::shared_ptr<ProblemBridge>;

} // namespace realpaver

#endif
