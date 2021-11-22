// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONTRACTOR_HPP
#define REALPAVER_CONTRACTOR_HPP

#include "realpaver_bitset.hpp"
#include "realpaver_box.hpp"
#include "realpaver_scope.hpp"

namespace realpaver {

/*****************************************************************************
 * Base class of contractors used to reduce interval vectors.
 *****************************************************************************/
class Contractor {
public:
   // constructor
   Contractor() = default;

   // destructor
   virtual ~Contractor();

   // returns true if this contractor depends on a variable
   // referenced in a bitset
   virtual bool dependsOn(const Bitset& bs) const = 0;

   // scope
   virtual Scope scope() const = 0;

   // contraction method
   virtual Proof contract(Box& B) = 0;

   // output on a stream
   virtual void print(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const Contractor& op);

} // namespace

#endif
