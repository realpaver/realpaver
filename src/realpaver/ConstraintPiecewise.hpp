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
 * @file   ConstraintPiecewise.hpp
 * @brief  Piecewise constraints
 * @author Raphaël Chenouard
 * @date   29 May 2025
 */

#ifndef REALPAVER_CONSTRAINT_PIECEWISE_HPP
#define REALPAVER_CONSTRAINT_PIECEWISE_HPP

#include "realpaver/Constraint.hpp"

namespace realpaver {

/**
 * @brief Representation of a piecewise constraint
 * {(v in [a1,b1] <=> z1) -> body1,..., (v in [an,bn] <=> zn) -> bodyn}.
 *
 * A reference variable v must be in an interval to activate the body constraint.
 * The body is any constraint.
 * An intermediary binary variable zi is used to assess the right case.
 *
 * A single case can and must be activated at the same time.
 */
class ConstraintPiecewise : public ConstraintRep {
public:
   /// Creates a piecewise constraint for v
   ConstraintPiecewise(Variable v);

   /// Default copy constructor
   ConstraintPiecewise(ConstraintPiecewise &) = default;

   /// No assignment
   ConstraintPiecewise &operator=(ConstraintPiecewise &) = delete;

   /// Defaut destructor
   ~ConstraintPiecewise() = default;

   /// Inserts a piece guard -> body with binary variable b
   void addPiece(Variable b, const Interval &guard, Constraint body);

   /// Returns the main variable used as guard
   const Variable &variable() const;

   /// Returns the i-th binary variable used as a guard
   const Variable &binary(size_t i) const;

   /// Returns the i-th interval used as a guard
   const Interval &interval(size_t i) const;

   /// Returns the i-th constraint
   const Constraint &constraint(size_t i) const;

   /// Returns the number of pieces
   size_t nbPieces() const;

   bool isConstant() const override;
   bool isAnIntegerConstraint() const override;
   Proof isSatisfied(const IntervalBox &B) override;
   double violation(const IntervalBox &B) override;
   Proof contract(IntervalBox &B) override;
   Proof isSatisfied(const DomainBox &box) override;
   double violation(const DomainBox &box) override;
   Proof contract(DomainBox &box) override;
   void print(std::ostream &os) const override;
   void acceptVisitor(ConstraintVisitor &vis) const override;
   ConstraintRep *cloneRoot() const override;

   /// Returns the name of the next generated binary variable
   static std::string getNextBinaryName();

private:
   Variable v_;                   // main variable
   std::vector<Variable> bins_;   // binary variables relating intervals activation
   std::vector<Interval> itvs_;   // intervals used to activate constraints
   std::vector<Constraint> ctrs_; // constraints to apply
   Interval guard_hull_;          // hull of intervals used as guard

   // satisfaction test of one piece
   Proof isPieceSatisfied(const IntervalBox &B, const Interval &iv, size_t i);
   Proof isPieceSatisfied(const DomainBox &box, const Interval &iv, size_t i);

   // checks the guards and assigns counters
   void checkActive(const DomainBox &B, size_t &nb_active, size_t &nb_possibly_active,
                    size_t &num_active) const;
   void checkActive(const IntervalBox &B, size_t &nb_active, size_t &nb_possibly_active,
                    size_t &num_active) const;

   static size_t nb_bins_;                  // counts the number of binary variables
   static const std::string binary_prefix_; // prefix of binary variables
};

class Problem;

/**
 * @brief Generates a piecewise constraint.
 *
 * The piecewise constraint {(v in [a1,b1] <=> z1) -> body1,..., (v in [an,bn] <=> zn) ->
 * bodyn} is created from the main variable v, the list guards = ([a1,b1], ..., [an,bn]),
 * the list bodies = (body1, ..., bodyn), and the problem pb. The binary variables z1,
 * ..., zn are generated from pb.
 */
Constraint piecewise(Variable v, const std::initializer_list<Interval> &guards,
                     const std::initializer_list<Constraint> &bodies, Problem &pb);

/**
 * @brief Generates a piecewise constraint.
 *
 * The piecewise constraint {(v in [a1,b1] <=> z1) -> body1,..., (v in [an,bn] <=> zn) ->
 * bodyn} is created from the main variable v, the vector guards = ([a1,b1], ...,
 * [an,bn]), the vector bodies = (body1, ..., bodyn), and the problem pb. The binary
 * variables z1, ..., zn are generated from pb.
 */
Constraint piecewise(Variable v, const std::vector<Interval> &guards,
                     const std::vector<Constraint> &bodies, Problem &pb);

/**
 * @brief Generates a piecewise constraint.
 *
 * The piecewise constraint {(v in [a1,b1] <=> z1) -> body1,..., (v in [an,bn] <=> zn) ->
 * bodyn} is created from the main variable v, the vector bins = (z1, ..., zn), the vector
 * guards = ([a1,b1], ..., [an,bn]), and the vector bodies = (body1, ..., bodyn).
 */
Constraint piecewise(Variable v, const std::vector<Variable> &bins,
                     const std::vector<Interval> &guards,
                     const std::vector<Constraint> &bodies);

/**
 * @brief Generates a piecewise constraint.
 *
 * The piecewise constraint {(v in [a1,b1] <=> z1) -> body1,..., (v in [an,bn] <=> zn) ->
 * bodyn} is created from the main variable v, the list bins = (z1, ..., zn), the list
 * guards = ([a1,b1], ..., [an,bn]), and the list bodies = (body1, ..., bodyn).
 */
Constraint piecewise(Variable v, const std::initializer_list<Variable> &bins,
                     const std::initializer_list<Interval> &guards,
                     const std::initializer_list<Constraint> &bodies);

} // namespace realpaver

#endif
