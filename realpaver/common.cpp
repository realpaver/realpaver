// This file is part of Realpaver. License: see COPYING file.

#include <ctime>
#include "realpaver/common.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, const Proof& p)
{
   switch(p)
   {
      case Proof::Empty:    return os << "empty";
      case Proof::Maybe:    return os << "maybe";
      case Proof::Feasible: return os << "feasible";
      case Proof::Inner:    return os << "inner";
      case Proof::Optimal:  return os << "optimal";
      default:              os.setstate(std::ios::failbit);
   }
   return os;
}

///////////////////////////////////////////////////////////////////////////////

Menhir::Menhir(const std::string& msg, std::string const& filename,
               size_t lineno) : msg_("Error: " + msg),
                                filename_(filename), lineno_(lineno)
{}

std::string Menhir::what() const
{
   return filename_ + "(" + std::to_string(lineno_)
                    + "): " + msg_;
}

///////////////////////////////////////////////////////////////////////////////

Logger Logger::instance_;

Logger::Logger() : connected_(false), os_(nullptr), ofs_()
{}

Logger::~Logger()
{
   if (ofs_.is_open())
      ofs_.close();
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
