///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_NODE_HPP
#define REALPAVER_NCSP_NODE_HPP

#include "realpaver/SearchNode.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a node of a search tree generated by a Branch-and-Prune algorithm
/// for solving a Numerical CSP.
///
/// A node inherits from the SearchNode class. It stores a proof certificate.
///////////////////////////////////////////////////////////////////////////////
class NcspNode : public SearchNode {
public:
   /// Creates a node
   /// @param scope set of variables
   /// @param reg domains of variables
   /// @param depth depth of this
   NcspNode(const Scope& scope, const IntervalRegion& reg,  int depth = 0);

   /// Default copy constructor
   NcspNode(const NcspNode& node) = default;

   /// No assignment
   NcspNode& operator=(const NcspNode&) = delete;

   /// Default destructor
   ~NcspNode() = default;

   /// @return the proof certificate
   Proof getProof() const;

   /// Sets the proof certificate
   /// @param p new proof certificate
   void setProof(Proof p);

private:
   Proof proof_;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const NcspNode& node);

/// Type of shared pointers on CSP nodes
typedef std::shared_ptr<NcspNode> SharedNcspNode;

} // namespace

#endif