///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/Common.hpp"
#include "realpaver/SymbolTable.hpp"

namespace realpaver {

ParsingSymbol::ParsingSymbol(const std::string& name)
      : name_(name),
        hcode_(hash1(name))
{}

ParsingSymbol::~ParsingSymbol()
{}

std::string ParsingSymbol::getName() const
{
   return name_;
}

///////////////////////////////////////////////////////////////////////////////

ConstantSymbol::ConstantSymbol(const std::string& name, const Interval& x)
      : ParsingSymbol(name),
        x_(x)
{}

///////////////////////////////////////////////////////////////////////////////

VariableSymbol::VariableSymbol(Variable v)
      : ParsingSymbol(v.getName()),
        v_(v)
{}

///////////////////////////////////////////////////////////////////////////////

SymbolTable::~SymbolTable()
{
   clear();
}

void SymbolTable::clear()
{
   for (auto entry : cmap_) delete entry.second;
   for (auto entry : vmap_) delete entry.second;
}

} // namespace
