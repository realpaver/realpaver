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
 * @file   Parser.hpp
 * @brief  Wrapper for the parser of the modelling language
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_PARSER_HPP
#define REALPAVER_PARSER_HPP

#include "realpaver/Param.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/SymbolTable.hpp"

namespace realpaver {

/// Wrapper for the parser of the modelling language
class Parser {
public:
   /// Constructor
   Parser();

   /// Constructor
   Parser(const Params &params);

   /**
    * @brief Parses a model from a file.
    *
    * @param filename input file
    * @param problem output problem
    * @return true in case of success, false otherwise
    */
   bool parseFile(const std::string &filename, Problem &problem);

   /**
    * @brief Parses a model from a string.
    *
    * @param realpaver_str input string
    * @param problem output problem
    * @return true in case of success, false otherwise
    */
   bool parseStr(const std::string &realpaver_str, Problem &problem);

   /// Returns the parse error in case of failure
   std::string getParseError() const;

private:
   SymbolTable symtab_; // table of symbols
   Params params_;      // parameters

   void initSymbolTable();
};

} // namespace realpaver

#endif
