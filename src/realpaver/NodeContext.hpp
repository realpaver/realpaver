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
 * @file   NodeContext.hpp
 * @brief  Context for search nodes
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_NODE_CONTEXT_HPP
#define REALPAVER_NODE_CONTEXT_HPP

#include "realpaver/Variable.hpp"
#include <list>
#include <memory>
#include <unordered_map>

namespace realpaver {

///  Type of informations that can be associated with search nodes
enum class NodeInfoType {
   SplitVar, ///< selected variable in a splitting step
   NbCID     ///< number of CID contractors
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, NodeInfoType typ);

/*----------------------------------------------------------------------------*/

/// Base class of informations that can be associated with search nodes
class NodeInfo {
public:
   /// Constructor
   NodeInfo(NodeInfoType typ);

   /// Virtual destructor
   virtual ~NodeInfo();

   /// Default copy constructor
   NodeInfo(const NodeInfo &) = default;

   /// Default assignment operator
   NodeInfo &operator=(const NodeInfo &) = default;

   /// Returns the type of this
   NodeInfoType getType() const;

private:
   NodeInfoType typ_; // type
};

/*----------------------------------------------------------------------------*/

/// Information that contains a variable
class NodeInfoVar : public NodeInfo {
public:
   /// Constructor
   NodeInfoVar(Variable v);

   /// Default destructor
   ~NodeInfoVar() = default;

   /// Default copy constructor
   NodeInfoVar(const NodeInfoVar &) = default;

   /// No assignment
   NodeInfoVar &operator=(const NodeInfoVar &) = delete;

   /// Returns the variable enclosed in this
   Variable getVar() const;

private:
   Variable v_; // variable
};

/*----------------------------------------------------------------------------*/

/// Information that contains a number of CID contractors
class NodeInfoCID : public NodeInfo {
public:
   /// Constructor
   NodeInfoCID(int nbcid = 0);

   /// Default destructor
   ~NodeInfoCID() = default;

   /// Default copy constructor
   NodeInfoCID(const NodeInfoCID &) = default;

   /// No assignment
   NodeInfoCID &operator=(const NodeInfoCID &) = delete;

   /// Returns the number of CID contractors
   size_t getNbCID() const;

   /// Assigns the number of CID contractors
   void setNbCID(size_t nbcid);

private:
   size_t nbcid_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Map that stores the informations associated with search nodes.
 *
 * An entry is a couple (node index, list of infos).
 */
class NodeContext {
public:
   /// Default constructor
   NodeContext() = default;

   /// Default destructor
   ~NodeContext() = default;

   /// No copy
   NodeContext(const NodeContext &) = delete;

   /// No assignment
   NodeContext &operator=(const NodeContext &) = delete;

   /// Inserts a new entry in this
   void insert(int index, std::shared_ptr<NodeInfo> info);

   /// Removes all the informations associated with a node
   void remove(int index);

   /**
    * @brief Gets an information associated with a node.
    *
    * Returns the information associated with the node index and the given type
    * if it exists, a null pointer otherwise
    */
   std::shared_ptr<NodeInfo> getInfo(int index, NodeInfoType typ) const;

   /// Returns the number of entries in this
   size_t size() const;

private:
   // map node index -> list of infos
   typedef std::list<std::shared_ptr<NodeInfo>> ListType;
   std::unordered_map<int, ListType> map_;

   // test used in assertions
   bool hasInfo(int index, NodeInfoType typ) const;
};

} // namespace realpaver

#endif
