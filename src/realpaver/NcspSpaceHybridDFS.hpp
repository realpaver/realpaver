///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_SPACE_HYBRID_DFS_HPP
#define REALPAVER_NCSP_SPACE_HYBRID_DFS_HPP

#include <set>
#include <vector>
#include "realpaver/NcspSpace.hpp"

namespace realpaver {

/// Criteria used to order  
enum class HybridDFSStyle { Depth, Perimeter, GridPerimeter };

/// output on a stream
std::ostream& operator<<(std::ostream& os, const HybridDFSStyle& style);

///////////////////////////////////////////////////////////////////////////////
/// This is an abstract class representing a set of shared NCSP nodes.
///////////////////////////////////////////////////////////////////////////////
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

   /// @return true if this is empty
   virtual bool isEmpty() const = 0;

   /// @return the size of this
   virtual size_t size() const = 0;

   /// Inserts a node in this
   /// @param node node inserted
   virtual void insert(const SharedNcspNode& node) = 0;

   /// Extracts the first node of this
   ///@ return the first node of this
   virtual SharedNcspNode extract() = 0;

   /// @param an index in 0 .. size()-1
   /// @return the i-th node of this
   virtual SharedNcspNode getNode(size_t i) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a set of shared NCSP nodes ordered by depth (ascending).
///////////////////////////////////////////////////////////////////////////////
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

   ///@{
   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedNcspNode& node) override;
   SharedNcspNode extract() override;
   SharedNcspNode getNode(size_t i) const override;
   ///@}

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

///////////////////////////////////////////////////////////////////////////////
/// This is a set of shared NCSP nodes ordered by perimeter (descending).
///////////////////////////////////////////////////////////////////////////////
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

   ///@{
   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedNcspNode& node) override;
   SharedNcspNode extract() override;
   SharedNcspNode getNode(size_t i) const override;
   ///@}

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

///////////////////////////////////////////////////////////////////////////////
/// This is a set of shared NCSP nodes ordered by gridperimeter (descending).
///////////////////////////////////////////////////////////////////////////////
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

   ///@{
   bool isEmpty() const override;
   size_t size() const override;
   void insert(const SharedNcspNode& node) override;
   SharedNcspNode extract() override;
   SharedNcspNode getNode(size_t i) const override;
   ///@}

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

///////////////////////////////////////////////////////////////////////////////
/// This is a space generated by a branch-and-prune algorithm for solving CSPs
/// that implements hybrid Best-First Depth-First search strategies.
///////////////////////////////////////////////////////////////////////////////
class NcspSpaceHybridDFS : public NcspSpace {
public:
   /// Constructor
   NcspSpaceHybridDFS(HybridDFSStyle style);

   /// Destructor
   ~NcspSpaceHybridDFS();

   /// No copy
   NcspSpaceHybridDFS(const NcspSpaceHybridDFS&) = delete;

   /// No assignment
   NcspSpaceHybridDFS& operator=(const NcspSpaceHybridDFS&) = delete;

   ///@{
   size_t nbSolNodes() const override;
   void pushSolNode(const SharedNcspNode& node) override;
   SharedNcspNode popSolNode() override;
   SharedNcspNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   void makeSolClusters(double gap) override;
   size_t nbTotalSolNodes() const override;
   size_t nbPendingNodes() const override;
   SharedNcspNode nextPendingNode() override;
   void insertPendingNode(const SharedNcspNode& node) override;
   SharedNcspNode getPendingNode(size_t i) const override;
   void insertPendingNodes(NcspSplit::iterator first,
                           NcspSplit::iterator last) override;

   ///@}

private:
   std::vector<SharedNcspNode> sta_;   // stack of pending nodes (for DFS)
   HybridNcspNodeSet * set_;           // set of pending nodes (for BestFS)
   std::vector<SharedNcspNode> vsol_;  // vector of solution nodes
   size_t stotal_;                     // number of solution nodes that have
                                       // been inserted in this
   bool leftRight_;                    // true if the current DFS stage is
                                       // left-to-right, false for right-to-left
};

} // namespace

#endif
