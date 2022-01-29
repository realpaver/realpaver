///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, a reliable interval solver of nonlinear   //
//                                 constraint satisfaction and optimization  //
//                                 problems over the real numbers.           //
//                                                                           //
// Copyright (C) 2020-2022 Laboratoire des Sciences du Numérique de Nantes   //
//                                                                           //
// Realpaver is a software distributed under the terms of the MIT License.   //
// See the file COPYING.                                                     //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LOGGER_HPP
#define REALPAVER_LOGGER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "realpaver/config.hpp"
#include "realpaver/Exception.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a logger.
///
/// A logger can write log messages to the console or a log file.
/// This design of this class follows the singleton design pattern.
///
/// LOG(msg) writes a log.
///////////////////////////////////////////////////////////////////////////////
class Logger {
public:
   /// Connects the logger to an output stream
   /// @param os output stream
   static void connectConsole(std::ostream& os = std::cout);

   /// Connects the logger to a log file
   /// @param filename name of the file
   static void connectFile(const std::string& filename);

   /// @return the instance
   static Logger* getInstance();

   /// Writes a message
   void log(const std::string& msg);

private:
   static Logger instance_;   // the instance
   bool connected_;           // true if it is connected
   std::ostream *os_;         // either a console
   std::ofstream ofs_;        // or a file

   Logger();
   ~Logger();
   Logger(const Logger&) = delete;
   Logger& operator=(const Logger&) = delete;
};

#if LOG_ON
#   define LOG(msg)                                         \
      do                                                    \
      {                                                     \
         std::ostringstream __os;                           \
         __os << msg;                                       \
         Logger::getInstance()->log(__os.str());            \
      }                                                     \
      while(0)
#else
#   define LOG(msg)
#endif

} // namespace

#endif
