///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_SYMBOL_TABLE_HPP
#define REALPAVER_SYMBOL_TABLE_HPP

#include <unordered_map>
#include <unordered_set>
#include "realpaver/Term.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
class ParsingSymbol {
public:
   ParsingSymbol(const std::string& name);

   virtual ~ParsingSymbol();

   std::string getName() const;

private:
   std::string name_;
   size_t hcode_;
};

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
class ConstantSymbol : public ParsingSymbol {
public:
   ConstantSymbol(const std::string& name, const Interval& x);

   Interval getValue() const;

private:
   Interval x_;
};

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
class VariableSymbol : public ParsingSymbol {
public:
   VariableSymbol(Variable v);

   Variable getVar() const;

private:
   Variable v_;
};

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
class SymbolTable {
public:
   SymbolTable() = default;

   ~SymbolTable();

   bool hasSymbol(const std::string& name) const;

   void clear();

   ConstantSymbol* findConstant(const std::string& name) const;
   VariableSymbol* findVariable(const std::string& name) const;

   void insertKeyword(const std::string& name);
   void insertConstant(const std::string& name, const Interval& x);

private:
   std::unordered_map<std::string, ConstantSymbol*> cmap_;
   std::unordered_map<std::string, VariableSymbol*> vmap_;
   std::unordered_set<std::string> keywords_;
};

} // namespace

#endif
