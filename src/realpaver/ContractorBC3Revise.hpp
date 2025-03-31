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
 * @file   ContractorBC3Revise.hpp
 * @brief  BC3Revise contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CONTRACTOR_BC3_REVISE_HPP
#define REALPAVER_CONTRACTOR_BC3_REVISE_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/IntervalNewtonUni.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/IntervalThickFunction.hpp"

namespace realpaver {

/**
 * @brief BC3Revise contractor implementing box consistency.
 *
 * It applies to a bounded thick interval function with form a <= F(x) <= b.
 * Given x in X, it finds the outermost consistent values by combining
 * search with an interval Newton method. It returns the interval [c, d]
 * such that c is the smallest value in X verifying a <= F(c) <= b and d
 * is the greatest value in X such that a <= F(d) <= b. If there is no
 * consistent value in X, it returns the empty set. This is the theory.
 *
 * In practice, a peel factor f in [0,100] is used to check the consistency of
 * small intervals at the bounds of domains during the search. Given an interval
 * [u, v], these small intervals are [u,u+w] and [v-w,v] with w = (f/100)*(v-u).
 * The search stops if these small intervals are consistent.
 */
class ContractorBC3Revise : public Contractor {
public:
   /// Contractor associated with the i-th function of a DAG and v
   ContractorBC3Revise(SharedDag dag, size_t i, Variable v);

   /// Destructor
   ~ContractorBC3Revise();

   /// No copy
   ContractorBC3Revise(const ContractorBC3Revise &) = delete;

   /// No assignment
   ContractorBC3Revise &operator=(const ContractorBC3Revise &) = delete;

   /// Returns the peel factor
   double getPeelFactor() const;

   /// Sets the peel factor
   void setPeelFactor(double f);

   /// Returns the maximum number of steps in the iterative method
   size_t getMaxIter() const;

   /// Sets the maximum number of steps in the iterative method
   void setMaxIter(size_t val);

   /// Returns the Newton operator enclosed
   IntervalNewtonUni *getNewton() const;

   Scope scope() const override;
   Proof contract(IntervalBox &B) override;
   void print(std::ostream &os) const override;

private:
   IntervalThickFunction f_;   // univariate thick interval function
   IntervalPeeler peeler_;     // peeling at interval bounds
   size_t maxiter_;            // maximum number of steps in shrink
   IntervalNewtonUni *newton_; // interval Newton method

   // split functions
   typedef bool (*SplitFun)(const Interval &x, Interval &x1, Interval &x2);
   static bool splitLeft(const Interval &x, Interval &x1, Interval &x2);
   static bool splitRight(const Interval &x, Interval &x1, Interval &x2);

   // peeling functions
   typedef void (*PeelFun)(const Interval &x, IntervalPeeler &peeler, Interval &b,
                           Interval &r);
   static void peelLeft(const Interval &x, IntervalPeeler &peeler, Interval &b,
                        Interval &r);
   static void peelRight(const Interval &x, IntervalPeeler &peeler, Interval &b,
                         Interval &r);

   // shrink functions
   Proof shrinkLeft(const Interval &x, Interval &res);
   Proof shrinkRight(const Interval &x, Interval &res);
   Proof shrink(const Interval &x, Interval &res, SplitFun split_fun, PeelFun peel_fun);

   // consistency checking
   Proof isConsistent(const Interval &x);
};

} // namespace realpaver

#endif
