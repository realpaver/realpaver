///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_PARSER_HPP
#define REALPAVER_PARSER_HPP

#include "realpaver/Problem.hpp"
#include "realpaver/SymbolTable.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a wrapper for a parser of the Realpaver language.
///////////////////////////////////////////////////////////////////////////////
class Parser {
public:
   /// Constructor
   Parser();

   /// Parses a model from a file
   /// @param filename input file
   /// @param problem output problem
   /// @return true in case of success, false otherwise
   bool parseFile(const std::string& filename, Problem& problem);

   /// @return the parse error in case of failure
   std::string getParseError() const;

private:
   SymbolTable symtab_;

   void initSymbolTable();
};

} // namespace

#endif
