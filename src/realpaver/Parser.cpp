///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "realpaver/Parser.hpp"

extern int realpaver_flex_init_file(realpaver::Problem* prob,
                                    realpaver::SymbolTable* symtab,
                                    const char* filename);
extern void realpaver_flex_cleanup_file(void);
extern int realpaver_bison_parse(void);
extern std::string realpaver_parse_error;

///////////////////////////////////////////////////////////////////////////////

namespace realpaver {

static std::vector<std::string> keywords = {
   "Variables", "Constants", "Constraints",
   "Aliases", "Functions", "Objectives",
   "Integer", "in",
   "MIN", "MAX", "min", "max", "abs", "sgn", "sqr", "sqrt", "pow",
   "log", "exp", "cos", "sin", "tan"
};

Parser::Parser() : symtab_()
{}

void Parser::initSymbolTable()
{
   // keywords
   for (auto k : keywords) symtab_.insertKeyword(k);

   // constants
   symtab_.insertConstant("pi", Interval::pi());
   symtab_.insertConstant("e",  exp(Interval(1.0)));
}

std::string Parser::getParseError() const
{
   return realpaver_parse_error;
}

bool Parser::parseFile(const std::string& filename, Problem& problem)
{
   initSymbolTable();

   int res = realpaver_flex_init_file(&problem, &symtab_, filename.c_str());
   if (res == 0) res = realpaver_bison_parse();

   realpaver_flex_cleanup_file();
   symtab_.clear();

   return res == 0;
}

} // namespace