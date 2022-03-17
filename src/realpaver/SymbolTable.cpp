///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/SymbolTable.hpp"

namespace realpaver {

ParsingSymbol::ParsingSymbol(const std::string& name)
      : name_(name),
        hcode_(hash1(name))
{
   THROW_IF(name == "", "Definition of symbol with no name");
}

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
{
   THROW_IF(x.isEmpty(), "Definition of constant symbol with empty interval");
}

Interval ConstantSymbol::getValue() const
{
   return x_;
}

///////////////////////////////////////////////////////////////////////////////

VariableSymbol::VariableSymbol(Variable v)
      : ParsingSymbol(v.getName()),
        v_(v)
{}

Variable VariableSymbol::getVar() const
{
   return v_;
}

///////////////////////////////////////////////////////////////////////////////

SymbolTable::~SymbolTable()
{
   clear();
}
 
bool SymbolTable::hasSymbol(const std::string& name) const
{
   auto itc = cmap_.find(name);
   if (itc != cmap_.end()) return true;

   auto itv = vmap_.find(name);
   if (itv != vmap_.end()) return true;

   auto itk = keywords_.find(name);
   if (itk != keywords_.end()) return true;

   return false;
}

ConstantSymbol* SymbolTable::findConstant(const std::string& name) const
{
   auto itc = cmap_.find(name);
   return (itc == cmap_.end()) ? nullptr : itc->second;
}

VariableSymbol* SymbolTable::findVariable(const std::string& name) const
{
   auto itv = vmap_.find(name);
   return (itv == vmap_.end()) ? nullptr : itv->second;   
}

void SymbolTable::insertKeyword(const std::string& name)
{
   keywords_.insert(name);
}

void SymbolTable::insertConstant(const std::string& name, const Interval& x)
{
   ConstantSymbol* symbol = new ConstantSymbol(name, x);
   cmap_.insert(std::make_pair(name, symbol));
}

void SymbolTable::insertVariable(const std::string& name, Variable v)
{
   VariableSymbol* symbol = new VariableSymbol(v);
   vmap_.insert(std::make_pair(name, symbol));
}

void SymbolTable::clear()
{
   for (auto entry : cmap_) delete entry.second;
   cmap_.clear();

   for (auto entry : vmap_) delete entry.second;
   vmap_.clear();
}

} // namespace
