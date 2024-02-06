///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_CONTRACTOR_ACID_HPP
#define REALPAVER_NCSP_CONTRACTOR_ACID_HPP

#include <memory>
#include "realpaver/ContractorVar3BCID.hpp"
#include "realpaver/IntervalSmearSumRel.hpp"
#include "realpaver/IntervalSlicer.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a propagator that implements the adaptive CID strategy.
///////////////////////////////////////////////////////////////////////////////
class ContractorACID : public Contractor {
public:
   /// Constructor
   /// @param ssr calculator of smear sum rel values
   /// @param op contractor of slices
   /// @param ns3B number of slices for 3B contractors
   /// @param nsCID number of slices for CID contractors
   /// @param learnLength length of the learning phase
   /// @param cycleLength length of the learning+exploitation phase
   /// @param ctRatio threshold on the quality of contractions
   ContractorACID(std::shared_ptr<IntervalSmearSumRel> ssr,
                  SharedContractor op, int ns3B, int nsCID,
                  int learnLength, int cycleLength, double ctRatio);

   /// Default copy constructor
   ContractorACID(const ContractorACID&) = default;

   /// No assignment
   ContractorACID& operator=(const ContractorACID&) = delete;

   /// Default destructor
   ~ContractorACID() = default;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;
   ///@}

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

   std::vector<size_t> kVarCID_;

   size_t call_;        // number of calls of the contract method
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
   static int lastSignificantGain(std::vector<double>& ctcGains,
                                  double ctRatio);

   // It computes the average of v.
   static size_t avgNbVarCID(std::vector<size_t>& v);
};

/// Type of shared pointers on contractors
typedef std::shared_ptr<ContractorACID> SharedContractorACID;

} // namespace

#endif
