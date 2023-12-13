///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_SPACE_DMDFS_HPP
#define REALPAVER_NCSP_SPACE_DMDFS_HPP

#include "realpaver/NcspSpace.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of objects that calculate the distance between
/// boxes.
///////////////////////////////////////////////////////////////////////////////
class DistCalculator {
public:
   /// Virtual destructor
   virtual ~DistCalculator();

   /// Calculates the distance between boxes
   /// @param db1 a box
   /// @param db2 a box having the same scope than db1
   /// @return the Hausdorff distance between db1 and db2
   virtual double distance(const DomainBox& db1, const DomainBox& db2) = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// This calculates the Hausdorff distance between boxes.
///////////////////////////////////////////////////////////////////////////////
class HausdorffDistCalculator : public DistCalculator {
public:
   double distance(const DomainBox& db1, const DomainBox& db2) override;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a space generated by a branch-and-prune algorithm for solving CSPs
/// that implements a Distant-Most Depth-First-Search strategy.
///
/// The distance used can be parameterized through a distance calculator.
/// The default one is the Hausdorff distance.
///////////////////////////////////////////////////////////////////////////////
class NcspSpaceDMDFS : public NcspSpace {
public:
   /// Constructor
   NcspSpaceDMDFS();

   /// Destructor
   ~NcspSpaceDMDFS();

   /// No assignment
   NcspSpaceDMDFS& operator=(const NcspSpaceDMDFS&) = delete;

   /// Default copy constructor
   NcspSpaceDMDFS(const NcspSpaceDMDFS&) = default;

   ///@{
   size_t nbSolNodes() const override;
   void pushSolNode(const SharedNcspNode& node) override;
   SharedNcspNode popSolNode() override;
   SharedNcspNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   void makeSolClusters(double gap) override;
   size_t nbPendingNodes() const override;
   SharedNcspNode nextPendingNode() override;
   void insertPendingNode(const SharedNcspNode& node) override;
   SharedNcspNode getPendingNode(size_t i) const override;
   ///@}

   /// Assigns the distance calculator in this
   /// @param dcalc the new calculator
   void setDistCalculator(std::unique_ptr<DistCalculator> dcalc);

private:
   struct Elem {
      SharedNcspNode node;    // node
      double mindist;         // distance to the closest solution
   };

   struct Comp {
      bool operator()(const Elem& x, const Elem& y)
      {
         return (x.mindist < y.mindist) ||
                 (x.mindist == y.mindist && x.node->index() < y.node->index());
      }
   } comparator;

   std::vector<Elem> vnode_;           // vector of pending nodes
   std::vector<SharedNcspNode> vsol_;  // vector of solution nodes

   DistCalculator* dcalc_;             // object used to calculate the distance
                                       // between boxes
};

} // namespace

#endif
