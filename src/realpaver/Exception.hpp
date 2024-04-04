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
 * @file   Exception.hpp
 * @brief  Class of exceptions
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#ifndef REALPAVER_EXCEPTION_HPP
#define REALPAVER_EXCEPTION_HPP

#include <sstream>
#include <string>
#include "realpaver/config.hpp"

namespace realpaver {

/**
 * @brief Class of exceptions.
 * 
 * Two macros are defined for convenience:
 * - THROW(msg) throws an exception with message 'msg';
 * - THROW_IF(cond,msg) throws an exception with message 'msg' if the
 *   condition 'cond' is true.
 */
class Exception {
public:
   /**
    * @brief Constructor.
    * @param msg a message
    * @param filename name of the file where this exception is thrown
    * @param lineno line number in the file
    */
   Exception(const std::string& msg, const std::string& filename = "",
             size_t lineno = 0);

   /// Default copy constructor
   Exception(const Exception&) = default;

   /// No assignment
   Exception& operator=(const Exception&) = delete;

   /// Default destructor
   ~Exception() = default;

   /// Returns the full explanation of this
   std::string what() const;

   /// Returns the message of this
   std::string message() const;

   /// Returns the name of the file where this exception is thrown
   std::string filename() const;

   /// Returns the line number in the file
   size_t lineno() const;

private:
   std::string msg_;
   std::string filename_;
   size_t lineno_;
};

#define THROW(msg)                                          \
      do                                                    \
      {                                                     \
         std::ostringstream __os;                           \
         __os << msg;                                       \
         throw Exception(__os.str(),__FILE__,__LINE__);     \
      }                                                     \
      while(0)

#define THROW_IF(cond,msg)                                  \
   if (cond)                                                \
      THROW(msg)

} // namespace

#endif
