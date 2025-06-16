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
 * @file   SelectorSSR.hpp
 * @brief  Variable selection strategy Smear Sum Relative
 * @author Laurent Granvilliers
 * @date   25 Apr 2025
 */

#ifndef REALPAVER_SELECTOR_SSR_HPP
#define REALPAVER_SELECTOR_SSR_HPP

#include "realpaver/IntervalFunctionVector.hpp"
#include "realpaver/Selector.hpp"

namespace realpaver {

/**
 * @brief Variable selection strategy Smear Sum Relative.
 *
 * It calculates the smear sum relative values of interval functions.
 *
 * Let F(x) be a vector of functions obtained from all the numeric constraints
 * of a problem and let B be a box. We first calculate the real matrix S
 * such that sij is the smear value of xi in fj, which is equal to the product
 * of the width of the domain of xi in B and the magnitude of the interval
 * derivative of fj with respect to xi evaluated in B. Then S is normalized
 * by considering each row to derive the smear relative values 0 <= rij <= 1.0.
 * Then for each column these values are addded.
 *
 * For example, let F = (f1, f2) and x = (x1, x2). Let the smear matrix be
   @verbatim
     S = (s11, s12)
         (s21, s22)
   @endverbatim
 *
 * Then S is normalized as
   @verbatim
     R = (s11 / (s11+s12), s12 / (s11+s12))
         (s21 / (s21+s22), s22 / (s21+s22))
   @endverbatim
 *
 * Then the sums are calculated by columns to derive
   @verbatim
     smearSumRel(x1) = s11 / (s11+s12) + s21 / (s21+s22)
     smearSumRel(x2) = s12 / (s11+s12) + s22 / (s21+s22)
   @endverbatim
 *
 * These values are stored in this. They can be accessed using getSSR(v).
 *
 * The variables that are not involved in numerical constraints are handled by the
 * Largest-First strategy.
 */
class SelectorSSR : public Selector {
public:
   /// Creates a selector on a function vector
   SelectorSSR(IntervalFunctionVector F);

   /// Default destructor
   ~SelectorSSR() = default;

   /// Default copy constructor
   SelectorSSR(const SelectorSSR &) = default;

   /// No assignment
   SelectorSSR &operator=(const SelectorSSR &) = delete;

   /// Returns the interval function vector of this
   IntervalFunctionVector getFun() const;

   /// Calculates the smearSumRel values of the variables in a box
   void calculate(const IntervalBox &B);

   /**
    * @brief Sorting method.
    *
    * Sorts the variables by decreasing ordering of their smearSumRel values.
    * It must be done after a call to calculate().
    */
   void sort();

   /**
    * @brief Access to the i-th variable.
    *
    *  After a call to calculate, the i-th variable corresponds to the i-th
    *  variable of the scope of the interval function vector.
    *
    *  After a call to sort, the i-th variable is the variable having index i
    *  in the decreasing ordering of the smear sum rel values, in particular
    *  getVar(0) returns the variable having the maximum smear sum rel value.
    */
   Variable getVar(size_t i) const;

   /// Returns the smear sum rel value of getVar(i)
   double getSmearSumRel(size_t i) const;

   /// Returns the size of the scope of this
   size_t nbVars() const;

   /// Returns the variable having the maximum smear sum rel value
   Variable getMaxVar() const;

   /// Output on a stream
   void print(std::ostream &os) const;

   bool apply(const DomainBox &box) override;

private:
   struct Item {
      Variable var; // variable
      double val;   // smearSumRel of var
   };

   struct CompItem {
      bool operator()(const Item itm1, const Item &itm2) const
      {
         return (itm1.val > itm2.val) ||
                (itm1.val == itm2.val && itm1.var.id() < itm2.var.id());
      }
   };

   IntervalFunctionVector F_; // function vector
   std::vector<Item> ssr_;    // vector of smearSumRel values
                              // ordered by the scope
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const SelectorSSR &sel);

} // namespace realpaver

#endif
