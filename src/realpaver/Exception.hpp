///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_EXCEPTION_HPP
#define REALPAVER_EXCEPTION_HPP

#include <sstream>
#include <string>
#include "realpaver/config.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an exception.
///
/// Two macros are defined for convenience:
/// - THROW(msg) throws an exception with message 'msg';
/// - THROW_IF(cond,msg) throws an exception with message 'msg' if the
///   condition 'cond' is true.
///////////////////////////////////////////////////////////////////////////////
class Exception {
public:
   /// Creates an exception
   /// @param msg      a message
   /// @param filename name of the file where this exception is thrown
   /// @param lineno   line number in the file
   Exception(const std::string& msg, const std::string& filename = "",
             size_t lineno = 0);

   /// Default copy constructor
   Exception(const Exception&) = default;

   /// No assignment
   Exception& operator=(const Exception&) = delete;

   /// Default destructor
   ~Exception() = default;

   /// @return the full explanation of this
   std::string what() const;

   /// @return the message of this
   std::string message() const;

   /// @return the name of the file where this exception is thrown
   std::string filename() const;

   /// @return the line number in the file
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
