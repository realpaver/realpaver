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
 * @file   BoxReporter.hpp
 * @brief  Reporter of solutions of problems
 * @author Laurent Granvilliers
 * @date   2024-12-19
 */

#ifndef REALPAVER_BOX_REPORTER_HPP
#define REALPAVER_BOX_REPORTER_HPP

#include <iostream>
#include <vector>
#include "realpaver/Alias.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/Variable.hpp"

namespace realpaver {

class Problem;

/// Abstract class of entities of solutions that are reported
class EntityReported {
public:
   /// Default constructor
   EntityReported() = default;

   /// Virtual destructor
   virtual ~EntityReported();

   /// Default copy constructor
   EntityReported(const EntityReported&) = default;

   /// No assignment
   EntityReported& operator=(const EntityReported&) = delete;

   /// Returns the name of this
   virtual std::string name() const = 0;

   /// Returns the value of this in a box
   virtual std::unique_ptr<Domain> domain(const DomainBox& box) const = 0;
};

/*----------------------------------------------------------------------------*/

/// Variable reported in a solution
class VariableReported : public EntityReported {
public:
   /// Constructor
   VariableReported(Variable v);

   /// Default destructor
   ~VariableReported() = default;

   /// Default copy constructor
   VariableReported(const VariableReported&) = default;

   /// No assignment
   VariableReported& operator=(const VariableReported&) = delete;

   std::string name() const override;
   std::unique_ptr<Domain> domain(const DomainBox& box) const override;

private:
   Variable v_;   // variable enclosed
};

/*----------------------------------------------------------------------------*/

/// Alias reported in a solution
class AliasReported : public EntityReported {
public:
   /// Constructor
   AliasReported(Alias a);

   /// Default destructor
   ~AliasReported() = default;

   /// Default copy constructor
   AliasReported(const AliasReported&) = default;

   /// No assignment
   AliasReported& operator=(const AliasReported&) = delete;

   std::string name() const override;
   std::unique_ptr<Domain> domain(const DomainBox& box) const override;

private:
   Alias a_;
};

/*----------------------------------------------------------------------------*/

class EntityReportedVector {
public:
   /// Default constructor
   EntityReportedVector() = default;

   /// Default copy constructor
   EntityReportedVector(const EntityReportedVector&) = default;

   /// No assignment
   EntityReportedVector& operator=(const EntityReportedVector&) = delete;

   /// Default destructor
   ~EntityReportedVector() = default;

   /// Returns the number of entities in this
   size_t size() const;

   /// Access to the i-th entity in this with 0 <= i < size()-1
   std::shared_ptr<EntityReported> get(size_t i) const;

   /// Adds a variable in the last place in this
   void addVariable(Variable v);

   /// Adds an alias in the last place in this
   void addAlias(Alias a);

   /// Returns true if this contains an entity given its name
   bool contains(const std::string& name) const;

   /// Removes an entity given its name from this
   void remove(const std::string& name);

   /// Returns the length of the longest name in this
   size_t maxNameLength() const;

private:
   std::vector<std::shared_ptr<EntityReported>> ents_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Reporter of solutions.
 *
 * A reporter contains a list of variables and a list of aliases.
 * It is used to report the solutions after solving a problem.
 * Only the variables and aliases enclosed are considered.
 */
class BoxReporter {
public:
   /// Constructor of empty reporter
   BoxReporter();

   /// Constructor of reporter on a problem
   BoxReporter(const Problem& p);

   /// Virtual destructor
   virtual ~BoxReporter();

   /// Default copy constructor
   BoxReporter(const BoxReporter&) = default;

   /// No assignment
   BoxReporter& operator=(const BoxReporter&) = delete;

   /// Adds a variable in the last place in this
   void addVariable(Variable v);

   /// Adds an alias in the last place in this
   void addAlias(Alias a);

   /// Removes an entity given its name from this
   void remove(const std::string& name);

   /// Returns the length of the longest name in this
   size_t maxNameLength() const;

   /// Abstract reporting method
   virtual void report(const DomainBox& box) = 0;

protected:
   EntityReportedVector ents_;  // vector of entities
};

/*----------------------------------------------------------------------------*/

/// Reporting of solutions on a stream
class StreamReporter : public BoxReporter {
public:
   /// Constructor of an empty reporter given an output stream
   StreamReporter(std::ostream& os);

   /// Constructor of a reporter on a problem given an output stream
   StreamReporter(const Problem& p, std::ostream& os);

   /// Default destructor
   ~StreamReporter() = default;

   /// Default copy constructor
   StreamReporter(const StreamReporter&) = default;

   /// No assignment
   StreamReporter& operator=(const StreamReporter&) = delete;

   /// Returns the stream enclosed in this
   std::ostream* get() const;

   /**
    * @brief Assigns a display flag.
    *
    * If b is true then each entity is reported ojn a separated line.
    * Otherwise all the entities are displayed on a single line.
    */
   void setVertical(bool b = true);

   void report(const DomainBox& box) override;

private:
   std::ostream* os_;  // output stream
   bool vertical_;     // yes: each entity is reported on a separated line
                       // no:  all the entities are displayed on a single line
};

} // namespace

#endif
