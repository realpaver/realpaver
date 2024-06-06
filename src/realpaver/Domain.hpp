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
 * @file   Domain.hpp
 * @brief  Classes of domains
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_DOMAIN_HPP
#define REALPAVER_DOMAIN_HPP

#include <iostream>
#include "realpaver/Interval.hpp"
#include "realpaver/IntervalUnion.hpp"
#include "realpaver/Range.hpp"
#include "realpaver/RangeUnion.hpp"
#include "realpaver/ZeroOne.hpp"

namespace realpaver {

/// Domain types with explicit values that can be used as array indexes
enum class DomainType {
   Binary        = 0,   ///< binary
   Interval      = 1,   ///< continuous interval
   IntervalUnion = 2,   ///< union of continuous intervals
   Range         = 3,   ///< discrete range
   RangeUnion    = 4    ///< union of discrete ranges
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Base class representing a variable domain.
 * 
 * A domain is a set of values that can be assigned to a variable. It can be
 * either discrete or continuous, connected or not. The compatibility with
 * intervals is ensured.
*/
class Domain {
public:
   /// Constructor
   Domain(DomainType type);

   /// Virtual destructor
   virtual ~Domain();

   /// Returns the domain type
   DomainType type() const;

   /**
    * @brief Returns the size of this.
    * 
    * The size is defined as:
    * - the width of the hull of this (continuous domain)
    * - the number of values in this (discrete domain)
    *
    * If this is empty then the size must be equal to 0.
    */
   virtual double size() const = 0;

   /// Returns true if this is empty
   virtual bool isEmpty() const = 0;

   /// Returns true if this is canonical (non empty and not splitable)
   virtual bool isCanonical() const = 0;

   /// Returns the interval hull of this
   virtual Interval intervalHull() const = 0;

   /// Contracts x as hull(x inter this)
   virtual void contractInterval(Interval& x) const = 0;

   /// Contracts this as (x inter this)
   virtual void contract(const Interval& x) = 0;

   /// Returns true if this is connected (e.g. real interval)
   virtual bool isConnected() const;

   /// Returns true if this is a binary domain
   virtual bool isBinary() const;

   /// Returns true if this is an integer domain
   virtual bool isInteger() const;

   /// Rsturns true if this is a real domain
   virtual bool isReal() const;

   /// Returns a clone of this
   virtual Domain* clone() const = 0;

   /// Output a stream
   virtual void print(std::ostream& os) const = 0;

private:
   DomainType type_;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Domain& dom);

/*----------------------------------------------------------------------------*/

/// Interval domain
class IntervalDomain : public Domain {
public:
   /// Constructor
   IntervalDomain(const Interval& x);

   /// Default copy constructor
   IntervalDomain(const IntervalDomain&) = default;

   /// No assignment
   IntervalDomain& operator=(const IntervalDomain&) = delete;

   /// Default destructor
   ~IntervalDomain() = default;

   /// Returns the interval enclosed
   const Interval& getVal() const;

   /// Assigns an interval to this
   void setVal(const Interval& x);

   double size() const override;
   bool isEmpty() const override;
   bool isCanonical() const override;
   bool isReal() const override;
   Interval intervalHull() const override;
   void contractInterval(Interval& x) const override;
   void contract(const Interval& x) override;
   bool isConnected() const override;
   IntervalDomain* clone() const override;
   void print(std::ostream& os) const override;

private:
   Interval val_;
};

/*----------------------------------------------------------------------------*/

/// Interval union domain
class IntervalUnionDomain : public Domain {
public:
   /// Constructor
   IntervalUnionDomain(const IntervalUnion& u);

   /// Constructor
   IntervalUnionDomain(const std::initializer_list<Interval>& l);

   /// Default copy constructor
   IntervalUnionDomain(const IntervalUnionDomain&) = default;

   /// No assignment
   IntervalUnionDomain& operator=(const IntervalUnionDomain&) = delete;

   /// Default destructor
   ~IntervalUnionDomain() = default;

   /// Returns the interval union enclosed
   const IntervalUnion& getVal() const;

   /// Assigns an interval union to this
   void setVal(const IntervalUnion& u);

   double size() const override;
   bool isEmpty() const override;
   bool isCanonical() const override;
   bool isReal() const override;
   Interval intervalHull() const override;
   void contractInterval(Interval& x) const override;
   void contract(const Interval& x) override;
   IntervalUnionDomain* clone() const override;
   void print(std::ostream& os) const override;

private:
   IntervalUnion val_;
};

/*----------------------------------------------------------------------------*/

/// Range domain
class RangeDomain : public Domain {
public:
   /// Constructor
   RangeDomain(const Range& r);

   /// Default copy constructor
   RangeDomain(const RangeDomain&) = default;

   /// No assignment
   RangeDomain& operator=(const RangeDomain&) = delete;

   /// Default destructor
   ~RangeDomain() = default;

   /// Returns the range enclosed
   const Range& getVal() const;

   /// Assigns a range to this
   void setVal(const Range& r);

   double size() const override;
   bool isEmpty() const override;
   bool isCanonical() const override;
   bool isInteger() const override;
   Interval intervalHull() const override;
   void contractInterval(Interval& x) const override;
   void contract(const Interval& x) override;
   RangeDomain* clone() const override;
   void print(std::ostream& os) const override;

private:
   Range val_;
};

/*----------------------------------------------------------------------------*/

/// Range union domain
class RangeUnionDomain : public Domain {
public:
   /// Constructor
   RangeUnionDomain(const RangeUnion& u);

   /// Constructor
   RangeUnionDomain(const std::initializer_list<Range>& l);

   /// Default copy constructor
   RangeUnionDomain(const RangeUnionDomain&) = default;

   /// No assignment
   RangeUnionDomain& operator=(const RangeUnionDomain&) = delete;

   /// Default destructor
   ~RangeUnionDomain() = default;

   /// Returns the range union enclosed
   const RangeUnion& getVal() const;

   /// Assigns a range union to this
   void setVal(const RangeUnion& u);

   double size() const override;
   bool isEmpty() const override;
   bool isCanonical() const override;
   bool isInteger() const override;
   Interval intervalHull() const override;
   void contractInterval(Interval& x) const override;
   void contract(const Interval& x) override;
   RangeUnionDomain* clone() const override;
   void print(std::ostream& os) const override;

private:
   RangeUnion val_;
};

/*----------------------------------------------------------------------------*/

/// 0/1 domain
class BinaryDomain : public Domain {
public:
   /// Constructor
   BinaryDomain();

   /// Constructor
   BinaryDomain(const ZeroOne& zo);

   /// Default copy constructor
   BinaryDomain(const BinaryDomain&) = default;

   /// No assignment
   BinaryDomain& operator=(const BinaryDomain&) = delete;

   /// Default destructor
   ~BinaryDomain() = default;

   /// Returns the zero-one enclosed
   const ZeroOne& getVal() const;

   /// Assigns a zero-one to this
   void setVal(const ZeroOne& zo);

   double size() const override;
   bool isEmpty() const override;
   bool isCanonical() const override;
   bool isBinary() const override;
   Interval intervalHull() const override;
   void contractInterval(Interval& x) const override;
   void contract(const Interval& x) override;
   BinaryDomain* clone() const override;
   void print(std::ostream& os) const override;

private:
   ZeroOne val_;
};

} // namespace

#endif
