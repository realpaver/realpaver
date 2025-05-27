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
 * @file   Linearizer.cpp
 * @brief  Linearization of nonlinear problems
 * @author Laurent Granvilliers
 * @date   2025-fev-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Linearizer.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

Linearizer::Linearizer(SharedDag dag)
    : dag_(dag)
    , scop_(dag->scope())
    , lfun_()
    , tol_(Params::GetDblParam("RELAXATION_EQ_TOL"))
    , mvv_()
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");

   for (size_t i = 0; i < dag->nbFuns(); ++i)
      lfun_.push_back(i);

   scop_ = ScopeBank::getInstance()->insertScope(scop_);
}

Linearizer::Linearizer(SharedDag dag, const IndexList &lfun)
    : dag_(dag)
    , scop_()
    , lfun_()
    , tol_(Params::GetDblParam("RELAXATION_TOL"))
    , mvv_()
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");
   ASSERT(!lfun.empty(), "No list of functions in a polytope maker");

   for (size_t i : lfun)
   {
      ASSERT(i < dag_->nbFuns(), "Bad function index in a polytope maker");

      lfun_.push_back(i);
      scop_.insert(dag->fun(i)->scope());
   }

   scop_ = ScopeBank::getInstance()->insertScope(scop_);
}

Linearizer::~Linearizer()
{
}

SharedDag Linearizer::dag() const
{
   return dag_;
}

Scope Linearizer::scope() const
{
   return scop_;
}

size_t Linearizer::linVarIndex(Variable v) const
{
   auto it = mvv_.find(v.id());
   ASSERT(it != mvv_.end(), "Iterator null");
   return it->second;
}

size_t Linearizer::linVarIndex(size_t id) const
{
   auto it = mvv_.find(id);
   ASSERT(it != mvv_.end(), "Iterator null");
   return it->second;
}

void Linearizer::setLinVarIndex(size_t id, size_t k)
{
   auto it = mvv_.find(id);
   if (it != mvv_.end())
      it->second = k;
   else
      mvv_.insert(std::make_pair(id, k));
}

double Linearizer::getRelaxTol() const
{
   return tol_;
}

void Linearizer::setRelaxTol(double tol)
{
   ASSERT(tol >= 0.0, "The relaxation tolerance must be positive: " << tol);

   tol_ = tol;
}

} // namespace realpaver
