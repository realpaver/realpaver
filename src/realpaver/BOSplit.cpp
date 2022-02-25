///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOSplit.hpp"

namespace realpaver {

BOSplit::BOSplit(const Scope& scope)
      : SplitStrategy<SharedBONode>(),
        scope_(scope)
{
   ASSERT(!scope.isEmpty(), "Empty scope in a split object");
}

BOSplit::~BOSplit()
{
   // TODO   
}

bool BOSplit::applyImpl(SharedBONode node)
{
   ASSERT(scope_.maxIndex()<node->getRegion()->size(),
          "Inconsistency between scope and region in a BO node");

   // selectionner alternativement avec une frequence la stratégie
   // largest et la stratégie smear
   //
   // embarquer un vectur de tolérances pour les variables
   //
   // Comme IntervalVector -> ToleranceVector
   //
   // par défaut on prend les tolerances dans les objets variables
   //
   // on peut aussi affecter tout le vecteur...
}

} // namespace
