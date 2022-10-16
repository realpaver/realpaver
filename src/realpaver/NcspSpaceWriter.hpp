///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_SPACE_WRITER_HPP
#define REALPAVER_NCSP_SPACE_WRITER_HPP

#include "realpaver/NcspSpace.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a base class for writers of NCSP spaces.
///////////////////////////////////////////////////////////////////////////////
class NcspSpaceWriter {
public:
   /// Default constructor
   NcspSpaceWriter() = default;

   /// Virtual destructor
   virtual ~NcspSpaceWriter();

   /// No assignment
   NcspSpaceWriter& operator=(const NcspSpaceWriter&) = delete;

   /// Default copy constructor
   NcspSpaceWriter(const NcspSpaceWriter&) = default;

   /// Writes a space
   /// @param space space written
   virtual void write(const NcspSpace& space) = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// This writes the solutions of NCSP spaces on a file.
///////////////////////////////////////////////////////////////////////////////
class NcspSpaceFileWriter : public NcspSpaceWriter {
public:
   /// Constructor
   /// @param filename name of output file
   /// @param pending true for writing pending nodes, false otherwise
   NcspSpaceFileWriter(const std::string& filename, bool pending = false);

   /// Destructor
   ~NcspSpaceFileWriter() = default;

   /// No assignment
   NcspSpaceFileWriter& operator=(const NcspSpaceFileWriter&) = delete;

   /// Default copy constructor
   NcspSpaceFileWriter(const NcspSpaceFileWriter&) = default;

   /// Overrides
   void write(const NcspSpace& space);

private:
   std::string filename_;
   bool pending_;
};

} // namespace

#endif
