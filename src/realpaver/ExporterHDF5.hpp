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
 * @file   ExporterHDF5.hpp
 * @brief  HDF5 Output for RealPaver
 * @author Raphaël Chenouard
 * @date   2025-6-27
 */

#ifndef REALPAVER_EXPORTERHDF5_HPP
#define REALPAVER_EXPORTERHDF5_HPP

#include "H5Cpp.h"

#include "realpaver/DomainBox.hpp"
#include "realpaver/Exporter.hpp"

#include <memory>
#include <string>

namespace realpaver {

/**
 * @brief HDF5 exporter for RealPaver.
 *
 * Dump Space to H5 files.
 */

const int MAX_STR_LEN = 128;

// Compound datatype basis for the variables' info
typedef struct {
   char name[MAX_STR_LEN];
   double abstol;
   double reltol;
   double lower_bound;
   double upper_bound;
} VarDesc;

// Compound datatype basis for the nodes' info
typedef struct {
   char realpaver_version[MAX_STR_LEN];
   char problem_name[MAX_STR_LEN];
   double total_time;
   double preprocessing_time;
   int nb_var_fixed;
   int nb_ctr_inactive;
   int search_status;
   int solution_status;
} SolverDesc;

// Compound datatype basis for the nodes' info
typedef struct {
   size_t id;
   size_t parent_id;
   size_t depth;
   int proof;
} NodeDesc;

class ExporterHDF5 : public Exporter {
public:
   /// Constructor of ExporterHDF5 on a problem
   ExporterHDF5(const Problem &pb);

   /// Destructor
   ~ExporterHDF5();

   /// Default copy constructor
   ExporterHDF5(const ExporterHDF5 &) = default;

   /// No assignment
   ExporterHDF5 &operator=(const ExporterHDF5 &) = delete;

   /// Method for saving solver state to a h5 file
   void save(const Solver &solver, const std::string filename);

   /// Method for saving a complete space to a h5 file
   void save(const Space &space, const std::string filename);

private:
   /// Method for writing variables and aliases information
   void write_variables_info(H5::Group *g, const DomainBox &box) const;

   /// Method for writing solver information
   void write_solver_info(H5::Group *g, const Solver &s) const;

   /// Method for writing solutions information
   void write_solutions(H5::Group *g, const Space &s) const;

   /// Method for writing pending nodes information
   void write_pendings(H5::Group *g, const Space &s) const;
};

} // namespace realpaver

#endif
