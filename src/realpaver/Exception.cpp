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
 * @file   Exception.cpp
 * @brief  Class of exceptions
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

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
