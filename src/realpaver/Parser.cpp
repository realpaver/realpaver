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
 * @file   Parser.cpp
 * @brief  Wrapper for the parser of the modelling language
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Parser.hpp"
#include <vector>

extern int realpaver_flex_init_file(realpaver::Problem *prob,
                                    realpaver::SymbolTable *symtab,
                                    realpaver::Params *params, const char *filename);
extern int realpaver_flex_init_str(realpaver::Problem *prob,
                                   realpaver::SymbolTable *symtab, realpaver::Params *prm,
                                   const char *realpaver_str);
extern void realpaver_flex_cleanup_file(void);
extern void realpaver_flex_cleanup_str(void);
extern int realpaver_bison_parse(void);
extern std::string realpaver_parse_error;

/*----------------------------------------------------------------------------*/

namespace realpaver {

static std::vector<std::string> keywords = {
    "Variables", "Constants", "Constraints", "Aliases", "Functions", "Objectives",
    "integer",   "in",        "binary",      "tol",     "inf",       "MIN",
    "MAX",       "min",       "max",         "abs",     "sgn",       "sqr",
    "sqrt",      "pow",       "log",         "exp",     "cos",       "sin",
    "tan",       "cosh",      "sinh",        "tanh",    "if",        "then",
    "else",      "or",        "and",         "not"};

Parser::Parser()
    : symtab_()
    , params_()
{
}

Parser::Parser(const Params &params)
    : params_(params)
{
}

void Parser::initSymbolTable()
{
   // keywords
   for (auto k : keywords)
      symtab_.insertKeyword(k);

   // constants
   symtab_.insertConstant("PI", Interval::pi());
   symtab_.insertConstant("Pi", Interval::pi());
   symtab_.insertConstant("pi", Interval::pi());
}

std::string Parser::getParseError() const
{
   return realpaver_parse_error;
}

bool Parser::parseFile(const std::string &filename, Problem &problem)
{
   initSymbolTable();

   // initializes the lexical analyzer
   int res = realpaver_flex_init_file(&problem, &symtab_, &params_, filename.c_str());

   // parses the input file
   if (res == 0)
      res = realpaver_bison_parse();

   if (problem.getName() == "")
      problem.setName(filename);

   realpaver_flex_cleanup_file();
   symtab_.clear();

   return res == 0;
}

bool Parser::parseStr(const std::string &realpaver_str, Problem &problem)
{
   initSymbolTable();

   // initializes the lexical analyzer
   realpaver_flex_init_str(&problem, &symtab_, &params_, realpaver_str.c_str());

   // parses the input file
   int res = realpaver_bison_parse();

   if (problem.getName() == "")
      problem.setName("no_name");

   realpaver_flex_cleanup_str();
   symtab_.clear();

   return res == 0;
}

} // namespace realpaver
