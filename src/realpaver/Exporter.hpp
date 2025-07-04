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
 * @file   Exporter.hpp
 * @brief  File export for RealPaver
 * @author Raphaël Chenouard
 * @date   2025-6-27
 */

#ifndef REALPAVER_EXPORTER_HPP
#define REALPAVER_EXPORTER_HPP

#include "realpaver/Preprocessor.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Solver.hpp"

#include <string>

namespace realpaver {

/**
 * @brief Generic file exporter class for RealPaver.
 *
 * Dump Space object to a file.
 */

class Exporter {
public:
   /// Constructor of Exporter for a problem
   Exporter(const Problem &pb);

   /// Destructor
   ~Exporter();

   /// Default copy constructor
   Exporter(const Exporter &) = default;

   /// No assignment
   Exporter &operator=(const Exporter &) = delete;

   /// Method for saving solver state to a file
   virtual void save(const Solver &solver, const std::string filename) = 0;

   /// Method for saving a complete space to a file
   virtual void save(const Space &space, const std::string filename) = 0;

protected:
   std::vector<Alias> aliasesToExport_;
   SharedPreprocessor pp_;
};

} // namespace realpaver

#endif
