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
