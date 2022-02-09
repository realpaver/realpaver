///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOPPresolver.hpp"
#include "realpaver/contractor_bco.hpp"
#include "realpaver/contractor_hc4.hpp"

namespace realpaver {

BOPPresolver::BOPPresolver(BOPModel& model)
      :  pool_(),
         propagator_(),
         init_(nullptr),
         region_(model.getInitRegion())
{
   init_ = std::make_shared<IntervalVector>(model.getInitRegion());

   Dag* dag = model.getDag();
   size_t i = 0;

   // operators associated with df / dv = 0
   for (Variable v : model.getObjScope())
   {
      SharedContractor op = std::make_shared<Hc4Contractor>(dag, i++);

      SharedContractor bop =
         std::make_shared<BcoContractor>(dag, i, v, op, init_);

      pool_.push(bop);
   }

   propagator_.setPool(&pool_);
}

bool BOPPresolver::presolve()
{
   Proof proof = propagator_.contract(region_);

   return proof != Proof::Empty;
}

IntervalVector BOPPresolver::getContractedRegion() const
{
   return region_;
}

} // namespace
