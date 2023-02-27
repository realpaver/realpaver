///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "realpaver/Parser.hpp"

extern int realpaver_flex_init_file(realpaver::Problem* prob,
                                    realpaver::SymbolTable* symtab,
                                    realpaver::Param* prm,
                                    const char* filename);
extern void realpaver_flex_cleanup_file(void);
extern int realpaver_bison_parse(void);
extern std::string realpaver_parse_error;

///////////////////////////////////////////////////////////////////////////////

namespace realpaver {

static std::vector<std::string> keywords = {
   "Variables", "Constants", "Constraints",
   "Aliases", "Functions", "Objectives",
   "integer", "in", "binary", "tol", "inf",
   "MIN", "MAX", "min", "max", "abs", "sgn", "sqr", "sqrt", "pow",
   "log", "exp", "cos", "sin", "tan",
   "if", "then", "else", "or", "and", "not"
};

Parser::Parser() : symtab_(), param_()
{}

Parser::Parser(const Param& prm) : param_(prm)
{}

void Parser::initSymbolTable()
{
   // keywords
   for (auto k : keywords) symtab_.insertKeyword(k);

   // constants
   symtab_.insertConstant("PI", Interval::pi());
}

std::string Parser::getParseError() const
{
   return realpaver_parse_error;
}

bool Parser::parseFile(const std::string& filename, Problem& problem)
{
   initSymbolTable();

   // initializes the lexical analyzer
   int res = realpaver_flex_init_file(&problem, &symtab_, &param_,
                                      filename.c_str());

   // parses the input file
   if (res == 0) res = realpaver_bison_parse();

   realpaver_flex_cleanup_file();
   symtab_.clear();

   return res == 0;
}

} // namespace
