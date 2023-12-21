///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_VARIABLE_HPP
#define REALPAVER_VARIABLE_HPP

#include <memory>
#include "realpaver/Domain.hpp"
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

   /// Destructor
   ~VariableRep();

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
   Domain* getDomain() const;

   /// Sets the domain of this
   /// @param dom new domain of this
   void setDomain(Domain* dom);

   /// @return true if this is a binary variable, false otherwise
   bool isBinary() const;

   /// @return true if this is an integer variable, false otherwise
   bool isInteger() const;

   /// @return true if this is a real variable, false otherwise
   bool isReal() const;

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
   Domain* dom_;
   Tolerance tol_;

   static int NEXT_ID;
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
   ///
   /// The default domain is the interval universe.
   Variable(const std::string& name);

   /// Creates a variable having no representation
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
   Domain* getDomain() const;

   /// Sets the domain of this
   /// @param dom new domain of this
   ///
   /// throws an exception if dom is null
   Variable& setDomain(std::unique_ptr<Domain> dom);

   /// @return true if this is an integer variable in [0, 1]
   bool isBinary() const;

   /// @return true if this is an integer variable, false otherwise
   bool isInteger() const;

   /// @return true if this is a real variable, false otherwise
   bool isReal() const;

   /// @return the hash code of this
   size_t hashCode() const;

   /// @return the tolerance of this (output precision of interval solutions)
   Tolerance getTolerance() const;

   /// Sets the tolerance of this
   /// @param tol new tolerance of this
   /// @return a reference to this
   Variable& setTolerance(const Tolerance& tol);

   /// @return a clone of this with a new representation
   Variable clone() const;

   /// Tests the equality of two variables
   /// @param other a variable
   /// @return true if this and other have the same identifier, false otherwise
   bool operator==(const Variable& other) const;

   /// @return true if it has been created with the default constructor, false
   ///         if this has a representation (i.e. it is a true variable)
   bool hasNullPointer() const;

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

///////////////////////////////////////////////////////////////////////////////
/// This is an equality comparator of variables.
///
/// This implements a function call operator that returns true if two
/// variables have the same id.
///////////////////////////////////////////////////////////////////////////////
struct VariableEqual {
   /// @param v a variable
   /// @param w a variable
   /// @return true if v and w represent the same variable
   bool operator()(const Variable& v, const Variable& w) const
   {
      return v.id() == w.id();
   }
};

} // namespace

#endif
