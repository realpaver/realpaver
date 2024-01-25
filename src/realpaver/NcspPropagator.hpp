///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_PROPAGATOR_HPP
#define REALPAVER_NCSP_PROPAGATOR_HPP

#include "realpaver/NcspNode.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of propagators for NCSPs.
///////////////////////////////////////////////////////////////////////////////
class NcspPropagator {
public:
   /// Default constructor
   NcspPropagator() = default;

   /// Default copy constructor
   NcspPropagator(const NcspPropagator&) = default;

   /// No assignment
   NcspPropagator& operator=(const NcspPropagator&) = delete;

   /// Virtual destructor
   virtual ~NcspPropagator();

   /// Contraction method
   /// @param a node whose boxt is contracted
   /// @return a certificate of proof
   virtual Proof contract(NcspNode& node) = 0;
   
   // TODO : ajouter un contexte
};

class NcspPropagatorHC4 : public NcspPropagator {

};

} // namespace

#endif
