///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_NODE_INFO_MAP_HPP
#define REALPAVER_NCSP_NODE_INFO_MAP_HPP

#include <list>
#include <memory>
#include <unordered_map>
#include "realpaver/Variable.hpp"

namespace realpaver {
   
   // TODO : commenter !!!

///////////////////////////////////////////////////////////////////////////////
/// Type of informations that can be associated with search nodes
///////////////////////////////////////////////////////////////////////////////
enum class NcspNodeInfoType {
   SplitVar    // selected variable in a splitting step
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, NcspNodeInfoType typ);

///////////////////////////////////////////////////////////////////////////////
class NcspNodeInfo {
public:
   NcspNodeInfo(NcspNodeInfoType typ);
   virtual ~NcspNodeInfo();

   NcspNodeInfoType getType() const;

private:
   NcspNodeInfoType typ_;
};

///////////////////////////////////////////////////////////////////////////////
class NcspNodeInfoVar : public NcspNodeInfo {
public:
   NcspNodeInfoVar(Variable v);

   Variable getVar() const;

private:
   Variable v_;
};

///////////////////////////////////////////////////////////////////////////////
class NcspNodeInfoMap {
public:

   void insert(int index, std::shared_ptr<NcspNodeInfo> info);

   void remove(int index);

   std::shared_ptr<NcspNodeInfo> getInfo(int index, NcspNodeInfoType typ) const;

   size_t size() const;

private:
   // map node index -> list of infos
   typedef std::list< std::shared_ptr<NcspNodeInfo> > ListType;
   std::unordered_map<int, ListType> map_;

   // test used in assertions
   bool hasInfo(int index,  NcspNodeInfoType typ) const;
};

} // namespace

#endif
