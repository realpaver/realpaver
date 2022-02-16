///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_PARAM_HPP
#define REALPAVER_PARAM_HPP

#include <string>
#include <unordered_map>
#include "realpaver/interval.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This registers the parameters of the application.
///
/// The design of this class follows the singleton design pattern.
///////////////////////////////////////////////////////////////////////////////
class Param {
public:
   /// Loads settings from a file
   /// @param name of the file
   static void init(const std::string& filename);

   /// Gets the value of an integral parameter
   /// @param name name of parameter
   /// @return value of parameter
   static int getIntParam(const string& name);

   /// Sets the value of an integral parameter
   /// @param name name of parameter
   /// @param val value assigned
   static void setIntParam(const string& name, int val);

   /// Gets the value of a real parameter
   /// @param name name of parameter
   /// @return value of parameter
   static double getDblParam(const string& name);

   /// Sets the value of a real parameter
   /// @param name name of parameter
   /// @param val value assigned
   static void setDblParam(const string& name, double val);

   /// Gets the value of a tolerance parameter
   /// @param name name of parameter
   /// @return value of parameter
   static Tolerance getTolParam(const string& name);

   /// Sets the value of a tolerance parameter
   /// @param name name of parameter
   /// @param val value assigned
   static void setTolParam(const string& name, const Tolerance& val);

   /// Gets the value of a string parameter
   /// @param name name of parameter
   /// @return value of parameter
   static std::string getStrParam(const string& name);

   /// Sets the value of a string parameter
   /// @param name name of parameter
   /// @param val value assigned
   static void setStrParam(const string& name, const std::string& val);

   /// Writes the parameters onna stream
   /// @param os output stream
   static void print(std::ostream& os);

private:
   static Param instance_;    // singleton
   std::string path_;         // file name
   int lineno_;               // line number in this file

   // creates the instance
   Param();

   // destroys the instance
   ~Param();

   // Copy and assignment protection
   Param(const Param&) = delete;
   Param& operator=(const Param&) = delete;

   // map of tolerances
   std::unordered_map<std::string, Tolerance> tolmap_;

   // map of integers
   std::unordered_map<std::string, int> intmap_;

   // map of doubles
   std::unordered_map<std::string, double> dblmap_;

   // map of strings
   std::unordered_map<std::string, std::string> strmap_;

   // processing of lines in a settings file
   void processParam(const std::string& name, const std::string& val);
   void readParam(const std::string& line, size_t first);
   void processLine(const std::string& line);
   void throwEx();

   // utility functions
   static size_t eatSpaces(const std::string& line, size_t first);
   static bool isDigit(char c);
   static bool isSpace(char c);
   static bool isLetter(char c);
   static bool isCharName(char c);
   static bool isInt(const std::string& s);
};

} // namespace

#endif
