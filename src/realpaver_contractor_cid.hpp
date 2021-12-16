// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONTRACTOR_CID_HPP
#define REALPAVER_CONTRACTOR_CID_HPP

#include "realpaver_contractor.hpp"
#include "realpaver_interval_slicer.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of contractors implementing the CID operator.
 *****************************************************************************/
class CidContractor : public Contractor {
public:
   // constructor
   // this contractor owns the slicer, hence the slicer is destroyed
   // when this contractor is destroyed
   CidContractor(const SharedContractor& op, const Variable& v,
                 IntervalSlicer* slicer);

   // destructor
   ~CidContractor();

   // copy protection
   CidContractor(const CidContractor&) = delete;
   CidContractor& operator=(const CidContractor&) = delete;

   // Override
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(Box& B);
   void print(std::ostream& os) const;

private:
   SharedContractor op_;
   Variable v_;
   IntervalSlicer* slicer_;
   Bitset bs_;
};

} // namespace

#endif
