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
/// This is the base class of symbols managed by parsers.
///////////////////////////////////////////////////////////////////////////////
class ParsingSymbol {
public:
   /// Constructor
   /// @param name symbol name
   ///
   /// Thorws an exception if name is empty.
   ParsingSymbol(const std::string& name);

   /// Virtual destructor
   virtual ~ParsingSymbol();

   /// @return the name of the symbol
   std::string getName() const;

private:
   std::string name_;
   size_t hcode_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a symbol representing an interval constant.
///////////////////////////////////////////////////////////////////////////////
class ConstantSymbol : public ParsingSymbol {
public:
   /// Constructor
   /// @param name symbol name
   /// @param x value
   ConstantSymbol(const std::string& name, const Interval& x);

   /// @return the value of this
   Interval getValue() const;

private:
   Interval x_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a symbol representing a variable.
///////////////////////////////////////////////////////////////////////////////
class VariableSymbol : public ParsingSymbol {
public:
   /// Constructor
   /// @param v a variable
   VariableSymbol(Variable v);

   /// @return the variable in this
   Variable getVar() const;

private:
   Variable v_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a table of symbols managed by parsers.
///
/// There are several types of symbols:
/// - constants;
/// - variables.
///////////////////////////////////////////////////////////////////////////////
class SymbolTable {
public:
   /// Creates an empty table
   SymbolTable() = default;

   /// Destructor
   ~SymbolTable();

   /// No copy
   SymbolTable(const SymbolTable&) = delete;

   /// No assignment
   SymbolTable& operator=(const SymbolTable&) = delete;

   /// Membership test
   /// @param name a string
   /// @return true if this contains a symbol having this name
   bool hasSymbol(const std::string& name) const;

   /// Clears this
   void clear();

   /// Finds a constant
   /// @param name a string
   /// @return the constant symbol having this name if it exists,
   ///         nullptr otherwise
   ConstantSymbol* findConstant(const std::string& name) const;

   /// Finds a variable
   /// @param name a string
   /// @return the variable symbol having this name if it exists,
   ///         nullptr otherwise
   VariableSymbol* findVariable(const std::string& name) const;

   /// Inserts a keyword
   void insertKeyword(const std::string& name);

   /// Inserts a constant symbol
   /// @param name symbol name
   /// @param x value
   void insertConstant(const std::string& name, const Interval& x);

   /// Inserts a variable symbol
   /// @param name symbol name
   /// @param v the variable object
   void insertVariable(const std::string& name, Variable v);

private:
   std::unordered_map<std::string, ConstantSymbol*> cmap_;
   std::unordered_map<std::string, VariableSymbol*> vmap_;
   std::unordered_set<std::string> keywords_;
};

} // namespace

#endif
