///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_SYMBOL_TABLE_HPP
#define REALPAVER_SYMBOL_TABLE_HPP

#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
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
   /// Throws an exception if name is empty.
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
/// This is a symbol representing a term.
///////////////////////////////////////////////////////////////////////////////
class AliasSymbol : public ParsingSymbol {
public:
   /// Constructor
   /// @param name name of the alias
   AliasSymbol(const std::string& name, const Term& t);

   /// @return the term enclosed
   Term getTerm() const;

private:
   Term t_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a symbol representing a function.
///
/// A function symbol has a name, a list of arguments represented by variables
/// and an expression.
///////////////////////////////////////////////////////////////////////////////
class FunctionSymbol : public ParsingSymbol {
public:
   /// Constructor
   /// @param name name of the function
   FunctionSymbol(const std::string& name);

   /// Adds an argument to this in the last place
   /// @param name name of the argument
   void addArgument(const std::string& name);

   /// @return the number of arguments of this
   size_t arity() const;

   /// Gets an argument
   /// @param i an index such that 0 <= i < arity()
   Variable getArgument(size_t i) const;

   /// Gets an argument
   /// @param name name of the variable
   /// @return the argument of this having this name
   Variable getVar(const std::string& name) const;

   /// Tests if this has an argument
   /// @param name name of an argument
   /// @return true if this has the argument, false otherwise
   bool hasArgument(const std::string& name) const;

   /// Assigns the expression of this
   /// @param t term assigned
   /// @return true if the variables of t are the arguments of this
   bool setTerm(const Term& t);

   /// @return the expression of this
   Term getTerm() const;

   /// Gets the index of an argument
   /// @param v an argument of this
   /// @return the index of v in this
   size_t getIndexVar(const Variable& v) const;

   /// Output on a stream
   /// @param os an output stream
   void print(std::ostream& os) const;

private:
   std::vector<Variable> args_;
   Term t_;
   Scope scop_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a call to a function.
///
/// A function call is defined by a function symbol and list of terms
/// assigned to its arguments.
///////////////////////////////////////////////////////////////////////////////
class FunctionCall {
public:
   /// Constructor
   /// @param f function called
   FunctionCall(FunctionSymbol* f);

   /// Adds a term in the last place
   /// @param t term assigned to an argument
   void addTerm(Term t);

   /// @return the function symbol enclosed
   FunctionSymbol* getFunctionSymbol() const;

   /// @return the number of terms assigned to the arguments
   size_t nbTerms() const;

   /// Gets a term assigned to an argument
   /// @param i index of an argument
   /// @return the term assigned to the i-th argument
   Term getTerm(size_t i) const;

private:
   FunctionSymbol* f_;
   std::vector<Term> lt_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a table of symbols managed by parsers.
///
/// There are several types of symbols:
/// - constants;
/// - variables;
/// - aliases;
/// - functions.
///
/// This has a stack of function calls which supports the composition of
/// function calls.
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

   /// Inserts a keyword
   /// @param name a string
   void insertKeyword(const std::string& name);

   /// Finds a constant
   /// @param name a string
   /// @return the constant symbol having this name if it exists,
   ///         nullptr otherwise
   ConstantSymbol* findConstant(const std::string& name) const;

   /// Creates and inserts a constant symbol
   /// @param name symbol name
   /// @param x value
   /// @return the new symbol
   ConstantSymbol* insertConstant(const std::string& name, const Interval& x);

   /// Finds a variable
   /// @param name a string
   /// @return the variable symbol having this name if it exists,
   ///         nullptr otherwise
   VariableSymbol* findVariable(const std::string& name) const;

   /// Creates and inserts a variable symbol
   /// @param name symbol name
   /// @param v the variable object
   /// @return the new symbol
   VariableSymbol* insertVariable(const std::string& name, Variable v);

   /// Finds an alias
   /// @param name a string
   /// @return the alias symbol having this name if it exists,
   ///         nullptr otherwise
   AliasSymbol* findAlias(const std::string& name) const;

   /// Creates and inserts an alias symbol
   /// @param name symbol name
   /// @param t term assigned to this symbol
   /// @return the new symbol
   AliasSymbol* insertAlias(const std::string& name, const Term& t);

   /// Finds a function
   /// @param name a string
   /// @return the function symbol having this name if it exists,
   ///         nullptr otherwise
   FunctionSymbol* findFunction(const std::string& name) const;

   /// Creates and inserts a function symbol
   /// @param name symbol name
   /// @return the new symbol
   FunctionSymbol* insertFunction(const std::string& name);

   /// Opens and pushes a new function call
   /// @param f the function called
   void pushFunctionCall(FunctionSymbol* f);

   /// Pops the last function call
   void popFunctionCall();

   /// Assigns a term to an argument of the last function called
   /// @param t term assigned
   void addFunctionArgument(Term t);

   /// @return true if there is at least one function called
   bool hasFunctionCall() const;

   /// Processes and pops the last function call
   /// @return a pair (res, t) such that res is false in case of failure
   ///         (the number of terms assigned is different fron the arity of
   ///         the function), res is true otherwise and in this case t is
   ///         the term corresponding to the function call
   std::pair<bool, Term> processFunCall();

private:
   std::unordered_map<std::string, ConstantSymbol*> cmap_;
   std::unordered_map<std::string, VariableSymbol*> vmap_;
   std::unordered_map<std::string, FunctionSymbol*> fmap_;
   std::unordered_map<std::string, AliasSymbol*> amap_;
   std::unordered_set<std::string> keywords_;

   std::stack<FunctionCall> call_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor of terms used to process a function call.
///////////////////////////////////////////////////////////////////////////////
class FunctionCallProcessor : public TermVisitor {
public:
   /// Creates a visitor
   /// @param fc a function call
   FunctionCallProcessor(FunctionCall* fc);

   ///@{
   void apply(const TermConst* t) override;
   void apply(const TermVar* t) override;
   void apply(const TermAdd* t) override;
   void apply(const TermSub* t) override;
   void apply(const TermMul* t) override;
   void apply(const TermDiv* t) override;
   void apply(const TermMin* t) override;
   void apply(const TermMax* t) override;
   void apply(const TermUsb* t) override;
   void apply(const TermAbs* t) override;
   void apply(const TermSgn* t) override;
   void apply(const TermSqr* t) override;
   void apply(const TermSqrt* t) override;
   void apply(const TermPow* t) override;
   void apply(const TermExp* t) override;
   void apply(const TermLog* t) override;
   void apply(const TermCos* t) override;
   void apply(const TermSin* t) override;
   void apply(const TermTan* t) override;
   void apply(const TermLin* t) override;
   ///@}

   Term getTerm() const;

private:
   FunctionCall* fc_;
   Term t_;
};

} // namespace

#endif
