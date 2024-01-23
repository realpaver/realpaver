///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_SMEAR_SUM_REL_HPP
#define REALPAVER_INTERVAL_SMEAR_SUM_REL_HPP

#include "realpaver/IntervalFunctionVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This calculates the smear sum relative values of interval functions.
///
/// Let F(x) be a vector of functions obtained from all the numeric constraints
/// of a problem and let B be a box. We first calculate the real matrix S
/// such that sij is the smear value of xi in fj, which is equal to the product
/// of the width of the domain of xi in B and the magnitude of the interval
/// derivative of fj with respect to xi evaluated in B. Then S is normalized
/// by considering each row to derive the smear relative values 0 <= rij <= 1.0.
/// Then for each column these values are addded.
///
/// For example, let F = (f1, f2) and x = (x1, x2). Let the smear matrix be
///   S = (s11, s12)
///       (s21, s22)
///
/// Then S is normalized as
///   R = (s11 / (s11+s12), s12 / (s11+s12))
///       (s21 / (s21+s22), s22 / (s21+s22))
///
/// Then the sums are calculated by columns to derive
///   smearSumRel(x1) = s11 / (s11+s12) + s21 / (s21+s22)
///   smearSumRel(x2) = s12 / (s11+s12) + s22 / (s21+s22)
///
/// These values are stored in this. They can be accessed using getSSR(v).
///////////////////////////////////////////////////////////////////////////////
class IntervalSmearSumRel {
public:
   /// Constructor
   /// @param f a function vector
   IntervalSmearSumRel(IntervalFunctionVector F);

   /// Default destructor
   ~IntervalSmearSumRel() = default;

   /// Default copy constructor
   IntervalSmearSumRel(const IntervalSmearSumRel& other) = default;

   /// No assignment
   IntervalSmearSumRel& operator=(const IntervalSmearSumRel&) = delete;

   /// @return the scope of this
   Scope scope() const;

   /// @return the interval function vector of this
   IntervalFunctionVector getFun() const;

   /// Calculates the smearSumRel value of the variables in a box
   /// @param B an interval box
   void calculate(const IntervalBox& B);

   /// Sorts the variables by decreasing ordering of their smearSumRel values
   /// It lmust be done after a call to calculateSSR.
   void sort();

   /// Access to a variable
   /// @param i an index between 0 and n-1 where n is the number of variables
   /// @return the i-th variable
   ///
   /// After a call to calculate, the i-th variable corresponds to the i-th
   /// variable of the scope of the interval function vector.
   ///
   /// After a call to sort, the i-th variable is the variable having index i
   /// in the decreasing ordering of the smear sum rel values, in particular
   /// getVar(0) returns the variable having the maximum smear sum rel value.
   Variable getVar(size_t i) const;

   /// Access to a smear sum rel value
   /// @param i an index between 0 and n-1 where n is the number of variables
   /// @return the smear sum rel value of getVar(i)
   double getSmearSumRel(size_t i) const;

   /// @return the size of the scope of this
   size_t nbVars() const;

   /// @return the variable having the maximum smear sum rel value
   Variable getMaxVar() const;

private:
   struct Item {
      Variable var;  // variable
      double val;    // smearSumRel of var
   };

   struct CompItem {
      bool operator()(const Item itm1, const Item& itm2) const
      {
         return (itm1.val > itm2.val) ||
                (itm1.val == itm2.val && itm1.var.id() < itm2.var.id());
      }
   };

   IntervalFunctionVector F_;    // function vector
   Scope scop_;                  // scope of function vector
   std::vector<Item> ssr_;       // vector of smearSumRel values
                                 // ordered by the scope
};

} // namespace

#endif
