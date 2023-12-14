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
   /// @param box a box
   /// @return a certificate of proof of the box with respect to the constraints
   ///         of the given problem
   Proof certify(IntervalBox& box);

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

   /// @return the tolerance on the distance between two consecutive boxes
   ///         in the certification technique of the Newton operator
   Tolerance getDTol() const;

   /// Sets the tolerance on the distance between two consecutive regions
   /// in the certification technique of the Newton operator
   /// @param tol absolute or relative tolerance
   ///
   /// The iteration stops if two consecutive regions are far enough, i.e.
   /// the distance between them is greater than tol
   void setDTol(const Tolerance& tol);

private:
   struct Item {
      Constraint ctr;
      bool iseq;
      Proof proof;
   };

   std::vector<Item> v_;

   IntervalNewton* mnewton_;        // multivariate Newton operator
   double delta_;                   // parameter delta of inflation
   double chi_;                     // parameter chi of inflation
   size_t maxiter_;                 // maximum number of iterations
   Tolerance dtol_;                 // tolerance for the certification technique
};

} // namespace

#endif
