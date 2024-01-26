///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/ContractorList.hpp"
#include "realpaver/NcspPropagator.hpp"

namespace realpaver {

NcspPropagator::~NcspPropagator()
{}

///////////////////////////////////////////////////////////////////////////////

NcspHC4::NcspHC4(ContractorFactory& facto)
      : NcspPropagator()
{
   // HC4
   op_ = facto.makeHC4();

   // Contractors for disconnected variable domains
   SharedContractor dop = facto.makeContractorDomain();
   if (dop != nullptr)
   {
      SharedContractorVector pool = std::make_shared<ContractorVector>();
      pool->push(op_);
      pool->push(dop);
      op_ = std::make_shared<ContractorList>(pool);
   }
}

Proof NcspHC4::contract(NcspNode& node, NcspContext& ctx)
{
   IntervalBox B(*node.box());
   return op_->contract(B);
}

///////////////////////////////////////////////////////////////////////////////

NcspHC4Newton::NcspHC4Newton(ContractorFactory& facto)
      : NcspPropagator()
{
   // HC4
   op_ = facto.makeHC4();

   // Interval Newton
   SharedContractor newton = facto.makeIntervalNewton();
   if (newton != nullptr)
   {
      SharedContractorVector pool = std::make_shared<ContractorVector>();
      pool->push(op_);
      pool->push(newton);
      op_ = std::make_shared<ContractorList>(pool);
   }

   // Contractors for disconnected variable domains
   SharedContractor dop = facto.makeContractorDomain();
   if (dop != nullptr)
   {
      SharedContractorVector pool = std::make_shared<ContractorVector>();
      pool->push(op_);
      pool->push(dop);
      op_ = std::make_shared<ContractorList>(pool);
   }
}

Proof NcspHC4Newton::contract(NcspNode& node, NcspContext& ctx)
{
   IntervalBox B(*node.box());
   return op_->contract(B);
}

} // namespace
