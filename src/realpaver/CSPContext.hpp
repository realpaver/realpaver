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
 * @file   CSPContext.hpp
 * @brief  CSP search context
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CSP_CONTEXT_HPP
#define REALPAVER_CSP_CONTEXT_HPP

#include <list>
#include <memory>
#include <unordered_map>
#include "realpaver/Variable.hpp"

namespace realpaver {

///  Type of informations that can be associated with CSP search nodes
enum class CSPNodeInfoType {
   SplitVar,      ///< selected variable in a splitting step
   NbCID          ///< number of CID contractors
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, CSPNodeInfoType typ);

/*----------------------------------------------------------------------------*/

/// Base class of informations that can be associated with CSP search nodes
class CSPNodeInfo {
public:
   /// Constructor
   CSPNodeInfo(CSPNodeInfoType typ);

   /// Virtual destructor
   virtual ~CSPNodeInfo();

   /// Default copy constructor
   CSPNodeInfo(const CSPNodeInfo&) = default;

   /// Default assignment operator
   CSPNodeInfo& operator=(const CSPNodeInfo&) = default;

   /// Returns the type of this
   CSPNodeInfoType getType() const;

private:
   CSPNodeInfoType typ_;  // type
};

/*----------------------------------------------------------------------------*/

/// Information that contains a variable
class CSPNodeInfoVar : public CSPNodeInfo {
public:
   /// Constructor
   CSPNodeInfoVar(Variable v);

   /// Default destructor
   ~CSPNodeInfoVar() = default;

   /// Default copy constructor
   CSPNodeInfoVar(const CSPNodeInfoVar&) = default;

   /// No assignment
   CSPNodeInfoVar& operator=(const CSPNodeInfoVar&) = delete;

   /// Returns the variable enclosed in this
   Variable getVar() const;

private:
   Variable v_;   // variable
};

/*----------------------------------------------------------------------------*/

/// Information that contains a number of CID contractors
class CSPNodeInfoCID : public CSPNodeInfo {
public:
   /// Constructor
   CSPNodeInfoCID(int nbcid = 0);

   /// Default destructor
   ~CSPNodeInfoCID() = default;

   /// Default copy constructor
   CSPNodeInfoCID(const CSPNodeInfoCID&) = default;

   /// No assignment
   CSPNodeInfoCID& operator=(const CSPNodeInfoCID&) = delete;

   /// Returns the number of CID contractors
   size_t getNbCID() const;

   /// Assigns the number of CID contractors
   void setNbCID(size_t nbcid);

private:
   size_t nbcid_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Map that stores the informations associated with CSP search nodes.
 *
 * An entry is a couple (node index, list of infos).
 */
class CSPContext {
public:
   /// Default constructor
   CSPContext() = default;

   /// Default destructor
   ~CSPContext() = default;

   /// No copy
   CSPContext(const CSPContext&) = delete;

   /// No assignment
   CSPContext& operator=(const CSPContext&) = delete;

   /// Inserts a new entry in this
   void insert(int index, std::shared_ptr<CSPNodeInfo> info);

   /// Removes all the informations associated with a node
   void remove(int index);

   /**
    * @brief Gets an information associated with a node.
    *
    * Returns the information associated with the node index and the given type
    * if it exists, a null pointer otherwise
    */
   std::shared_ptr<CSPNodeInfo> getInfo(int index, CSPNodeInfoType typ) const;

   /// Returns the number of entries in this
   size_t size() const;

private:
   // map node index -> list of infos
   typedef std::list< std::shared_ptr<CSPNodeInfo> > ListType;
   std::unordered_map<int, ListType> map_;

   // test used in assertions
   bool hasInfo(int index,  CSPNodeInfoType typ) const;
};

} // namespace

#endif
