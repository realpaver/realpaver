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
 * @file   Param.hpp
 * @brief  Parameters of solvers
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_PARAM_HPP
#define REALPAVER_PARAM_HPP

#include <string>
#include <unordered_map>

namespace realpaver {

/**
 * @brief Registers the set of parameters of an application.
 *
 * This class has a static instance storing the default values of the
 * parameters used to initialize the components of the library.
 */
class Param {
public:
   /// Constructor of an empty set
   Param();

   /// Default destructor
   ~Param() = default;

   /// Default copy constructor
   Param(const Param &) = default;

   /// Default assignment operator
   Param &operator=(const Param &) = default;

   /// Loads settings from a file
   void loadParam(const std::string &filename);

   /// Gets the value of an integral parameter name
   int getIntParam(const std::string &name) const;

   /// Sets the value of an integral parameter name to val
   void setIntParam(const std::string &name, int val);

   /// Gets the value of a real parameter name
   double getDblParam(const std::string &name) const;

   /// Sets the value of a real parameter name to val
   void setDblParam(const std::string &name, double val);

   /// Gets the value of a string parameter name
   std::string getStrParam(const std::string &name) const;

   /// Sets the value of a string parameter name to val
   void setStrParam(const std::string &name, const std::string &val);

   /// Writes the parameters on a stream
   void print(std::ostream &os) const;

   /// Loads settings from a file in the static instance
   static void LoadParam(const std::string &filename);

   /// Gets the value of an integral parameter in the static instance
   static int GetIntParam(const std::string &name);

   /// Sets the value of an integral parameter in the static instance
   static void SetIntParam(const std::string &name, int val);

   /// Gets the value of a real parameter in the static instance
   static double GetDblParam(const std::string &name);

   /// Sets the value of a real parameter in the static instance
   static void SetDblParam(const std::string &name, double val);

   /// Gets the value of a string parameter in the static instance
   static std::string GetStrParam(const std::string &name);

   /// Sets the value of a string parameter in the static instance
   static void SetStrParam(const std::string &name, const std::string &val);

   /// Writes the parameters of the static instance on a stream
   static void PrintParam(std::ostream &os);

private:
   static Param instance_; // default instance

   std::string path_;
   int lineno_;

   // maps of parameters
   std::unordered_map<std::string, int> intmap_;
   std::unordered_map<std::string, double> dblmap_;
   std::unordered_map<std::string, std::string> strmap_;

   // processing of lines in a settings file
   void processParam(const std::string &name, const std::string &val);
   void readParam(const std::string &line, size_t first);
   void processLine(const std::string &line);
   void throwEx();

   // utility functions
   static size_t eatSpaces(const std::string &line, size_t first);
   static bool isDigit(char c);
   static bool isSpace(char c);
   static bool isLetter(char c);
   static bool isCharName(char c);
   static bool isInt(const std::string &s);
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const Param &prm);

} // namespace realpaver

#endif
