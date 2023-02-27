///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_PARAM_HPP
#define REALPAVER_PARAM_HPP

#include <string>
#include <unordered_map>
#include "realpaver/Interval.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This registers the parameters of an application.
///
/// This class has a static instance storing the default values of the
/// parameters used to initialize the components of the library.
///////////////////////////////////////////////////////////////////////////////
class Param {
public:
   /// Creates an instance
   Param();

   /// Default destructor
   ~Param() = default;

   /// Default copy constructor
   Param(const Param&) = default;

   /// Default assignment operator
   Param& operator=(const Param&) = default;

   /// Loads settings from a file
   /// @param name of the file
   void loadParam(const std::string& filename);

   /// Gets the value of an integral parameter
   /// @param name name of parameter
   /// @return value of parameter
   int getIntParam(const string& name) const;

   /// Sets the value of an integral parameter
   /// @param name name of parameter
   /// @param val value assigned
   void setIntParam(const string& name, int val);

   /// Gets the value of a real parameter
   /// @param name name of parameter
   /// @return value of parameter
   double getDblParam(const string& name) const;

   /// Sets the value of a real parameter
   /// @param name name of parameter
   /// @param val value assigned
   void setDblParam(const string& name, double val);

   /// Gets the value of a tolerance parameter
   /// @param name name of parameter
   /// @return value of parameter
   Tolerance getTolParam(const string& name) const;

   /// Sets the value of a tolerance parameter
   /// @param name name of parameter
   /// @param val value assigned
   void setTolParam(const string& name, const Tolerance& val);

   /// Gets the value of a string parameter
   /// @param name name of parameter
   /// @return value of parameter
   std::string getStrParam(const string& name) const;

   /// Sets the value of a string parameter
   /// @param name name of parameter
   /// @param val value assigned
   void setStrParam(const string& name, const std::string& val);

   /// Writes the parameters on a stream
   /// @param os output stream
   void print(std::ostream& os) const;

   /// Loads settings from a file in the static instance
   /// @param name of the file
   static void LoadParam(const std::string& filename);

   /// Gets the value of an integral parameter in the static instance
   /// @param name name of parameter
   /// @return value of parameter
   static int GetIntParam(const string& name);

   /// Sets the value of an integral parameter in the static instance
   /// @param name name of parameter
   /// @param val value assigned
   static void SetIntParam(const string& name, int val);

   /// Gets the value of a real parameter in the static instance
   /// @param name name of parameter
   /// @return value of parameter
   static double GetDblParam(const string& name);

   /// Sets the value of a real parameter in the static instance
   /// @param name name of parameter
   /// @param val value assigned
   static void SetDblParam(const string& name, double val);

   /// Gets the value of a tolerance parameter in the static instance
   /// @param name name of parameter
   /// @return value of parameter
   static Tolerance GetTolParam(const string& name);

   /// Sets the value of a tolerance parameter in the static instance
   /// @param name name of parameter
   /// @param val value assigned
   static void SetTolParam(const string& name, const Tolerance& val);

   /// Gets the value of a string parameter in the static instance
   /// @param name name of parameter
   /// @return value of parameter
   static std::string GetStrParam(const string& name);

   /// Sets the value of a string parameter in the static instance
   /// @param name name of parameter
   /// @param val value assigned
   static void SetStrParam(const string& name, const std::string& val);

   /// Writes the parameters of the static instance on a stream
   /// @param os output stream
   static void PrintParam(std::ostream& os);

private:
   static Param instance_;  // default instance

   std::string path_;
   int lineno_;
   std::unordered_map<std::string, Tolerance> tolmap_;
   std::unordered_map<std::string, int> intmap_;
   std::unordered_map<std::string, double> dblmap_;
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

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Param& prm);

} // namespace

#endif
