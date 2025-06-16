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
 * @file   Logger.cpp
 * @brief  Logger
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/Logger.hpp"
#include <ctime>
#include <limits>

namespace realpaver {

Logger Logger::instance_;

Logger::Logger()
    : connected_(false)
    , level_(LogLevel::none)
    , ofs_()
    , path_("")
    , maxsize_(std::numeric_limits<unsigned long>::max())
    , fprec_(8)
{
}

Logger::~Logger()
{
   if (ofs_.is_open())
      ofs_.close();
}

std::string Logger::getPath() const
{
   return path_;
}

LogLevel Logger::getLogLevel() const
{
   return level_;
}

void Logger::setLogLevel(LogLevel level)
{
   level_ = level;
}

unsigned long Logger::getSize() const
{
   std::ifstream f(path_, std::ios::in | std::ios::binary);
   f.seekg(0, std::ios::end);
   return f.tellg();
}

unsigned long Logger::getMaxSize() const
{
   return maxsize_;
}

void Logger::setMaxSize(unsigned long nbytes)
{
   maxsize_ = nbytes;
}

Logger *Logger::getInstance()
{
   return &instance_;
}

void Logger::init(LogLevel level, const std::string &path)
{
   THROW_IF(instance_.connected_, "The logger is already connected");

   instance_.ofs_.open(path, std::ofstream::out);

   THROW_IF(!instance_.ofs_.is_open(), "The log file cannot be open");

   instance_.path_ = path;
   instance_.connected_ = true;
   instance_.level_ = level;

   std::time_t now = time(nullptr);
   instance_.ofs_ << "Log file " << REALPAVER_STRING << " @ "
                  << std::asctime(std::localtime(&now)) << std::endl;
}

void Logger::log(LogLevel level, const std::string &msg)
{
   THROW_IF(!instance_.connected_, "The logger is not connected");

   if (getSize() < getMaxSize())
   {
      std::string s = LogLevelToString(level) + ".";
      instance_.ofs_ << std::setw(7) << std::left << s << msg << std::endl;
   }
}

std::streamsize Logger::floatPrecision() const
{
   return fprec_;
}

std::streamsize Logger::setFloatPrecision(std::streamsize n)
{
   std::streamsize old = fprec_;
   fprec_ = n;
   return old;
}

void Logger::newline()
{
   ofs_ << std::endl;
}

std::string LogLevelToString(LogLevel level)
{
   switch (level)
   {
   case LogLevel::none:
      return "NONE";
   case LogLevel::main:
      return "MAIN";
   case LogLevel::inter:
      return "INTER";
   case LogLevel::low:
      return "LOW";
   case LogLevel::full:
      return "FULL";
   }
   return "NONE";
}

LogLevel StringToLogLevel(const std::string &s)
{
   if (s == "NONE")
      return LogLevel::none;
   if (s == "MAIN")
      return LogLevel::main;
   if (s == "INTER")
      return LogLevel::inter;
   if (s == "LOW")
      return LogLevel::low;
   if (s == "FULL")
      return LogLevel::full;
   THROW("Symbol '" << s << "' is not a log level");
}

} // namespace realpaver
