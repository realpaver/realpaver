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

#include <ctime>
#include "realpaver/Logger.hpp"

namespace realpaver {

Logger Logger::instance_;

Logger::Logger() : connected_(false), os_(nullptr), ofs_()
{}

Logger::~Logger()
{
   if (ofs_.is_open())
      ofs_.close();
}

Logger* Logger::getInstance()
{
   return &instance_;
}

void Logger::connectConsole(std::ostream& os)
{
   THROW_IF(instance_.connected_, "the logger is already connected");

   instance_.connected_ = true;
   instance_.os_ = &os;

   std::time_t now = time(nullptr);
   (*instance_.os_) << "run "
                    << REALPAVER_STRING
                    << " @ "
                    << std::asctime(std::localtime(&now))
                    << std::endl;
}

void Logger::connectFile(const std::string& filename)
{
   THROW_IF(instance_.connected_, "the logger is already connected");

   instance_.ofs_.open(filename, std::ofstream::out);

   THROW_IF(!instance_.ofs_.is_open(), "the log file cannot be open");

   instance_.connected_ = true;

   std::time_t now = time(nullptr);
   instance_.ofs_ << "log file "
                  << REALPAVER_STRING
                  << " @ "
                  << std::asctime(std::localtime(&now))
                  << std::endl;
}

void Logger::log(const std::string& s)
{
   THROW_IF(!instance_.connected_, "the logger is not connected");

   if (instance_.ofs_.is_open())
      instance_.ofs_ << s << std::endl;

   else
      (*instance_.os_) << s << std::endl;
}

} // namespace
