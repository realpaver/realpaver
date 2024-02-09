///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_PROVER_HPP
#define REALPAVER_PROVER_HPP

#include "realpaver/IntervalNewton.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/IntervalNewtonUni.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This provides proof certificates for solutions.
///
/// Given a set of constraints {C1, ..., Cm} and a box B, the certification
/// process is as follows:
/// - 
///////////////////////////////////////////////////////////////////////////////
class Prover {
public:
   /// Constructor
   /// @param p a problem
   Prover(const Problem& p);

   /// Destructor
   ~Prover();

   /// No copy
   Prover(const Prover&) = delete;

   /// No asignment
   Prover& operator=(const Prover&) = delete;

   /// Certification method
   /// @param B a box
   /// @return a certificate of proof of B with respect to the constraints
   ///         of the given problem
   Proof certify(IntervalBox& B);

   /// Sets a limit of iterations of the Newton operator
   /// @param n new value of the limit
   void setMaxIter(size_t n);

   /// @return the maximum number of iterations of the Newton operator
   size_t getMaxIter() const;

   /// @return parameter delta of the inflation technique of the certification
   ///         technique of the Newton operator
   double getInflationDelta() const;

   /// Assigns the parameter delta of the inflation technique of the
   /// certification technique of the Newton operator
   /// @param val new value
   void setInflationDelta(const double& val);

   /// @return parameter chi of the inflation technique of the certification
   ///         technique of the Newton operator
   double getInflationChi() const;

   /// Assigns the parameter chi of the inflation technique of the certification
   /// technique of the Newton operator
   /// @param val new value
   void setInflationChi(const double& val);

private:
   struct Item {
      Constraint ctr;
      bool iseq;
      Proof proof;
   };
   std::vector<Item> v_;            // vector of constraints
   SharedDag dag_;                  // dag of equations
   IntervalNewton* mnewton_;        // multivariate Newton operator on the dag
   IntervalNewtonUni* unewton_;     // univariate Newton operator
   double delta_;                   // parameter delta of inflation
   double chi_;                     // parameter chi of inflation
   size_t maxiter_;                 // maximum number of iterations
};

} // namespace

#endif
