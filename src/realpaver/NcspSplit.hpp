///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_SPLIT_HPP
#define REALPAVER_NCSP_SPLIT_HPP

#include <list>
#include "realpaver/DomainSlicerFactory.hpp"
#include "realpaver/NcspContext.hpp"
#include "realpaver/NcspNode.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This implements a splitting strategy for NCSP nodes.
///
/// It manages a counter of node indexes. When a new sub-node is created in the
/// apply method, its index is assigned to the counter value, which is then
/// incremented.
///
/// In order to reset this counter, the reset method must be called, e.g.,
/// before the solving of a new problem.
///////////////////////////////////////////////////////////////////////////////
class NcspSplit {
public:
   /// Creates a splitting object
   /// @param scop set of variables that are examined
   /// @param smap a domain slicer map
   NcspSplit(Scope scop, std::unique_ptr<DomainSlicerMap> smap);

   /// Destructor
   virtual ~NcspSplit();

   /// No copy
   NcspSplit(const NcspSplit&) = delete;

   /// No assignment
   NcspSplit& operator=(const NcspSplit&) = delete;

   /// @return the scope of this
   Scope scope() const;

   /// Splits a node
   /// @param node a node occuring in a search tree
   /// @return true if the given node can be split in at least two sub-nodes,
   ///         false otherwise
   ///
   /// The sub-nodes can be obtained through iterators.
   void apply(SharedNcspNode& node);

   /// @return the number of nodes generated by the last split
   size_t getNbNodes() const;

   /// @return the number of application of this
   size_t getNbSplits() const;

   /// Clears the informations stored for a node
   /// @param index the node index
   ///
   /// If the node is no more used then it is important to save memory
   void removeInfo(int index);

   /// Resets this
   void reset();

   /// @return the information map of this
   NcspContext* getInfoMap() const;

protected:
   typedef std::list<SharedNcspNode> ContainerType;

   Scope scop_;                  // set of variables
   DomainSlicerMap* slicerMap_;  // slicer of domains
   NcspContext* infoMap_;    // map of informations about nodes
   ContainerType cont_;          // container for the sub-nodes

   // Implements the splitting method
   virtual void applyImpl(SharedNcspNode& node) = 0;

   // Splits a node given a selected variable
   // It may be used by applyImpl in the sub-classes
   void splitOne(SharedNcspNode& node, Variable v);

   // Clones a node, assigns an index to the clone and increments its depth
   SharedNcspNode cloneNode(const SharedNcspNode& node);

private:
   size_t nbs_;   // number of splitting steps
   size_t idx_;   // next node index

public:
   /// Type of iterators on the set of sub-nodes
   typedef typename ContainerType::iterator iterator;

   /// @returns an iterator on the beginning of the container of sub-nodes
   iterator begin();

   /// @returns an iterator on the end of the container of sub-nodes
   iterator end();
};

///////////////////////////////////////////////////////////////////////////////
/// This implements the Round-Robin strategy.
///////////////////////////////////////////////////////////////////////////////
class NcspSplitRR : public NcspSplit {
public:
   /// Creates a splitting object
   /// @param scop set of variables that are examined
   /// @param smap a domain slicer map
   NcspSplitRR(Scope scop, std::unique_ptr<DomainSlicerMap> smap);

   /// Default destructor
   ~NcspSplitRR() = default;

   /// No copy
   NcspSplitRR(const NcspSplitRR&) = delete;

   /// No assignment
   NcspSplitRR& operator=(const NcspSplitRR&) = delete;

   ///@{
   void applyImpl(SharedNcspNode& node) override;
   ///@}

private:
   // variable selection method
   std::pair<bool, Variable> selectVar(SharedNcspNode& node);
};

///////////////////////////////////////////////////////////////////////////////
/// This implements the Largest-First strategy.
///////////////////////////////////////////////////////////////////////////////
class NcspSplitLF : public NcspSplit {
public:
   /// Creates a splitting object
   /// @param scop set of variables that are examined
   /// @param smap a domain slicer map
   NcspSplitLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap);

   /// Default destructor
   ~NcspSplitLF() = default;

   /// No copy
   NcspSplitLF(const NcspSplitLF&) = delete;

   /// No assignment
   NcspSplitLF& operator=(const NcspSplitLF&) = delete;

   ///@{
   void applyImpl(SharedNcspNode& node) override;
   ///@}

   /// Variable selection method
   /// @param 
   /// @param node a node
   /// @return a couple (b, v) such that b is false if no variable has been
   ///         selected, b is true if v has ben selected
   std::pair<bool, Variable> selectVar(SharedNcspNode& node);

   static std::pair<bool, Variable> selectVar(const Scope& scop,
                                              const DomainBox& box);
};

///////////////////////////////////////////////////////////////////////////////
/// This implements the Smallest-First strategy.
///////////////////////////////////////////////////////////////////////////////
class NcspSplitSF : public NcspSplit {
public:
   /// Creates a splitting object
   /// @param scop set of variables that are examined
   /// @param smap a domain slicer map
   NcspSplitSF(Scope scop, std::unique_ptr<DomainSlicerMap> smap);

   /// Default destructor
   ~NcspSplitSF() = default;

   /// No copy
   NcspSplitSF(const NcspSplitSF&) = delete;

   /// No assignment
   NcspSplitSF& operator=(const NcspSplitSF&) = delete;

   ///@{
   void applyImpl(SharedNcspNode& node) override;
   ///@}

private:
   // variable selection method
   std::pair<bool, Variable> selectVar(SharedNcspNode& node);
};

///////////////////////////////////////////////////////////////////////////////
/// This implements a splitting strategy for mixed problems.
///
/// If there is an integer variable whose domain is splitable, then it selects
/// the integer variable having the smallest domain. Otherwise, it selects the
/// real variable having the largest domain.
///////////////////////////////////////////////////////////////////////////////
class NcspSplitSLF : public NcspSplit {
public:
   /// Creates a splitting object
   /// @param scop set of variables that are examined
   /// @param smap a domain slicer map
   NcspSplitSLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap);

   /// Default destructor
   ~NcspSplitSLF() = default;

   /// No copy
   NcspSplitSLF(const NcspSplitSLF&) = delete;

   /// No assignment
   NcspSplitSLF& operator=(const NcspSplitSLF&) = delete;

   ///@{
   void applyImpl(SharedNcspNode& node) override;
   ///@}

private:
   // variable selection method
   std::pair<bool, Variable> selectVar(SharedNcspNode& node);
};

///////////////////////////////////////////////////////////////////////////////
/// This implements a splitting strategy for mixed problems.
///
/// If there is an integer variable whose domain is splitable, then it selects
/// the integer variable having the smallest domain. Otherwise, it selects the
/// real variable having the largest domain.
///////////////////////////////////////////////////////////////////////////////
class NcspSplitSSR : public NcspSplit {
public:
   /// Creates a splitting object
   /// @param scop set of variables that are examined
   /// @param smap a domain slicer map
   NcspSplitSSR(std::shared_ptr<IntervalSmearSumRel> ssr,
                std::unique_ptr<DomainSlicerMap> smap);

   /// Default destructor
   ~NcspSplitSSR() = default;

   /// No copy
   NcspSplitSSR(const NcspSplitSSR&) = delete;

   /// No assignment
   NcspSplitSSR& operator=(const NcspSplitSSR&) = delete;

   ///@{
   void applyImpl(SharedNcspNode& node) override;
   ///@}

private:
   std::shared_ptr<IntervalSmearSumRel> ssr_;

   // variable selection method
   std::pair<bool, Variable> selectVar(SharedNcspNode& node);
};

} // namespace

#endif
