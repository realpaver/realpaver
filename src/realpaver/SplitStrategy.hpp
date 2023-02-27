///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_SPLIT_STRATEGY_HPP
#define REALPAVER_SPLIT_STRATEGY_HPP

#include <list>

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a base class for splitting strategies.
///
/// An object of this class splits a node in a search tree and generates a set
/// of sub-nodes. The sub-nodes are inserted in a container.
/// Iterators on this container are provided.
///
/// This is parameterized by the type of nodes T which must be a pointer type
/// and a sub-class of SearchNode. The depth of a sub-node is incremented when
/// it is added in the container.
///////////////////////////////////////////////////////////////////////////////
template <typename T>
class SplitStrategy {
public:
   /// Type of nodes
   typedef T NodeType;

   /// Creates a splitting object
   SplitStrategy();

   /// Destructor
   virtual ~SplitStrategy();

   /// No copy
   SplitStrategy(const SplitStrategy&) = delete;

   /// No assignment
   SplitStrategy& operator=(const SplitStrategy&) = delete;

   /// @return the number of nodes generated by the last split
   size_t getNbNodes() const;

   /// @return the number of application of this
   size_t getNbSplits() const;

   /// Splits a node
   /// @param node no of a BO search tree
   /// @return true the given node can be split in at least two sub-nodes
   ///
   /// This method calls applyImpl(node).
   bool apply(NodeType node);

   /// Clears the container of nodes and resets the counters
   void reset();

private:
   typedef std::list<NodeType> ContainerType;

   ContainerType cont_;
   size_t nbsplits_;

protected:
   /// Inserts a node in this container
   void push(NodeType node);

   /// Implements the split
   virtual bool applyImpl(NodeType node) = 0;

public:
   /// Iterators on the set of generated nodes
   typedef typename ContainerType::iterator iterator;

   /// @returns an iterator on the beginning of the container of sub-nodes
   iterator begin();

   /// @returns an iterator on the end of the container of sub-nodes
   iterator end();
};

template <typename T>
SplitStrategy<T>::SplitStrategy()
      : cont_(),
        nbsplits_(0)
{}

template <typename T>
SplitStrategy<T>::~SplitStrategy()
{}

template <typename T>
size_t SplitStrategy<T>::getNbNodes() const
{
   return cont_.size();
}

template <typename T>
size_t SplitStrategy<T>::getNbSplits() const
{
   return nbsplits_;
}

template <typename T>
bool SplitStrategy<T>::apply(NodeType node)
{
   cont_.clear();
   ++nbsplits_;
   return applyImpl(node);
}

template <typename T>
void SplitStrategy<T>::reset()
{
   cont_.clear();
   nbsplits_ = 0;
}

template <typename T>
void SplitStrategy<T>::push(NodeType node)
{
   cont_.push_back(node);
}

template <typename T>
typename SplitStrategy<T>::iterator SplitStrategy<T>::begin()
{
   return cont_.begin();
}

template <typename T>
typename SplitStrategy<T>::iterator SplitStrategy<T>::end()
{
   return cont_.end();
}

} // namespace

#endif
