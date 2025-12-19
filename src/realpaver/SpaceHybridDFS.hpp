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
 * @file   SpaceHybridDFS.hpp
 * @brief  Search space for hybrid strategies
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_SPACE_HYBRID_DFS_HPP
#define REALPAVER_SPACE_HYBRID_DFS_HPP

#include "realpaver/Node.hpp"
#include "realpaver/Space.hpp"
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

/// Abstract class representing a set of shared nodes
class HybridNodeSet {
public:
   /// Constructor
   HybridNodeSet() = default;

   /// Virtual destructor
   virtual ~HybridNodeSet();

   /// Default copy constructor
   HybridNodeSet(const HybridNodeSet &) = default;

   /// No assignment
   HybridNodeSet &operator=(const HybridNodeSet &) = delete;

   /// Returns true if this is empty
   virtual bool isEmpty() const = 0;

   /// Returns the size of this
   virtual size_t size() const = 0;

   /// Inserts a node in this
   virtual void insert(const SharedNode &node) = 0;

   /// Extracts the first node of this
   virtual SharedNode extract() = 0;

   /// Returns the i-th node of this
   virtual SharedNode getNode(size_t i) const = 0;
};

/*----------------------------------------------------------------------------*/

/// Set of shared nodes ordered by depth (ascending)
class DepthNodeSet : public HybridNodeSet {
public:
   /// Constructor of an empty set
   DepthNodeSet() = default;

   /// Default destructor
   ~DepthNodeSet() = default;

   /// Default copy constructor
   DepthNodeSet(const DepthNodeSet &) = default;

   /// No assignment
   DepthNodeSet &operator=(const DepthNodeSet &) = delete;

   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedNode &node) override;
   SharedNode extract() override;
   SharedNode getNode(size_t i) const override;

private:
   struct Elem {
      SharedNode node;
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

/// Set of shared nodes ordered by perimeter (descending)
class PerimeterNodeSet : public HybridNodeSet {
public:
   /// Constructor of an empty set
   PerimeterNodeSet() = default;

   /// Default destructor
   ~PerimeterNodeSet() = default;

   /// Default copy constructor
   PerimeterNodeSet(const PerimeterNodeSet &) = default;

   /// No assignment
   PerimeterNodeSet &operator=(const PerimeterNodeSet &) = delete;

   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedNode &node) override;
   SharedNode extract() override;
   SharedNode getNode(size_t i) const override;

private:
   struct Elem {
      SharedNode node;
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

/// Set of shared nodes ordered by grid perimeter (descending)
class GridPerimeterNodeSet : public HybridNodeSet {
public:
   /// Constructor of an empty set
   GridPerimeterNodeSet() = default;

   /// Default destructor
   ~GridPerimeterNodeSet() = default;

   /// Default copy constructor
   GridPerimeterNodeSet(const GridPerimeterNodeSet &) = default;

   /// No assignment
   GridPerimeterNodeSet &operator=(const GridPerimeterNodeSet &) = delete;

   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedNode &node) override;
   SharedNode extract() override;
   SharedNode getNode(size_t i) const override;

private:
   struct Elem {
      SharedNode node;
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
class SpaceHybridDFS : public Space<Node> {
public:
   /// Constructor
   SpaceHybridDFS(HybridDFSStyle style);

   /// Destructor
   ~SpaceHybridDFS();

   /// No copy
   SpaceHybridDFS(const SpaceHybridDFS &) = delete;

   /// No assignment
   SpaceHybridDFS &operator=(const SpaceHybridDFS &) = delete;

   size_t nbSolNodes() const override;
   void pushSolNode(const SharedNode &node) override;
   SharedNode popSolNode() override;
   SharedNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   size_t nbPendingNodes() const override;
   SharedNode popPendingNode() override;
   void pushPendingNode(const SharedNode &node) override;
   SharedNode getPendingNode(size_t i) const override;

private:
   std::vector<SharedNode> sta_;  // stack of pending nodes (for DFS)
   HybridNodeSet *set_;           // set of pending nodes (for BestFS)
   std::vector<SharedNode> vsol_; // vector of solution nodes
   bool leftRight_;               // true if the current DFS stage is
                                  // left-to-right, false for right-to-left
};

} // namespace realpaver

#endif
