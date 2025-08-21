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
 * @file   Exporter.cpp
 * @brief  File export for RealPaver
 * @author Raphaël Chenouard
 * @date   2025-6-27
 */

#include "realpaver/Exporter.hpp"

namespace realpaver {

Exporter::Exporter(const Problem &pb)
    : pp_(nullptr)
    , savePendings_(false)
{
   for (size_t i = 0; i < pb.nbAliases(); ++i)
   {
      Alias a = pb.aliasAt(i);
      if (pb.isAliasReported(a))
         aliasesToExport_.push_back(a);
   }
}

Exporter::~Exporter()
{
}

void Exporter::setSavePendings(bool s)
{
   savePendings_ = s;
}

} // namespace realpaver
