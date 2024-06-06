/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   SymbolTable.hpp
 * @brief  Symbol table for parsing
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_SYMBOL_TABLE_HPP
#define REALPAVER_SYMBOL_TABLE_HPP

#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "realpaver/Term.hpp"

namespace realpaver {

/// Base class of symbols managed by parsers
class ParsingSymbol {
public:
   /// Constructor
   ParsingSymbol(const std::string& name);

   /// Virtual destructor
   virtual ~ParsingSymbol();

   /// Returns the name of the symbol
   std::string getName() const;

private:
   std::string name_;
   size_t hcode_;
};

/*----------------------------------------------------------------------------*/

/// Symbol representing an interval constant
class ConstantSymbol : public ParsingSymbol {
public:
   /// Constructor
   ConstantSymbol(const std::string& name, const Interval& x);

   /// Returns the value of this
   Interval getValue() const;

private:
   Interval x_;
};

/*----------------------------------------------------------------------------*/

/// Symbol representing a variable
class VariableSymbol : public ParsingSymbol {
public:
   /// Constructor
   VariableSymbol(Variable v);

   /// Returns the variable in this
   Variable getVar() const;

private:
   Variable v_;
};

/*----------------------------------------------------------------------------*/

/// Symbol representing a term
class AliasSymbol : public ParsingSymbol {
public:
   /// Constructor
   AliasSymbol(const std::string& name, const Term& t);

   /// Returns the term enclosed
   Term getTerm() const;

private:
   Term t_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Symbol representing a function.
 *
 * A function symbol has a name, a list of arguments represented by variables
 * and an expression.
 */
class FunctionSymbol : public ParsingSymbol {
public:
   /// Constructor
   FunctionSymbol(const std::string& name);

   /// Adds an argument to this in the last place
   void addArgument(const std::string& name);

   /// Returns the number of arguments of this
   size_t arity() const;

   /// Gets the i-th argument with 0 <= i < arity()
   Variable getArgument(size_t i) const;

   /// Gets an argument given its name
   Variable getVar(const std::string& name) const;

   /// Tests if this has an argument
   bool hasArgument(const std::string& name) const;

   /**
    * @brief Assigns an expression of this.
    * 
    * Returns true if the variables of t correspond to the arguments of this.
    */
   bool setTerm(const Term& t);

   /// Returns the expression of this
   Term getTerm() const;

   /// Gets the index of v in this
   size_t getIndexVar(const Variable& v) const;

   /// Output on a stream
   void print(std::ostream& os) const;

private:
   std::vector<Variable> args_;
   Term t_;
   Scope scop_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Call to a function.
 *
 * A function call is defined by a function symbol and list of terms
 * assigned to its arguments.
 */
class FunctionCall {
public:
   /// Constructor
   FunctionCall(FunctionSymbol* f);

   /// Adds a term in the last place
   void addTerm(Term t);

   /// Returns the function symbol enclosed
   FunctionSymbol* getFunctionSymbol() const;

   /// Returns the number of terms assigned to the arguments
   size_t nbTerms() const;

   /// Gets the term assigned to the i-th argument
   Term getTerm(size_t i) const;

private:
   FunctionSymbol* f_;
   std::vector<Term> lt_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Table of symbols managed by parsers.
 *
 * There are several types of symbols:
 * - constants;
 * - variables;
 * - aliases;
 * - functions.
 *
 * This has a stack of function calls which supports the composition of
 * function calls.
 */
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
   bool hasSymbol(const std::string& name) const;

   /// Clears this
   void clear();

   /// Inserts a keyword
   void insertKeyword(const std::string& name);

   /// Finds a constant
   ConstantSymbol* findConstant(const std::string& name) const;

   /// Creates and inserts a constant symbol
   ConstantSymbol* insertConstant(const std::string& name, const Interval& x);

   /// Finds a variable
   VariableSymbol* findVariable(const std::string& name) const;

   /// Creates and inserts a variable symbol
   VariableSymbol* insertVariable(const std::string& name, Variable v);

   /// Finds an alias
   AliasSymbol* findAlias(const std::string& name) const;

   /// Creates and inserts an alias symbol
   AliasSymbol* insertAlias(const std::string& name, const Term& t);

   /// Finds a function
   FunctionSymbol* findFunction(const std::string& name) const;

   /// Creates and inserts a function symbol
   FunctionSymbol* insertFunction(const std::string& name);

   /// Opens and pushes a new function call
   void pushFunctionCall(FunctionSymbol* f);

   /// Pops the last function call
   void popFunctionCall();

   /// Assigns a term to an argument of the last function called
   void addFunctionArgument(Term t);

   /// Returns true if there is at least one function called
   bool hasFunctionCall() const;

   /**
    * @brief Processes and pops the last function call.
    * 
    * Returns a pair (res, t) such that res is false in case of failure (the
    * number of terms assigned is different fron the arity of the function),
    * res is true otherwise and in this case t is the term corresponding to the
    * function call.
    */
   std::pair<bool, Term> processFunCall();

private:
   std::unordered_map<std::string, ConstantSymbol*> cmap_;
   std::unordered_map<std::string, VariableSymbol*> vmap_;
   std::unordered_map<std::string, FunctionSymbol*> fmap_;
   std::unordered_map<std::string, AliasSymbol*> amap_;
   std::unordered_set<std::string> keywords_;

   std::stack<FunctionCall> call_;
};

/*----------------------------------------------------------------------------*/

/// Visitor of terms used to process a function call
class FunctionCallProcessor : public TermVisitor {
public:
   /// Creates a visitor
   FunctionCallProcessor(FunctionCall* fc);

   void apply(const TermCst* t) override;
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
   void apply(const TermCosh* t) override;
   void apply(const TermSinh* t) override;
   void apply(const TermTanh* t) override;

   /// Returns the new term
   Term getTerm() const;

private:
   FunctionCall* fc_;
   Term t_;
};

} // namespace

#endif
