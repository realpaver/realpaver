///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_OBJECTIVE_HPP
#define REALPAVER_OBJECTIVE_HPP

#include "realpaver/Term.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the shared representation of an objective function
///////////////////////////////////////////////////////////////////////////////
class ObjectiveRep {
public:
   /// Creates the representation of an objective function
   /// @param f expression of the function
   /// @param minimization sense
   ///        - true for minimization
   ///        - false for maximization
   ObjectiveRep(Term f, bool minimization);

   /// No copy
   ObjectiveRep(const ObjectiveRep&) = delete;

   /// No assignment
   ObjectiveRep& operator=(const ObjectiveRep&) = delete;

   /// Default destructor
   ~ObjectiveRep() = default;

   /// @return the expression of this
   Term getTerm() const;

   /// @return true if this is constant (variable free)
   bool isConstant() const;

   /// Dependency test
   /// @param v a variable
   /// @return true if this depends on v
   bool dependsOn(const Variable& v) const;

   /// @return true if this is linear
   bool isLinear() const;

   /// @return true if this has to be minimized
   bool isMinimization() const;

   /// @return true if this has to be maximized
   bool isMaximization() const;

private:
   Term f_;
   bool minimization_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is an objective function.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class Objective {
public:
   /// Default copy constructor
   Objective(const Objective&) = default;

   /// Default assignment operator
   Objective& operator=(const Objective&) = default;

   /// Default destructor
   ~Objective() = default;

   /// @return the expression of this
   Term getTerm() const;

   /// @return true if this is constant (variable free)
   bool isConstant() const;

   /// Dependency test
   /// @param v a variable
   /// @return true if this depends on v
   bool dependsOn(const Variable& v) const;

   /// @return true if this is linear
   bool isLinear() const;

   /// @return true if this has to be minimized
   bool isMinimization() const;

   /// @return true if this has to be maximized
   bool isMaximization() const;

   friend Objective MIN(Term f);
   friend Objective MAX(Term f);

private:
   std::shared_ptr<ObjectiveRep> rep_;

   Objective(Term f, bool minimization);
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, Objective obj);

/// Creates an objective function
/// @param f expression
/// @return minimize f
Objective MIN(Term f);

/// Creates an objective function
/// @param f expression
/// @return maximize f
Objective MAX(Term f);

} // namespace

#endif
