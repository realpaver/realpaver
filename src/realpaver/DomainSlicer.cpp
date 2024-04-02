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
 * @file   DomainSlicer.cpp
 * @brief  Classes of domain slicers
 * @author Laurent Granvilliers
 * @date   2023-11-19
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/DomainSlicer.hpp"

namespace realpaver {

DomainSlicer::~DomainSlicer()
{}

size_t DomainSlicer::apply(Domain* dom)
{
   cont_.clear();
   applyImpl(dom);
   return cont_.size();
}

size_t DomainSlicer::nbSlices() const
{
   return cont_.size();
}

void DomainSlicer::clear()
{
   cont_.clear();
}

void DomainSlicer::push(Domain* dom)
{
   if (!dom->isEmpty())
   {
      std::unique_ptr<Domain> p(dom);
      cont_.push_back(std::move(p));
   }
}

DomainSlicer::iterator DomainSlicer::begin()
{
   return cont_.begin();
}

DomainSlicer::iterator DomainSlicer::end()
{
   return cont_.end();
}

std::unique_ptr<Domain> DomainSlicer::next(iterator& it)
{
   ASSERT(it != end(), "Bad iterator in a domain slicer");

   std::unique_ptr<Domain>p = std::move(*it);
   iterator aux = it;
   ++it;
   cont_.erase(aux);
   return p;
}

/*----------------------------------------------------------------------------*/

void BinaryDomainSlicer::applyImpl(Domain* dom)
{
   ASSERT(dynamic_cast<BinaryDomain*>(dom) != nullptr,
          "Bad domain type as input of a binary slicer");

   ASSERT(static_cast<BinaryDomain*>(dom)->getVal().isUniverse(),
          "Binary domain not splitable");

   push(new BinaryDomain(ZeroOne::zero()));
   push(new BinaryDomain(ZeroOne::one()));
}

/*----------------------------------------------------------------------------*/

void IntervalDomainBisecter::applyImpl(Domain* dom)
{
   IntervalDomain* ptr = dynamic_cast<IntervalDomain*>(dom);
   
   ASSERT(ptr != nullptr,
          "Bad domain type as input of an interval domain slicer");

   ASSERT(!ptr->isCanonical(), "Interval domain not splitable");

   const Interval& x = ptr->getVal();
   double m = x.midpoint();
   push(new IntervalDomain(Interval(x.left(), m)));
   push(new IntervalDomain(Interval(m, x.right())));
}

/*----------------------------------------------------------------------------*/

void IntervalUnionDomainBisecter::applyImpl(Domain* dom)
{
   IntervalUnionDomain* ptr = dynamic_cast<IntervalUnionDomain*>(dom);
   
   ASSERT(ptr != nullptr,
          "Bad domain type as input of an interval union domain slicer");

   ASSERT(!ptr->isCanonical(), "Interval union domain not splitable");

   const IntervalUnion& u = ptr->getVal();
   size_t n = u.size();

   if (n > 1)
   {
      // u has more than one interval -> divides the union in two parts
      size_t i = n >> 1;
      push(new IntervalUnionDomain(u.subUnion(0, i-1)));
      push(new IntervalUnionDomain(u.subUnion(i, n-1)));
   }
   else
   {
      // u is reduced to one interval -> bisects it
      const Interval& x = u[0];
      double m = x.midpoint();
      push(new IntervalDomain(Interval(x.left(), m)));
      push(new IntervalDomain(Interval(m, x.right())));
   }
}

/*----------------------------------------------------------------------------*/

void RangeDomainBisecter::applyImpl(Domain* dom)
{
   RangeDomain* ptr = dynamic_cast<RangeDomain*>(dom);
   
   ASSERT(ptr != nullptr,
          "Bad domain type as input of a range domain slicer");

   ASSERT(!ptr->isCanonical(), "Range domain not splitable");

   const Range& r = ptr->getVal();
   Integer m = r.midpoint();
   push(new RangeDomain(Range(r.left(), m)));
   push(new RangeDomain(Range(m+1, r.right())));
}

/*----------------------------------------------------------------------------*/

void RangeUnionDomainBisecter::applyImpl(Domain* dom)
{
   RangeUnionDomain* ptr = dynamic_cast<RangeUnionDomain*>(dom);
   
   ASSERT(ptr != nullptr,
          "Bad domain type as input of a range union domain slicer");

   ASSERT(!ptr->isCanonical(), "Range union domain not splitable");

   const RangeUnion& u = ptr->getVal();
   size_t n = u.size();

   if (n > 1)
   {
      // u has more than one range -> divides the union in two parts
      size_t i = n >> 1;
      push(new RangeUnionDomain(u.subUnion(0, i-1)));
      push(new RangeUnionDomain(u.subUnion(i, n-1)));
   }
   else
   {
      // u is reduced to one interval -> bisects it
      const Range& r = u[0];
      Integer m = r.midpoint();
      push(new RangeDomain(Range(r.left(), m)));
      push(new RangeDomain(Range(m+1, r.right())));
   }
}

} // namespace
