///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_DOMAIN_HPP
#define REALPAVER_DOMAIN_HPP

#include <iostream>
#include "realpaver/Interval.hpp"
#include "realpaver/IntervalUnion.hpp"
#include "realpaver/Range.hpp"
#include "realpaver/RangeUnion.hpp"
#include "realpaver/ZeroOne.hpp"

namespace realpaver {
   
/// domain types
enum class DomainType {
   Binary,
   Interval,
   IntervalUnion,
   Range,
   RangeUnion
};

///////////////////////////////////////////////////////////////////////////////
/// This is a pure abstract class representing a variable domain.
///
/// The compatibility with intervals is ensured.
///////////////////////////////////////////////////////////////////////////////
class Domain {
public:
   /// Constructor
   Domain(DomainType type);

   /// Virtual destructor
   virtual ~Domain();

   /// @return the domain type
   DomainType type() const;

   /// @return if this is a real domain then returns the width of this,
   ///         returns the number of values in this otherwise
   ///
   /// If this is empty then the size must be equal to 0.
   virtual double size() const = 0;

   /// @return true if this is empty
   virtual bool isEmpty() const = 0;

   /// @return true if this is canonical (non empty and not splitable)
   virtual bool isCanonical() const = 0;

   /// Returns the interval hull of this
   virtual Interval intervalHull() const = 0;

   /// Contraction method
   /// @param x an interval assigned to hull(x inter this)
   virtual void contractInterval(Interval& x) const = 0;

   /// Contraction method
   /// @param x an interval such that this is assigned to (x inter this)
   virtual void contract(const Interval& x) = 0;

   /// @return true if this is connected, typically if it is a real interval
   ///
   /// The default implementation returns false
   virtual bool isConnected() const;

   /// @return true if this is a binary domain
   ///
   /// The default implementation returns false
   virtual bool isBinary() const;

   /// @return true if this is an integer domain
   ///
   /// The default implementation returns false
   virtual bool isInteger() const;

   /// @return true if this is a real domain
   ///
   /// The default implementation returns false
   virtual bool isReal() const;

   /// @return a clone of this
   virtual Domain* clone() const = 0;

   /// prints this on a stream
   /// @param os an output stream
   virtual void print(std::ostream& os) const = 0;

private:
   DomainType type_;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Domain& dom);

///////////////////////////////////////////////////////////////////////////////
/// This is an interval domain.
///////////////////////////////////////////////////////////////////////////////
class IntervalDomain : public Domain {
public:
   /// Creates a domain
   IntervalDomain(const Interval& x);

   /// Default copy constructor
   IntervalDomain(const IntervalDomain&) = default;

   /// No assignment
   IntervalDomain& operator=(const IntervalDomain&) = delete;

   /// Default destructor
   ~IntervalDomain() = default;

   /// @return the interval enclosed
   const Interval& getVal() const;

   /// Setter
   /// @param x an interval assigned to this
   void setVal(const Interval& x);

   ///@{
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
   ///@}

private:
   Interval val_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is an interval union domain.
///////////////////////////////////////////////////////////////////////////////
class IntervalUnionDomain : public Domain {
public:
   /// Creates a domain
   /// @param u value assigned to this
   IntervalUnionDomain(const IntervalUnion& u);

   /// Creates a domain
   /// @param l value assigned to this
   IntervalUnionDomain(const std::initializer_list<Interval>& l);

   /// Default copy constructor
   IntervalUnionDomain(const IntervalUnionDomain&) = default;

   /// No assignment
   IntervalUnionDomain& operator=(const IntervalUnionDomain&) = delete;

   /// Default destructor
   ~IntervalUnionDomain() = default;

   /// @return the interval union enclosed
   const IntervalUnion& getVal() const;

   /// Setter
   /// @param u an interval union assigned to this
   void setVal(const IntervalUnion& u);

   ///@{
   double size() const override;
   bool isEmpty() const override;
   bool isCanonical() const override;
   bool isReal() const override;
   Interval intervalHull() const override;
   void contractInterval(Interval& x) const override;
   void contract(const Interval& x) override;
   IntervalUnionDomain* clone() const override;
   void print(std::ostream& os) const override;
   ///@}

private:
   IntervalUnion val_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a range domain.
///////////////////////////////////////////////////////////////////////////////
class RangeDomain : public Domain {
public:
   /// Creates a domain
   /// @param r value assigned to this
   RangeDomain(const Range& r);

   /// Default copy constructor
   RangeDomain(const RangeDomain&) = default;

   /// No assignment
   RangeDomain& operator=(const RangeDomain&) = delete;

   /// Default destructor
   ~RangeDomain() = default;

   /// @return the range enclosed
   const Range& getVal() const;

   /// Setter
   /// @param r a range assigned to this
   void setVal(const Range& r);

   ///@{
   double size() const override;
   bool isEmpty() const override;
   bool isCanonical() const override;
   bool isInteger() const override;
   Interval intervalHull() const override;
   void contractInterval(Interval& x) const override;
   void contract(const Interval& x) override;
   RangeDomain* clone() const override;
   void print(std::ostream& os) const override;
   ///@}

private:
   Range val_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a range union domain.
///////////////////////////////////////////////////////////////////////////////
class RangeUnionDomain : public Domain {
public:
   /// Creates a domain
   /// @param u value assigned to this
   RangeUnionDomain(const RangeUnion& u);

   /// Creates a domain
   /// @param l value assigned to this
   RangeUnionDomain(const std::initializer_list<Range>& l);

   /// Default copy constructor
   RangeUnionDomain(const RangeUnionDomain&) = default;

   /// No assignment
   RangeUnionDomain& operator=(const RangeUnionDomain&) = delete;

   /// Default destructor
   ~RangeUnionDomain() = default;

   /// @return the range union enclosed
   const RangeUnion& getVal() const;

   /// Setter
   /// @param u a range union assigned to this
   void setVal(const RangeUnion& u);

   ///@{
   double size() const override;
   bool isEmpty() const override;
   bool isCanonical() const override;
   bool isInteger() const override;
   Interval intervalHull() const override;
   void contractInterval(Interval& x) const override;
   void contract(const Interval& x) override;
   RangeUnionDomain* clone() const override;
   void print(std::ostream& os) const override;
   ///@}

private:
   RangeUnion val_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a 0/1 domain.
///////////////////////////////////////////////////////////////////////////////
class BinaryDomain : public Domain {
public:
   /// Creates a 0/1 domain
   BinaryDomain();

   /// Creates a 0/1 domain
   /// @param zo value of this
   BinaryDomain(const ZeroOne& zo);

   /// Default copy constructor
   BinaryDomain(const BinaryDomain&) = default;

   /// No assignment
   BinaryDomain& operator=(const BinaryDomain&) = delete;

   /// Default destructor
   ~BinaryDomain() = default;

   /// @return the zero-one enclosed
   const ZeroOne& getVal() const;

   /// Setter
   /// @param z a zero-one assigned to this
   void setVal(const ZeroOne& zo);

   ///@{
   double size() const override;
   bool isEmpty() const override;
   bool isCanonical() const override;
   bool isBinary() const override;
   Interval intervalHull() const override;
   void contractInterval(Interval& x) const override;
   void contract(const Interval& x) override;
   BinaryDomain* clone() const override;
   void print(std::ostream& os) const override;
   ///@}

private:
   ZeroOne val_;
};

} // namespace

#endif
