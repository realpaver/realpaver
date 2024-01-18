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
#include "Variable.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
enum class NcspNodeInfoType {
   SplitVar
};

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

private:
   // map node index -> list of infos
   typedef std::list< std::shared_ptr<NcspNodeInfo> > ListType;
   std::unordered_map<int, ListType> map_;

   // test used in assertions
   bool hasInfo(int index,  NcspNodeInfoType typ) const;
};

} // namespace

#endif
