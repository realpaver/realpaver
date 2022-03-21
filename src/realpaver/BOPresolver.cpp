///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOContractor.hpp"
#include "realpaver/BOPresolver.hpp"
#include "realpaver/HC4Contractor.hpp"

namespace realpaver {

BOPresolver::BOPresolver(BOModel& model)
      : propagator_(),
        init_(nullptr),
        region_(model.getInitRegion())
{
   init_ = std::make_shared<IntervalRegion>(model.getInitRegion());
   SharedContractorVector pool = std::make_shared<ContractorVector>();

   Dag* dag = model.getDag();
   size_t i = 0;

   // operators associated with df / dv = 0
   for (Variable v : model.getObjScope())
   {
      SharedContractor op = std::make_shared<HC4Contractor>(dag, i);

      if (model.isBoundaryVar(v))
      {
         SharedContractor bop =
            std::make_shared<BOContractor>(dag, i, v, op, init_);

         pool->push(bop);
      }
      else
      {
         pool->push(op);
      }

      i = i+1;
   }

   propagator_.setPool(pool);
}

bool BOPresolver::presolve()
{   
   Proof proof = propagator_.contract(region_);

   return proof != Proof::Empty;
}

IntervalRegion BOPresolver::getContractedRegion() const
{
   return region_;
}

} // namespace
