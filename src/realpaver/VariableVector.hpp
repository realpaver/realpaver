///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_VARIABLE_VECTOR_HPP
#define REALPAVER_VARIABLE_VECTOR_HPP

#include <memory>
#include <vector>
#include "realpaver/Variable.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the representation of a vector of variables.
///////////////////////////////////////////////////////////////////////////////
class VariableVectorRep {
public:
   /// Constructor
   /// @param name base name
   /// @param first index of the first variable in this
   /// @param last index of the last variable in this
   VariableVectorRep(const std::string& name, int first, int last);

   /// Default destructor
   ~VariableVectorRep() = default;

   /// No copy
   VariableVectorRep(const VariableVectorRep&) = delete;

   /// No assignment
   VariableVectorRep& operator=(const VariableVectorRep&) = delete;

   /// Output on a stream
   /// @param os a stream
   void print(std::ostream& os) const;

   /// @return the size of this
   int size() const;

   /// @return the index of the first variable of this
   int firstIndex() const;

   /// @return the index of the last variable of this
   int lastIndex() const;

   /// Gets one variable
   /// @param i an index of this
   /// @return the variable at index i in this
   Variable get(int i) const;

   /// Assigns the domain of all the variables of this
   /// @param x an interval
   void setDomain(const Interval& x);

   /// Defines all the variables as integer variables
   void setInteger();

   /// Defines all the variables as binary variables
   void setBinary();

   /// Sets the tolerance of all the variables of this
   /// @param tol new tolerance
   void setTolerance(const Tolerance& tol);

   /// @return the hash code of this
   size_t hashCode() const;

private:
   std::string name_;            // base name
   int first_, last_;            // bounds of the variable indexes
   std::vector<Variable> vars_;  // the variables
};

///////////////////////////////////////////////////////////////////////////////
/// This is a vector of variables.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class VariableVector {
public:
   /// Constructor of a vector of continuous variables
   /// @param name base name
   /// @param first index of the first variable in this
   /// @param last index of the last variable in this
   VariableVector(const std::string& name, int first, int last);

   /// Default copy constructor
   VariableVector(const VariableVector&) = default;

   /// Default assignment
   VariableVector& operator=(const VariableVector&) = default;

   /// Default destructor
   ~VariableVector() = default;

   /// Output on a stream
   /// @param os a stream
   void print(std::ostream& os) const;

   /// @return the size of this
   int size() const;

   /// @return the index of the first variable of this
   int firstIndex() const;

   /// @return the index of the last variable of this
   int lastIndex() const;

   /// Gets one variable
   /// @param i an index of this
   /// @return the variable at index i in this
   Variable get(int i) const;

   /// Gets one variable
   /// @param i an index of this
   /// @return the variable at index i in this
   Variable operator[](int i) const;

   /// Assigns the domain of all the variables of this
   /// @param x an interval
   /// @return a reference to this
   VariableVector& setDomain(const Interval& x);

   /// Defines all the variables as integer variables
   /// @return a reference to this
   VariableVector& setInteger();

   /// Defines all the variables as binary variables
   /// @return a reference to this
   VariableVector& setBinary();

   /// Sets the tolerance of all the variables of this
   /// @param tol new tolerance
   /// @return a reference to this
   VariableVector& setTolerance(const Tolerance& tol);

   /// @return the hash code of this
   size_t hashCode() const;

private:
   std::shared_ptr<VariableVectorRep> rep_;
};

/// Output on a stream
/// @param os a stream
/// @param v a variable vector written on os
/// @return os
std::ostream& operator<<(std::ostream& os, const VariableVector& v);

} // namespace

#endif
