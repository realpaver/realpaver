///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include "realpaver/NcspSpaceWriter.hpp"

namespace realpaver {

NcspSpaceWriter::~NcspSpaceWriter()
{}

///////////////////////////////////////////////////////////////////////////////

NcspSpaceFileWriter::NcspSpaceFileWriter(const std::string& filename,
                                         bool pending)
      : NcspSpaceWriter(),
        filename_(filename),
        pending_(pending)
{}

void NcspSpaceFileWriter::write(const NcspSpace& space)
{
   std::ofstream sf;
   sf.open(filename_, std::ofstream::out);

   if (sf.bad()) THROW("Bad solution filename: " << filename_);

   size_t ns = space.nbSolutionNodes();
   size_t np = space.nbPendingNodes();
   SharedNcspNode node;

   // writes the variables
   if (ns + np > 0)
   {
      node = (ns > 0) ? space.getSolutionNode(0) : space.getPendingNode(0);
      Scope sco = node->scope();

      auto it = sco.begin();
      sf << "VAR " << (*it).getName();
      while (++it != sco.end()) sf << ", " << (*it).getName();
      sf << std::endl << std::endl;
   }

   // writes the solutions
   sf << "SOLUTION " << ns << std::endl;

   for (size_t i=0; i<ns; ++i)
   {
      node = space.getSolutionNode(i);
      Proof proof = node->getProof();
      sf << "#" << (i+1) << " ";
      switch(proof)
      {
         case Proof::Feasible: sf << "(F)"; break;
         case Proof::Inner:    sf << "(I)"; break;
         default:              sf << "(U)";
      }
      sf << " " << (*node->region()) << std::endl;
   }

   if (pending_)
   {
      if (ns > 0) sf << std::endl;

      sf << "PENDING " << np << std::endl;
      for (size_t i=0; i<np; ++i)
      {
         node = space.getPendingNode(i);
         sf << "#" << (i+1) << " " << (*node->region()) << std::endl;
      }
   }
   sf.close();
}

} // namespace
