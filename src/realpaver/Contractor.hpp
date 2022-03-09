///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_HPP
#define REALPAVER_CONTRACTOR_HPP

#include <memory>
#include "realpaver/Bitset.hpp"
#include "realpaver/IntervalRegion.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for interval contractors.
///
/// An interval contractor is in general associated with a constraint.
/// Given an interval region, it removes infeasible facets (or it prunes
/// interval bounds) and returns a certificate of proof:
/// - Proof::Empty if there is no solution;
/// - Proof::Feasible if it is proved that there is a solution;
/// - Proof::Inner if the vector is included in the feasible set;
/// - Proof::Maybe otherwise, i.e. no proof is done.
///
/// This class is a pure abstract class.
///////////////////////////////////////////////////////////////////////////////
class Contractor {
public:
   /// Default constructor
   Contractor() = default;

   /// Default copy constructor
   Contractor(const Contractor&) = default;

   /// No assignment
   Contractor& operator=(const Contractor&) = delete;

   /// Virtual destructor
   virtual ~Contractor();

   /// Tests if this contractor depends on a variable referenced in a bitset
   /// @param bs a bitset
   /// @return the result of the test
   virtual bool dependsOn(const Bitset& bs) const = 0;

   /// @return the set of variable on which this depends on
   virtual Scope scope() const = 0;

   /// Contraction method
   /// @param reg interval region that is contracted
   /// @return a certificate of proof
   virtual Proof contract(IntervalRegion& reg) = 0;

   /// Output on a stream
   /// param os an output stream
   ///
   /// The default implementation does nothing.
   virtual void print(std::ostream& os) const;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Contractor& op);

/// Type of shared pointers on contractors
typedef std::shared_ptr<Contractor> SharedContractor;

} // namespace

#endif
