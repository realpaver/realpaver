// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_COMMON_HPP
#define REALPAVER_COMMON_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "realpaver/config.hpp"

namespace realpaver {

/*****************************************************************************
 * Hash functions.
 *****************************************************************************/
template <typename T>
size_t Hash1(const T& h)
{
   std::hash<T> fun;
   return fun(h);
}

inline size_t Hash2(size_t h1, size_t h2)
{
   return h1 ^ (h2 << 1);
}

inline size_t Hash3(size_t h1, size_t h2, size_t h3)
{
   return Hash2(h1,Hash2(h2,h3));
}

inline size_t Hash4(size_t h1, size_t h2, size_t h3, size_t h4)
{
   return Hash2(h1,Hash3(h2,h3,h4));
}

/*****************************************************************************
 * Certificates returned by provers.
 *****************************************************************************/
enum class Proof {
   Empty,      // empty solution set
   Maybe,      // no proof is derived
   Feasible,   // proof of existence of a solution
   Inner,      // proof of inner box
   Optimal     // proof of optimality
};

std::ostream& operator<<(std::ostream& os, const Proof& p);

/*****************************************************************************
 * Class of exceptions.
 * 
 * THROW(msg) throws an exception with message "msg".
 * THROW_IF(cond,msg) throws an exception with message "msg" if "cond" is true.
 *****************************************************************************/
class Menhir {
public:
   Menhir(const std::string& msg, const std::string& filename = "",
          size_t lineno = 0);

   // returns the full explanation
   std::string what() const;

   // returns the message
   std::string message() const;

   // returns the filename from which this exception has been thrown
   std::string filename() const;

   // returns the line number in filename()
   size_t lineno() const;

private:
   std::string msg_;       // message
   std::string filename_;  // file name
   size_t lineno_;         // line number
};

inline std::string Menhir::message() const
{
   return msg_;
}

inline std::string Menhir::filename() const
{
   return filename_;
}

inline size_t Menhir::lineno() const
{
   return lineno_;
}

#define THROW(msg)                                          \
      do                                                    \
      {                                                     \
         std::ostringstream __os;                           \
         __os << msg;                                       \
         throw Menhir(__os.str(),__FILE__,__LINE__);        \
      }                                                     \
      while(0)

#define THROW_IF(cond,msg)                                  \
   if (cond)                                                \
      THROW(msg)

/*****************************************************************************
 * Class of loggers writing log messages to the console or a log file.
 * The class is implemented following the singleton design pattern.
 * 
 * LOG(msg) writes a log.
 *****************************************************************************/
class Logger {
public:
   // connects the logger to the console
   static void connectConsole(std::ostream& os = std::cout);

   // connects the logger to a log file
   static void connectFile(const std::string& filename);

   // returns the only instance
   static Logger* getInstance();

   // writes a message
   void log(const std::string& s);

private:
   static Logger instance_;   // the only instance
   bool connected_;           // true if it is connected
   std::ostream *os_;         // console
   std::ofstream ofs_;        // or file

   Logger();
   ~Logger();
   Logger(const Logger&) = delete;
   Logger& operator=(const Logger&) = delete;
};

inline Logger* Logger::getInstance()
{
   return &instance_;
}

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

#if DEBUG_ON
#   define DEBUG(msg)   std::cerr << msg << std::endl
#else
#   define DEBUG(msg)
#endif

#if ASSERT_ON
#   define ASSERT(cond, msg)                                \
      if (!(cond))                                            \
         THROW(msg)
#else
#   define ASSERT(cond, msg)
#endif

} // namespace

#endif
