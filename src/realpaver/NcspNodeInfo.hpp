///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_NODE_INFO_HPP
#define REALPAVER_NCSP_NODE_INFO_HPP

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>
#include "realpaver/IntervalSmearSumRel.hpp"
#include "realpaver/Variable.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// Type of informations that can be associated with search nodes.
///////////////////////////////////////////////////////////////////////////////
enum class NcspNodeInfoType {
   SplitVar,      // selected variable in a splitting step
   SmearSumRel    // smar relative values of variables
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, NcspNodeInfoType typ);

///////////////////////////////////////////////////////////////////////////////
/// This is the abstract base class of informations that can be associated with
/// search nodes.
///////////////////////////////////////////////////////////////////////////////
class NcspNodeInfo {
public:
   /// Constructor
   /// @param typ information type
   NcspNodeInfo(NcspNodeInfoType typ);

   /// Virtual destructor
   virtual ~NcspNodeInfo();

   /// Default copy constructor
   NcspNodeInfo(const NcspNodeInfo&) = default;

   /// Default assignment operator
   NcspNodeInfo& operator=(const NcspNodeInfo&) = default;

   /// @return the type of this
   NcspNodeInfoType getType() const;

private:
   NcspNodeInfoType typ_;  // type
};

///////////////////////////////////////////////////////////////////////////////
/// This is an information that contains a variable.
///////////////////////////////////////////////////////////////////////////////
class NcspNodeInfoVar : public NcspNodeInfo {
public:
   /// Constructor
   /// @param v a variable
   NcspNodeInfoVar(Variable v);

   /// Default destructor
   ~NcspNodeInfoVar() = default;

   /// Default copy constructor
   NcspNodeInfoVar(const NcspNodeInfoVar&) = default;

   /// No assignment
   NcspNodeInfoVar& operator=(const NcspNodeInfoVar&) = delete;

   /// @return the variable enclosed in this
   Variable getVar() const;

private:
   Variable v_;   // variable
};

///////////////////////////////////////////////////////////////////////////////
/// This is an information that contains smear sum relative values.
///
/// See the IntervalSmearSumRel class.
///////////////////////////////////////////////////////////////////////////////
class NcspNodeInfoSSR : public NcspNodeInfo {
public:
   /// Constructor
   /// @param f a function
   NcspNodeInfoSSR(std::shared_ptr<IntervalSmearSumRel> obj);

   /// Default destructor
   ~NcspNodeInfoSSR() = default;

   /// Copy constructor
   /// @param other value assigned to this, except the sorted vector
   NcspNodeInfoSSR(const NcspNodeInfoSSR& other);

   /// No assignment
   NcspNodeInfoSSR& operator=(const NcspNodeInfoSSR&) = delete;

   /// @return the scope of this
   Scope scope() const;

   /// Calculates the smearSumRel value of the variables in a box
   /// @param B an interval box
   void calculate(const IntervalBox& B);

   /// Gets the smearSumRel value of a variable
   /// @param v a variable
   /// @return the smear sum rel value of v in this
   double getSmearSumRel(const Variable& v) const;

   /// Sorts the variables by decreasing ordering of their smearSumRel values
   /// It lmust be done after a call to calculateSSR.
   void sort();

   /// @return true if the variables have been sorted, false otherwise
   bool isSorted() const;

   /// Access to a variable after sorting
   /// @param i an index between 0 and n-1 where n is the number of variables
   /// @return the i-th sorted variable
   Variable getSortedVar(size_t i) const;

   /// @return the size of the scope of this
   size_t nbVars() const;

private:
   struct Item {
      Variable var;  // variable
      double val;    // smearSumRel of var
   };

   struct CompItem {
      bool operator()(const Item itm1, const Item& itm2) const
      {
         return (itm1.val > itm2.val) ||
                (itm1.val == itm2.val && itm1.var.id() < itm2.var.id());
      }
   };

   std::shared_ptr<IntervalSmearSumRel> obj_;
   std::vector<Item> sv_;        // vector of smear sum rel values
   bool sorted_;                 // true if the vector has been sorted
};

///////////////////////////////////////////////////////////////////////////////
/// This is a map that stores the informations associated with search nodes.
///
/// An entry is a couple (node index, list of infos).
///////////////////////////////////////////////////////////////////////////////
class NcspNodeInfoMap {
public:
   /// Default constructor
   NcspNodeInfoMap() = default;

   /// Default destructor
   ~NcspNodeInfoMap() = default;

   /// No copy
   NcspNodeInfoMap(const NcspNodeInfoMap&) = delete;

   /// No assignment
   NcspNodeInfoMap& operator=(const NcspNodeInfoMap&) = delete;

   /// Inserts a new entry in this
   /// @param index a node index
   /// @param info an information for the node
   void insert(int index, std::shared_ptr<NcspNodeInfo> info);

   /// Removes all the informations associated with a node
   /// @param index a node index
   void remove(int index);

   /// Gets an information associated with a node
   /// @param index a node index
   /// @param typ an information type
   /// @return the information associated with the given node havng the given
   ///         type if it exists, a null pointer otherwise
   std::shared_ptr<NcspNodeInfo> getInfo(int index, NcspNodeInfoType typ) const;

   /// @return the number of entries in this
   size_t size() const;

private:
   // map node index -> list of infos
   typedef std::list< std::shared_ptr<NcspNodeInfo> > ListType;
   std::unordered_map<int, ListType> map_;

   // test used in assertions
   bool hasInfo(int index,  NcspNodeInfoType typ) const;
};

} // namespace

#endif
