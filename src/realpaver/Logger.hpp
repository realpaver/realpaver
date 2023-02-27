///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
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

// classical levels: none, fatal, error, warning, info, debug, verbose

/// Logging levels
enum class LogLevel {
   none    ,    ///< no log
   main    ,    ///< main level e.g. display results of first-class algorithms
   inter   ,    ///< intermediary level e.g. display nodes in a search algorithm
   low     ,    ///< low level e.g. display quantities in iterative methods
   full         ///< verbose mode
};

std::string LogLevelToString(LogLevel level);
LogLevel StringToLogLevel(const std::string& s);

///////////////////////////////////////////////////////////////////////////////
/// This is a logger.
///
/// A logger can write log messages to a log file.
/// The design of this class follows the singleton design pattern.
///
/// LOG(msg) writes a log.
///////////////////////////////////////////////////////////////////////////////
class Logger {
public:
   /// Connects the logger to a log file
   /// @param level log level
   /// @param filename name of the log file
   static void init(LogLevel level, const std::string& path);

   /// @return the instance
   static Logger* getInstance();

   /// @return the path of the log file
   std::string getPath() const;

   /// @return the current log level of this
   LogLevel getLogLevel() const;

   /// Sets the log level of this
   /// @param new log level
   void setLogLevel(LogLevel level);

   /// @return the maximum size of a log file in bytes
   size_t getMaxSize() const;

   /// @return the current size of the log file in bytes
   size_t getSize() const;

   /// Sets the maximum size of a log file
   /// @param nbytes maximum size of a log file in bytes
   void setMaxSize(size_t nbytes);

   /// Writes a message
   void log(LogLevel level, const std::string& msg);

private:
   static Logger instance_;   // the instance
   bool connected_;           // true if it is connected
   LogLevel level_;           // current log level
   std::ofstream ofs_;        // log file
   std::string path_;         // path of the log file
   size_t maxsize_;           // maximum size of a log file in bytes

   Logger();
   ~Logger();
   Logger(const Logger&) = delete;
   Logger& operator=(const Logger&) = delete;
};

#if LOG_ON
#  define LOG(level, msg)                                   \
   if (level <= Logger::getInstance()->getLogLevel())       \
   {                                                        \
      do                                                    \
      {                                                     \
         std::ostringstream __os;                           \
         __os << msg;                                       \
         Logger::getInstance()->log(level, __os.str());     \
      }                                                     \
      while(0);                                             \
   }
#else
#  define LOG(level, msg)
#endif

#define LOG_MAIN(msg)  LOG(LogLevel::main, msg)
#define LOG_INTER(msg) LOG(LogLevel::inter, msg)
#define LOG_LOW(msg)   LOG(LogLevel::low, msg)
#define LOG_FULL(msg)  LOG(LogLevel::full, msg)

} // namespace

#endif
