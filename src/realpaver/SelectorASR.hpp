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
 * @file   SelectorASR.hpp
 * @brief  Variable selection strategy Affine Sum Relative
 * @author Laurent Granvilliers
 * @date   25 Apr 2025
 */

#ifndef REALPAVER_SELECTOR_ASR_HPP
#define REALPAVER_SELECTOR_ASR_HPP

#include "realpaver/Dag.hpp"
#include "realpaver/Selector.hpp"

namespace realpaver {

class SelectorASR : public Selector {
public:
   /// Creates a selector on a DAG
   SelectorASR(SharedDag dag);

   /// Default destructor
   ~SelectorASR() = default;

   /// Default copy constructor
   SelectorASR(const SelectorASR &) = default;

   /// No assignment
   SelectorASR &operator=(const SelectorASR &) = delete;

   /// Returs the DAG
   SharedDag getDag() const;

   /// Calculates the affineSumRel values of the variables in a box
   void calculate(const IntervalBox &B);

   /**
    * @brief Sorting method.
    *
    * Sorts the variables by decreasing ordering of their affineSumRel values.
    * It must be done after a call to calculate.
    */
   void sort();

   /**
    * @brief Access to the i-th variable.
    *
    *  After a call to calculate, the i-th variable corresponds to the i-th
    *  variable of the scope of the interval function vector.
    *
    *  After a call to sort, the i-th variable is the variable having index i
    *  in the decreasing ordering of the affine sum rel values, in particular
    *  getVar(0) returns the variable having the maximum affine sum rel value.
    */
   Variable getVar(size_t i) const;

   /// Returns the affine sum rel value of getVar(i)
   double getAffineSumRel(size_t i) const;

   /// Returns the size of the scope of this
   size_t nbVars() const;

   /// Returns the variable having the maximum affine sum rel value
   Variable getMaxVar() const;

   /// Output on a stream
   void print(std::ostream &os) const;

   bool apply(const DomainBox &box) override;

private:
   struct Item {
      Variable var; // variable
      double val;   // affineSumRel of var
   };

   struct CompItem {
      bool operator()(const Item itm1, const Item &itm2) const
      {
         return (itm1.val > itm2.val) ||
                (itm1.val == itm2.val && itm1.var.id() < itm2.var.id());
      }
   };

   SharedDag dag_;         // DAG of functions
   std::vector<Item> asr_; // vector of affineSumRel values ordered by the scope
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const SelectorASR &sel);

} // namespace realpaver

#endif
