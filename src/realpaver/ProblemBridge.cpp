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
 * @file   ProblemBridge.cpp
 * @brief  Bridge between problems managed by the preprocessor
 * @author Laurent Granvilliers
 * @date   3 june 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ProblemBridge.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

ProblemBridge::ProblemBridge(Problem &src, Problem &dest)
    : src_(&src)
    , box_(nullptr)
    , dest_(&dest)
    , varmap_()
    , fixmap_()
{
   box_ = new DomainBox(src.scope());
}

ProblemBridge::~ProblemBridge()
{
   delete box_;
}

bool ProblemBridge::allVarsFixed() const
{
   return varmap_.empty();
}

bool ProblemBridge::isFixed(const Variable &v) const
{
   auto it = fixmap_.find(v);
   return it != fixmap_.end();
}

bool ProblemBridge::isUnfixed(const Variable &v) const
{
   auto it = varmap_.find(v);
   return it != varmap_.end();
}

Interval ProblemBridge::getFixedDomain(const Variable &v) const
{
   ASSERT(isFixed(v), "Domain of " << v.getName() << " is not fixed");

   auto it = fixmap_.find(v);
   return it->second;
}

Variable ProblemBridge::srcToDestVar(const Variable &v) const
{
   ASSERT(!isFixed(v), "Domain of " << v.getName() << " is fixed");

   auto it = varmap_.find(v);
   return it->second;
}

Scope ProblemBridge::destScope() const
{
   Scope scop;
   for (auto p : varmap_)
      scop.insert(p.second);
   return scop;
}

Scope ProblemBridge::fixedScope() const
{
   Scope scop;
   for (auto p : fixmap_)
      scop.insert(p.first);
   return ScopeBank::getInstance()->insertScope(scop);
}

Scope ProblemBridge::unfixedScope() const
{
   Scope scop;
   for (auto p : varmap_)
      scop.insert(p.first);
   return ScopeBank::getInstance()->insertScope(scop);
}

size_t ProblemBridge::nbFixedVars() const
{
   return fixmap_.size();
}

size_t ProblemBridge::nbUnfixedVars() const
{
   return varmap_.size();
}

Variable ProblemBridge::getFixedVar(size_t i) const
{
   ASSERT(i < nbFixedVars(), "Bad access to a fixed variable in a preprocessor");

   auto it = fixmap_.begin();
   std::advance(it, i);
   return (*it).first;
}

Variable ProblemBridge::getUnfixedVar(size_t i) const
{
   ASSERT(i < nbUnfixedVars(), "Bad access to a-n unfixed variable in a preprocessor");

   auto it = varmap_.begin();
   std::advance(it, i);
   return (*it).first;
}

DomainBox ProblemBridge::fixedRegion() const
{
   ASSERT(fixmap_.size() > 0, "Fixed region required but no fixed variable");

   IntervalBox B(fixedScope());
   for (auto p : fixmap_)
      B.set(p.first, p.second);

   return DomainBox(B);
}

DomainBox ProblemBridge::destRegion() const
{
   DomainBox dest(destScope());
   for (auto p : varmap_)
   {
      std::unique_ptr<Domain> domain(box_->get(p.first)->clone());
      dest.set(p.second, std::move(domain));
   }
   return dest;
}

Variable ProblemBridge::insertVar(Variable v, const Domain &dom)
{
   // creates a clone of the variable in the destination problem
   Variable w = dest_->addClonedVar(v);

   // assigns the reduced domain to it
   std::unique_ptr<Domain> wdom(dom.clone());
   w.setDomain(std::move(wdom));

   // new map entry
   varmap_.insert(std::make_pair(v, w));

   return w;
}

void ProblemBridge::insertFixed(Variable v, const Domain &dom)
{
   fixmap_.insert(std::make_pair(v, dom.intervalHull()));
}

DomainBox &ProblemBridge::srcRegion()
{
   return *box_;
}

void ProblemBridge::removeFixedVar(const Variable &v)
{
   auto it = fixmap_.find(v);
   if (it != fixmap_.end())
      fixmap_.erase(it);
}

} // namespace realpaver
