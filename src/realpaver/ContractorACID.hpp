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
 * @file   ContractorACID.hpp
 * @brief  ACID contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CONTRACTOR_ACID_HPP
#define REALPAVER_CONTRACTOR_ACID_HPP

#include <memory>
#include "realpaver/ContractorVar3BCID.hpp"
#include "realpaver/IntervalSmearSumRel.hpp"
#include "realpaver/IntervalSlicer.hpp"

namespace realpaver {

/**
 * @brief Propagation algorithm implementing the adaptive CID strategy (Alg. 1).
 *
 * The ACID1 algorithm works as follows.
 * With each variable of the problem is associated a 3BCID contractor.
 * In each call of the contraction method, numVarCID 3BCID contractors
 * are applied.
 *
 * Which ordering for these contractors? The one given by the smear sum
 * relative strategy that evaluates the derivatives in the current box.
 *
 * How many of the contractors? numVarCID which is first assigned to the
 * number of variables. This number then evolves in learning phases. And
 * it is just used in exploitation phases.
 *
 * Let learnLength be the number of calls of the contraction method in every
 * learning phase. In each of these calls, the numVarCID 3BCID contractors
 * are applied and we seek for the last one that has reduced the box enough
 * with a respect to a ratio called ctRatio. At the end of the learning phase,
 * an average is calculated and it is assigned to numVarCID for the next
 * exploitation phase.
 */
class ContractorACID : public Contractor {
public:
   /**
    * @brief Constructor.
    * @param ssr calculator of smear sum rel values
    * @param op contractor of slices
    * @param ns3B number of slices for 3B contractors
    * @param nsCID number of slices for CID contractors
    * @param learnLength length of the learning phase
    * @param cycleLength length of the learning+exploitation phase
    * @param ctRatio threshold on the quality of contractions
    * @param varMinWidth threshold on the wodth of variable domains
    */
   ContractorACID(std::shared_ptr<IntervalSmearSumRel> ssr,
                  SharedContractor op, int ns3B, int nsCID,
                  int learnLength, int cycleLength, double ctRatio,
                  double varMinWidth);

   /// Default copy constructor
   ContractorACID(const ContractorACID&) = default;

   /// No assignment
   ContractorACID& operator=(const ContractorACID&) = delete;

   /// Default destructor
   ~ContractorACID() = default;

   /// Returns the length of the learning phase
   int learnLength() const;

   /// Returns the length of the learning+exploitation phase
   int cycleLength() const;

   /// Returns the threshold on the reduction gain
   double ctRatio() const;

   /// Returns the contractor of slices
   SharedContractor sliceContractor() const;

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

private:
   std::shared_ptr<IntervalSmearSumRel> ssr_;   // calculator of smear
                                                // sum rel values
   SharedContractor op_;                        // contractor of slices e.g. HC4
   Scope scop_;                                 // scope
   size_t n_;                                   // scope size

   int numVarCID_;   // number of var3BCID contractors that must be applied
                     // in a call to the contract method

   std::vector<ContractorVar3BCID*> // var3BCID_[i] is the var3BCID contractor
      var3BCID_;                    // associated with the i-th variable
                                    // of the scope

   int sumGood_;  // during the learning phase, it counts the number of
                  // applications of var3BCID contractors that have reduced
                  // enough the box with respect to ctRatio_

   int nbCalls_;        // number of calls of the contract method
   int learnLength_;    // number of calls in the learning phase
   int cycleLength_;    // number of calls in a cycle made of an exploitation
                        // phase and a learning phase
   double ctRatio_;     // threshold on the reduction gain

   // Let ctcGains = g_0, ..., g_(p-1) be the reduction ratios obtained by the
   // p var3BCID contractors applied in some call of the contract method.
   //
   // If g_i <= ctRatio for each i then it returns 0, which means that no
   // contractor is efficient enough.
   //
   // Otherwise let k be the greatest integer such that g_k > ctRatio, which
   // indicates that the gains are not enough after k. Then it returns k+1,
   // which the maximum number of contractors that must be applied in order to
   // obtain a sufficient gain.
   static int lastSignificantGain(double* ctcGains, double ctRatio, int imax);

   // Calculates the gain ratio between prev and the reduced box next
   static double gainRatio(const IntervalBox& prev, const IntervalBox& next,
                           const Scope& scop);
};

/** @brief Type of shared pointers on contractors */
using SharedContractorACID = std::shared_ptr<ContractorACID>;

} // namespace

#endif
