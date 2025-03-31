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
 * @file   Contractor3BCID.hpp
 * @brief  3BCID contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CONTRACTOR_VAR_3BCID_HPP
#define REALPAVER_CONTRACTOR_VAR_3BCID_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

/**
 * @brief Contractor that combines the 3B and CID strategies.
 *
 * Let op be a contractor, let v a variable from its scope, and let B be a box.
 * Let ns3B and nsCID be integers greater than 2.
 *
 * First, a shaving process (3B) is applied. The domain of v in B is divided in
 * ns3B slices. Then the outermost inconsistent slices are eliminated by
 * applying op. If 0, 1 or 2 slices remain, then the contraction process stops.
 * Otherwise, a CID strategy is applied to the remaining box (the initial box
 * minus the outermost consistent slices.
 *
 * A threshold on the width of variable domains is considered. Basically, only
 * slices whose widhths are greater than this threshold are processed.
 */
class ContractorVar3BCID : public Contractor {
public:
   /**
    * @brief Constructor.
    * @param op a contractor
    * @param v a variable
    * @param ns3B number of slices for the 3B contraction process
    * @param nsCID number of slices for the CID contraction process
    */
   ContractorVar3BCID(SharedContractor op, Variable v, size_t ns3B, size_t nsCID);

   /// Default destructor
   ~ContractorVar3BCID() = default;

   /// No copy
   ContractorVar3BCID(const ContractorVar3BCID &) = delete;

   /// No assignment
   ContractorVar3BCID &operator=(const ContractorVar3BCID &) = delete;

   /// Returns the variable whose domain is sliced
   Variable getVar() const;

   /// Assigns the variable whose domain is sliced
   void setVar(Variable v);

   Scope scope() const override;
   Proof contract(IntervalBox &B) override;
   void print(std::ostream &os) const override;

   /// Returns the threshold on the width of variable domains
   double varMinWidth() const;

   /// Assigns the threshold on the width of variable domains
   void setVarMinWidth(double val);

private:
   SharedContractor op_; // contractor of slices
   Variable v_;          // variable whose domain is sliced
   size_t ns3B_;         // number of slices for the 3B contractor
   size_t nsCID_;        // number of slices for the CID contractor
   double varMnWidth_;   // threshold on the width of variable domains

   // CID process applied to Bcid
   // B3bCid is the resulting box
   // Returns true if it leads to a contraction
   bool contractCID(IntervalBox &Bcid, IntervalBox &B3bCid);
};

} // namespace realpaver

#endif
