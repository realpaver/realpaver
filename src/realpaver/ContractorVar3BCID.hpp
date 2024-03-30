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
 * @date   2022-5-6
*/

#ifndef REALPAVER_CONTRACTOR_VAR_3BCID_HPP
#define REALPAVER_CONTRACTOR_VAR_3BCID_HPP

#include "realpaver/ContractorVar3B.hpp"
#include "realpaver/ContractorVarCID.hpp"

namespace realpaver {

/**
 * @brief Contractor that combines a 3B contractor and a CID contractor.
 *
 * Given a variable and a box B, the 3B contractor is applied. If the domain
 * of v in B is not reduced then B is returned. Oherwise, the CID
 * contractor is applied.
 */
class ContractorVar3BCID : public Contractor {
public:
   /**
    * @brief Constructor.
    * @param op a contractor
    * @param v a variable
    * @param slicer3B a slicer for the 3B contractor
    * @param slicerCID  a slicer for the CID contractor
    */
   ContractorVar3BCID(SharedContractor op, Variable v,
                      std::unique_ptr<IntervalSlicer> slicer3B,
                      std::unique_ptr<IntervalSlicer> slicerCID);

   /**
    * @brief Constructor.
    * @param op a contractor
    * @param v a variable
    * @param n3B number of slices for the 3B contractor (at least 2)
    * @param nCID  number of slices for the CID contractor (at least 2)
    */
   ContractorVar3BCID(SharedContractor op, Variable v, size_t n3B, size_t nCID);

   /// Destructor
   ~ContractorVar3BCID();

   /// No copy
   ContractorVar3BCID(const ContractorVar3BCID&) = delete;

   /// No assignment
   ContractorVar3BCID& operator=(const ContractorVar3BCID&) = delete;

   /// Returns the variable whose domain is sliced
   Variable getVar() const;

   /// Assigns the variable whose domain is sliced
   void setVar(Variable v);

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

private:
   Variable v_;               // variable whose domain is sliced
   ContractorVar3B* ctc3B_;   // 3B contractor
   ContractorVarCID* ctcCID_; // CID contractor
};

} // namespace

#endif
