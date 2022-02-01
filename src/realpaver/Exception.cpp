///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/Exception.hpp"

namespace realpaver {

Exception::Exception(const std::string& msg, const std::string& filename,
                     size_t lineno)
      : msg_("Error: " + msg),
        filename_(filename),
        lineno_(lineno)
{}

std::string Exception::what() const
{
   return filename_ + "(" + std::to_string(lineno_) + "): " + msg_;
}

std::string Exception::message() const
{
   return msg_;
}

std::string Exception::filename() const
{
   return filename_;
}

size_t Exception::lineno() const
{
   return lineno_;
}

} // namespace
