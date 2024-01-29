///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_CONTEXT_HPP
#define REALPAVER_NCSP_CONTEXT_HPP

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
   NbCID          // number of CID contractors
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
/// This is an information that contains a number of CID contractors.
///////////////////////////////////////////////////////////////////////////////
class NcspNodeInfoCID : public NcspNodeInfo {
public:
   /// Constructor
   /// @param nbcid number of CID contractors applied
   NcspNodeInfoCID(int nbcid = 0);

   /// Default destructor
   ~NcspNodeInfoCID() = default;

   /// Default copy constructor
   NcspNodeInfoCID(const NcspNodeInfoCID&) = default;

   /// No assignment
   NcspNodeInfoCID& operator=(const NcspNodeInfoCID&) = delete;

   /// @return the number of CID contractors
   size_t getNbCID() const;

   /// Assigns the number of CID contractors
   /// @param nbcid the new value
   void setNbCID(size_t nbcid);

private:
   size_t nbcid_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a map that stores the informations associated with search nodes.
///
/// An entry is a couple (node index, list of infos).
///////////////////////////////////////////////////////////////////////////////
class NcspContext {
public:
   /// Default constructor
   NcspContext() = default;

   /// Default destructor
   ~NcspContext() = default;

   /// No copy
   NcspContext(const NcspContext&) = delete;

   /// No assignment
   NcspContext& operator=(const NcspContext&) = delete;

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
