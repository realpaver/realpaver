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
   
class Parser {
public:
   Parser();

   bool parseFile(const std::string& filename, Problem& problem);

   std::string getParseError() const;

private:
   SymbolTable symtab_;
   std::string error_;

   void initSymbolTable();
};

} // namespace

#endif
