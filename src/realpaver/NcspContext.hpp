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
 * @file   NcspContext.hpp
 * @brief  NCSP search context
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_NCSP_CONTEXT_HPP
#define REALPAVER_NCSP_CONTEXT_HPP

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>
#include "realpaver/IntervalSmearSumRel.hpp"
#include "realpaver/Variable.hpp"

namespace realpaver {

///  Type of informations that can be associated with NCSP search nodes
enum class NcspNodeInfoType {
   SplitVar,      ///< selected variable in a splitting step
   NbCID          ///< number of CID contractors
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, NcspNodeInfoType typ);

/*----------------------------------------------------------------------------*/

/// Base class of informations that can be associated with NCSP search nodes
class NcspNodeInfo {
public:
   /// Constructor
   NcspNodeInfo(NcspNodeInfoType typ);

   /// Virtual destructor
   virtual ~NcspNodeInfo();

   /// Default copy constructor
   NcspNodeInfo(const NcspNodeInfo&) = default;

   /// Default assignment operator
   NcspNodeInfo& operator=(const NcspNodeInfo&) = default;

   /// Returns the type of this
   NcspNodeInfoType getType() const;

private:
   NcspNodeInfoType typ_;  // type
};

/*----------------------------------------------------------------------------*/

/// Information that contains a variable
class NcspNodeInfoVar : public NcspNodeInfo {
public:
   /// Constructor
   NcspNodeInfoVar(Variable v);

   /// Default destructor
   ~NcspNodeInfoVar() = default;

   /// Default copy constructor
   NcspNodeInfoVar(const NcspNodeInfoVar&) = default;

   /// No assignment
   NcspNodeInfoVar& operator=(const NcspNodeInfoVar&) = delete;

   /// Returns the variable enclosed in this
   Variable getVar() const;

private:
   Variable v_;   // variable
};

/*----------------------------------------------------------------------------*/

/// Information that contains a number of CID contractors
class NcspNodeInfoCID : public NcspNodeInfo {
public:
   /// Constructor
   NcspNodeInfoCID(int nbcid = 0);

   /// Default destructor
   ~NcspNodeInfoCID() = default;

   /// Default copy constructor
   NcspNodeInfoCID(const NcspNodeInfoCID&) = default;

   /// No assignment
   NcspNodeInfoCID& operator=(const NcspNodeInfoCID&) = delete;

   /// Returns the number of CID contractors
   size_t getNbCID() const;

   /// Assigns the number of CID contractors
   void setNbCID(size_t nbcid);

private:
   size_t nbcid_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Map that stores the informations associated with NCSP search nodes.
 *
 * An entry is a couple (node index, list of infos).
 */
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
   void insert(int index, std::shared_ptr<NcspNodeInfo> info);

   /// Removes all the informations associated with a node
   void remove(int index);

   /**
    * @brief Gets an information associated with a node.
    * 
    * Returns the information associated with the node index and the given type
    * if it exists, a null pointer otherwise
    */
   std::shared_ptr<NcspNodeInfo> getInfo(int index, NcspNodeInfoType typ) const;

   /// Returns the number of entries in this
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
