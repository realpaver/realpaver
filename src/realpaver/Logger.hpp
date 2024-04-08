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
 * @file   Logger.hpp
 * @brief  Management of log files
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#ifndef REALPAVER_LOGGER_HPP
#define REALPAVER_LOGGER_HPP

#include <fstream>
#include <iomanip>
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

/// Conversion function
std::string LogLevelToString(LogLevel level);

/// Conversion function
LogLevel StringToLogLevel(const std::string& s);

/**
 * @brief Management of log files.
 * 
 * A logger can write log messages to a log file.
 * The design of this class follows the singleton design pattern.
 * 
 * LOG(msg) writes a log.
 */
class Logger {
public:
   /// Connects the logger to a log file given a log level and a filename
   static void init(LogLevel level, const std::string& path);

   /// Returns the instance
   static Logger* getInstance();

   /// Returns the path of the log file
   std::string getPath() const;

   /// Returns the current log level of this
   LogLevel getLogLevel() const;

   /// Sets the log level of this
   void setLogLevel(LogLevel level);

   /// Returns the maximum size of a log file in bytes
   unsigned long getMaxSize() const;

   /// Returns the current size of the log file in bytes
   unsigned long getSize() const;

   /// Sets the maximum size of a log in bytes
   void setMaxSize(unsigned long nbytes);

   /// Writes a message
   void log(LogLevel level, const std::string& msg);

   /// Returns the number of digits of floats when printed on a stream
   std::streamsize floatPrecision() const;

   /// Assigns the number of digits of floats when printed on a stream
   std::streamsize setFloatPrecision(std::streamsize n);

   /// Inserts a new line in the log file
   void newline();

private:
   static Logger instance_;   // the instance
   bool connected_;           // true if it is connected
   LogLevel level_;           // current log level
   std::ofstream ofs_;        // log file
   std::string path_;         // path of the log file
   unsigned long maxsize_;    // maximum size of a log file in bytes
   std::streamsize fprec_;    // float precision (number of digits)

   Logger();
   ~Logger();
   Logger(const Logger&) = delete;
   Logger& operator=(const Logger&) = delete;
};

#if LOG_ON
#  define LOG(level, msg)                                         \
   if (level <= Logger::getInstance()->getLogLevel())             \
   {                                                              \
      do                                                          \
      {                                                           \
         std::ostringstream __os;                                 \
         __os.precision(Logger::getInstance()->floatPrecision()); \
         __os << msg;                                             \
         Logger::getInstance()->log(level, __os.str());           \
      }                                                           \
      while(0);                                                   \
   }
#else
#  define LOG(level, msg)
#endif

#define LOG_MAIN(msg)  LOG(LogLevel::main, msg)
#define LOG_INTER(msg) LOG(LogLevel::inter, msg)
#define LOG_LOW(msg)   LOG(LogLevel::low, msg)
#define LOG_FULL(msg)  LOG(LogLevel::full, msg)

#if LOG_ON
#  define LOG_NL(level)                                           \
   if (level <= Logger::getInstance()->getLogLevel())             \
   {                                                              \
      do                                                          \
      {                                                           \
         Logger::getInstance()->newline();                        \
      }                                                           \
      while(0);                                                   \
   }
#else
#  define LOG_NL(level)
#endif

#define LOG_NL_MAIN()  LOG_NL(LogLevel::main)
#define LOG_NL_INTER() LOG_NL(LogLevel::inter)
#define LOG_NL_LOW()   LOG_NL(LogLevel::low)
#define LOG_NL_FULL()  LOG_NL(LogLevel::full)

} // namespace

#endif
