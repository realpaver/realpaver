///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_VARIABLE_HPP
#define REALPAVER_VARIABLE_HPP

#include <memory>
#include "realpaver/Interval.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the shared representation of a variable in a problem.
///////////////////////////////////////////////////////////////////////////////
class VariableRep {
public:
   /// Creates a variable representation
   /// @param name of the variable
   VariableRep(const std::string& name);

   /// No copy
   VariableRep(const VariableRep&) = delete;

   /// No assignment
   VariableRep& operator=(const VariableRep&) = delete;

   /// Default destructor
   ~VariableRep() = default;

   /// @return the unique identifier of this
   size_t id() const;

   /// Sets the unique identifier of this
   /// @param id new identifier of this
   void setId(size_t id);

   /// @return the name of this
   std::string getName() const;

   /// Sets the name of this
   /// @param name new name of this
   void setName(const std::string& name);

   /// @return the domain of this
   Interval getDomain() const;

   /// Sets the domain of this
   /// @param x new domain of this
   void setDomain(const Interval& x);

   /// @return true if this is discrete, false otherwise
   bool isDiscrete() const;

   /// Sets this as a discrete variable
   void setDiscrete();

   /// @return true if this is continuous, false otherwise
   bool isContinuous() const;

   /// Sets this as a continuous variable
   void setContinuous();

   /// @return the hash code of this
   size_t hashCode() const;

   /// @return the tolerance of this (output precision of interval solutions)
   Tolerance getTolerance() const;

   /// Sets the tolerance of this
   /// @param tol new tolerance of this
   void setTolerance(const Tolerance& tol);

private:
   std::string name_;
   size_t id_;
   Interval domain_;
   bool continuous_;
   Tolerance tol_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a variable in a problem.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class Variable {
public:
   /// Creates a variable
   /// @param name of the variable
   Variable(const std::string& name);

   /// Creates a variable with no representation
   Variable();

   /// Default copy constructor
   Variable(const Variable&) = default;

   /// Default assignment operator
   Variable& operator=(const Variable&) = default;

   /// Default destructor
   ~Variable() = default;

   /// @return the unique identifier of this
   size_t id() const;

   /// Sets the unique identifier of this
   /// @param id new identifier of this
   Variable& setId(size_t id);

   /// @return the name of this
   std::string getName() const;

   /// Sets the name of this
   /// @param name new name of this
   Variable& setName(const std::string& name);

   /// @return the domain of this
   Interval getDomain() const;

   /// Sets the domain of this
   /// @param x new domain of this
   Variable& setDomain(const Interval& x);

   /// @return true if this is discrete, false otherwise
   bool isDiscrete() const;

   /// @return true if this is a discrete variable in [0, 1]
   bool isDiscreteBool() const;

   /// @return true if this is a discrete variable not in [0, 1]
   bool isDiscreteInt() const;

   /// Sets this as a discrete variable
   /// @return a reference to this
   Variable& setDiscrete();

   /// @return true if this is continuous, false otherwise
   bool isContinuous() const;

   /// Sets this as a continuous variable
   /// @return a reference to this
   Variable& setContinuous();

   /// @return the hash code of this
   size_t hashCode() const;

   /// @return the tolerance of this (output precision of interval solutions)
   Tolerance getTolerance() const;

   /// Sets the tolerance of this
   /// @param tol new tolerance of this
   Variable& setTolerance(const Tolerance& tol);

   /// @return a clone of this with a new representation
   Variable clone() const;

   /// Tests the equality of two variables
   /// @param other a variable
   /// @return true if this and other have the same identifier, false otherwise
   bool operator==(const Variable& other) const;

   /// @return true if this has no representation, i.e. it has been created
   ///         with the default constructor
   bool isNull() const;

private:
   std::shared_ptr<VariableRep> rep_;
};

/// Output on a stream
/// @param os a stream
/// @param v a variable written on os
/// @return os
std::ostream& operator<<(std::ostream& os, const Variable& v);

///////////////////////////////////////////////////////////////////////////////
/// This is a variable hasher.
///
/// This implements a function call operator that simply returns the hash code
/// of a variable.
///////////////////////////////////////////////////////////////////////////////
struct VariableHasher {
   /// Hash function
   /// @param v a variable
   /// @return the hash code of 'v'
   std::size_t operator()(const Variable& v) const
   {
      return v.hashCode();
   }
};

} // namespace

#endif
