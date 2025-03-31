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
 * @file   CSPSpaceHybridDFS.hpp
 * @brief  Hybrid strategies
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CSP_SPACE_HYBRID_DFS_HPP
#define REALPAVER_CSP_SPACE_HYBRID_DFS_HPP

#include "realpaver/CSPSpace.hpp"
#include <set>
#include <vector>

namespace realpaver {

/// Criteria used to order sets of boxes
enum class HybridDFSStyle {
   Depth,        ///< depth of a node
   Perimeter,    ///< perimeter of a box
   GridPerimeter ///< grid perimeter of a box
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const HybridDFSStyle &style);

/*----------------------------------------------------------------------------*/

/// Abstract class representing a set of shared CSP nodes
class HybridCSPNodeSet {
public:
   /// Constructor
   HybridCSPNodeSet() = default;

   /// Virtual destructor
   virtual ~HybridCSPNodeSet();

   /// Default copy constructor
   HybridCSPNodeSet(const HybridCSPNodeSet &) = default;

   /// No assignment
   HybridCSPNodeSet &operator=(const HybridCSPNodeSet &) = delete;

   /// Returns true if this is empty
   virtual bool isEmpty() const = 0;

   /// Returns the size of this
   virtual size_t size() const = 0;

   /// Inserts a node in this
   virtual void insert(const SharedCSPNode &node) = 0;

   /// Extracts the first node of this
   virtual SharedCSPNode extract() = 0;

   /// Returns the i-th node of this
   virtual SharedCSPNode getNode(size_t i) const = 0;
};

/*----------------------------------------------------------------------------*/

/// Set of shared CSP nodes ordered by depth (ascending)
class DepthCSPNodeSet : public HybridCSPNodeSet {
public:
   /// Constructor of an empty set
   DepthCSPNodeSet() = default;

   /// Default destructor
   ~DepthCSPNodeSet() = default;

   /// Default copy constructor
   DepthCSPNodeSet(const DepthCSPNodeSet &) = default;

   /// No assignment
   DepthCSPNodeSet &operator=(const DepthCSPNodeSet &) = delete;

   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedCSPNode &node) override;
   SharedCSPNode extract() override;
   SharedCSPNode getNode(size_t i) const override;

private:
   struct Elem {
      SharedCSPNode node;
      int depth;
   };

   struct Comp {
      bool operator()(const Elem &e1, const Elem &e2) const
      {
         return (e1.depth < e2.depth) ||
                (e1.depth == e2.depth && e1.node->index() < e2.node->index());
      }
   };

   std::set<Elem, Comp> set_;
};

/*----------------------------------------------------------------------------*/

/// Set of shared CSP nodes ordered by perimeter (descending)
class PerimeterCSPNodeSet : public HybridCSPNodeSet {
public:
   /// Constructor of an empty set
   PerimeterCSPNodeSet() = default;

   /// Default destructor
   ~PerimeterCSPNodeSet() = default;

   /// Default copy constructor
   PerimeterCSPNodeSet(const PerimeterCSPNodeSet &) = default;

   /// No assignment
   PerimeterCSPNodeSet &operator=(const PerimeterCSPNodeSet &) = delete;

   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedCSPNode &node) override;
   SharedCSPNode extract() override;
   SharedCSPNode getNode(size_t i) const override;

private:
   struct Elem {
      SharedCSPNode node;
      double peri;
   };

   struct Comp {
      bool operator()(const Elem &e1, const Elem &e2) const
      {
         return (e1.peri > e2.peri) ||
                (e1.peri == e2.peri && e1.node->index() < e2.node->index());
      }
   };

   std::set<Elem, Comp> set_;
};

/*----------------------------------------------------------------------------*/

/// Set of shared CSP nodes ordered by grid perimeter (descending)
class GridPerimeterCSPNodeSet : public HybridCSPNodeSet {
public:
   /// Constructor of an empty set
   GridPerimeterCSPNodeSet() = default;

   /// Default destructor
   ~GridPerimeterCSPNodeSet() = default;

   /// Default copy constructor
   GridPerimeterCSPNodeSet(const GridPerimeterCSPNodeSet &) = default;

   /// No assignment
   GridPerimeterCSPNodeSet &operator=(const GridPerimeterCSPNodeSet &) = delete;

   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedCSPNode &node) override;
   SharedCSPNode extract() override;
   SharedCSPNode getNode(size_t i) const override;

private:
   struct Elem {
      SharedCSPNode node;
      double peri;
   };

   struct Comp {
      bool operator()(const Elem &e1, const Elem &e2) const
      {
         return (e1.peri > e2.peri) ||
                (e1.peri == e2.peri && e1.node->index() < e2.node->index());
      }
   };

   std::set<Elem, Comp> set_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Hybrid Best-First Depth-First search strategies.
 *
 * A DFS strategy is used to find the next solution. When a solution is found,
 * the best pending node is selected according to a given ordering, e.g. the
 * highest node in the search tree or the one having the largest perimeter.
 *
 * @see HybridDFSStyle
 */
class CSPSpaceHybridDFS : public CSPSpace {
public:
   /// Constructor
   CSPSpaceHybridDFS(HybridDFSStyle style);

   /// Destructor
   ~CSPSpaceHybridDFS();

   /// No copy
   CSPSpaceHybridDFS(const CSPSpaceHybridDFS &) = delete;

   /// No assignment
   CSPSpaceHybridDFS &operator=(const CSPSpaceHybridDFS &) = delete;

   size_t nbSolNodes() const override;
   void pushSolNode(const SharedCSPNode &node) override;
   SharedCSPNode popSolNode() override;
   SharedCSPNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   size_t nbPendingNodes() const override;
   SharedCSPNode nextPendingNode() override;
   void insertPendingNode(const SharedCSPNode &node) override;
   SharedCSPNode getPendingNode(size_t i) const override;
   void insertPendingNodes(CSPSplit::iterator first, CSPSplit::iterator last) override;

private:
   std::vector<SharedCSPNode> sta_;  // stack of pending nodes (for DFS)
   HybridCSPNodeSet *set_;           // set of pending nodes (for BestFS)
   std::vector<SharedCSPNode> vsol_; // vector of solution nodes
   bool leftRight_;                  // true if the current DFS stage is
                                     // left-to-right, false for right-to-left
};

} // namespace realpaver

#endif
