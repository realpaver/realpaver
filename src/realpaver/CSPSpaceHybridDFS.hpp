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

#include <set>
#include <vector>
#include "realpaver/CSPSpace.hpp"

namespace realpaver {

/// Criteria used to order sets of boxes
enum class HybridDFSStyle {
   Depth,         ///< depth of a node
   Perimeter,     ///< perimeter of a box
   GridPerimeter  ///< grid perimeter of a box
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const HybridDFSStyle& style);

/*----------------------------------------------------------------------------*/

/// Abstract class representing a set of shared NCSP nodes
class HybridNcspNodeSet {
public:
   /// Constructor
   HybridNcspNodeSet() = default;

   /// Virtual destructor
   virtual ~HybridNcspNodeSet();

   /// Default copy constructor
   HybridNcspNodeSet(const HybridNcspNodeSet&) = default;

   /// No assignment
   HybridNcspNodeSet& operator=(const HybridNcspNodeSet&) = delete;

   /// Returns true if this is empty
   virtual bool isEmpty() const = 0;

   /// Returns the size of this
   virtual size_t size() const = 0;

   /// Inserts a node in this
   virtual void insert(const SharedNcspNode& node) = 0;

   /// Extracts the first node of this
   virtual SharedNcspNode extract() = 0;

   /// Returns the i-th node of this
   virtual SharedNcspNode getNode(size_t i) const = 0;
};

/*----------------------------------------------------------------------------*/

/// Set of shared NCSP nodes ordered by depth (ascending)
class DepthNcspNodeSet : public HybridNcspNodeSet {
public:
   /// Constructor of an empty set
   DepthNcspNodeSet() = default;

   /// Default destructor
   ~DepthNcspNodeSet() = default;

   /// Default copy constructor
   DepthNcspNodeSet(const DepthNcspNodeSet&) = default;

   /// No assignment
   DepthNcspNodeSet& operator=(const DepthNcspNodeSet&) = delete;

   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedNcspNode& node) override;
   SharedNcspNode extract() override;
   SharedNcspNode getNode(size_t i) const override;

private:
   struct Elem {
      SharedNcspNode node;
      int depth;
   };

   struct Comp {
      bool operator()(const Elem& e1, const Elem& e2) const
      {
         return (e1.depth < e2.depth) ||
                (e1.depth == e2.depth && e1.node->index() < e2.node->index());
      }
   };

   std::set<Elem, Comp> set_;
};

/*----------------------------------------------------------------------------*/

/// Set of shared NCSP nodes ordered by perimeter (descending)
class PerimeterNcspNodeSet : public HybridNcspNodeSet {
public:
   /// Constructor of an empty set
   PerimeterNcspNodeSet() = default;

   /// Default destructor
   ~PerimeterNcspNodeSet() = default;

   /// Default copy constructor
   PerimeterNcspNodeSet(const PerimeterNcspNodeSet&) = default;

   /// No assignment
   PerimeterNcspNodeSet& operator=(const PerimeterNcspNodeSet&) = delete;

   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedNcspNode& node) override;
   SharedNcspNode extract() override;
   SharedNcspNode getNode(size_t i) const override;

private:
   struct Elem {
      SharedNcspNode node;
      double peri;
   };

   struct Comp {
      bool operator()(const Elem& e1, const Elem& e2) const
      {
         return (e1.peri > e2.peri) ||
                (e1.peri == e2.peri && e1.node->index() < e2.node->index());
      }
   };

   std::set<Elem, Comp> set_;
};

/*----------------------------------------------------------------------------*/

/// Set of shared NCSP nodes ordered by grid perimeter (descending)
class GridPerimeterNcspNodeSet : public HybridNcspNodeSet {
public:
   /// Constructor of an empty set
   GridPerimeterNcspNodeSet() = default;

   /// Default destructor
   ~GridPerimeterNcspNodeSet() = default;

   /// Default copy constructor
   GridPerimeterNcspNodeSet(const GridPerimeterNcspNodeSet&) = default;

   /// No assignment
   GridPerimeterNcspNodeSet& operator=(const GridPerimeterNcspNodeSet&) = delete;

   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedNcspNode& node) override;
   SharedNcspNode extract() override;
   SharedNcspNode getNode(size_t i) const override;

private:
   struct Elem {
      SharedNcspNode node;
      double peri;
   };

   struct Comp {
      bool operator()(const Elem& e1, const Elem& e2) const
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
   CSPSpaceHybridDFS(const CSPSpaceHybridDFS&) = delete;

   /// No assignment
   CSPSpaceHybridDFS& operator=(const CSPSpaceHybridDFS&) = delete;

   size_t nbSolNodes() const override;
   void pushSolNode(const SharedNcspNode& node) override;
   SharedNcspNode popSolNode() override;
   SharedNcspNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   size_t nbPendingNodes() const override;
   SharedNcspNode nextPendingNode() override;
   void insertPendingNode(const SharedNcspNode& node) override;
   SharedNcspNode getPendingNode(size_t i) const override;
   void insertPendingNodes(NcspSplit::iterator first,
                           NcspSplit::iterator last) override;

private:
   std::vector<SharedNcspNode> sta_;   // stack of pending nodes (for DFS)
   HybridNcspNodeSet* set_;            // set of pending nodes (for BestFS)
   std::vector<SharedNcspNode> vsol_;  // vector of solution nodes
   bool leftRight_;                    // true if the current DFS stage is
                                       // left-to-right, false for right-to-left
};

} // namespace

#endif
