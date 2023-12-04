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

#include "realpaver/Interval.hpp"
#include "realpaver/IntervalUnion.hpp"
#include "realpaver/Range.hpp"
#include "realpaver/ZeroOne.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a pure abstract class representing a variable domain.
///
/// The compatibility with intervals is ensured.
///////////////////////////////////////////////////////////////////////////////
class Domain {
public:
   /// Virtual destructor
   virtual ~Domain();

   /// @return true if this is empty
   virtual bool isEmpty() const = 0;

   /// Returns the interval hull of this
   virtual Interval intervalHull() const = 0;

   /// Contraction method
   /// @param x an interval assigned to hull(x inter this)
   virtual void contract(Interval& x) = 0;

   /// @return a clone of this
   virtual Domain* clone() const = 0;
};

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
   bool isEmpty() const override;
   Interval intervalHull() const override;
   void contract(Interval& x) override;
   IntervalDomain* clone() const override;
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

   /// @return the interval enclosed
   const IntervalUnion& getVal() const;

   /// Setter
   /// @param u an interval union assigned to this
   void setVal(const IntervalUnion& u);

   ///@{
   bool isEmpty() const override;
   Interval intervalHull() const override;
   void contract(Interval& x) override;
   IntervalUnionDomain* clone() const override;
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
   bool isEmpty() const override;
   Interval intervalHull() const override;
   void contract(Interval& x) override;
   RangeDomain* clone() const override;
   ///@}

private:
   Range val_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a 0/1 domain.
///////////////////////////////////////////////////////////////////////////////
class BinaryDomain : public Domain {
public:
   /// Creates a 0/1 domain
   BinaryDomain();

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
   bool isEmpty() const override;
   Interval intervalHull() const override;
   void contract(Interval& x) override;
   BinaryDomain* clone() const override;
   ///@}

private:
   ZeroOne val_;
};

} // namespace

#endif
