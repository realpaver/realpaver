///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <ctime>
#include "realpaver/Logger.hpp"

namespace realpaver {

Logger Logger::instance_;

Logger::Logger() :
      connected_(false),
      level_(LogLevel::none),
      ofs_(),
      path_(""),
      maxsize_(1048576)
{}

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

size_t Logger::getSize() const
{
   std::ifstream f(path_, std::ios::in | std::ios::binary);
   f.seekg(0, std::ios::end);
   return f.tellg();
}

size_t Logger::getMaxSize() const
{
   return maxsize_;
}

void Logger::setMaxSize(size_t nbytes)
{
   maxsize_ = nbytes;
}

Logger* Logger::getInstance()
{
   return &instance_;
}

void Logger::init(LogLevel level, const std::string& path)
{
   THROW_IF(instance_.connected_, "the logger is already connected");

   instance_.ofs_.open(path, std::ofstream::out);

   THROW_IF(!instance_.ofs_.is_open(), "the log file cannot be open");

   instance_.path_      = path;
   instance_.connected_ = true;
   instance_.level_     = level;

   std::time_t now = time(nullptr);
   instance_.ofs_ << "log file "
                  << REALPAVER_STRING
                  << " @ "
                  << std::asctime(std::localtime(&now))
                  << std::endl;
}

void Logger::log(LogLevel level, const std::string& msg)
{
   THROW_IF(!instance_.connected_, "the logger is not connected");

   if (getSize() < getMaxSize())
      instance_.ofs_ << msg << std::endl;
}

} // namespace
