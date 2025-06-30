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

#include "DomainBox.hpp"
#include "NodeContext.hpp"
#include "realpaver/Domain.hpp"
#include "realpaver/ExporterHDF5.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

const int rank = 2; // One line per solution, one column per dimension

int proof2int(Proof pf)
{
   switch (pf)
   {
   case Proof::Empty:
      return 0;
   case Proof::Maybe:
      return 1;
   case Proof::Feasible:
      return 2;
   case Proof::Inner:
      return 3;
   }
   return -1;
}

ExporterHDF5::ExporterHDF5(const Problem &pb)
{
   for (size_t i = 0; i < pb.nbAliases(); ++i)
   {
      Alias a = pb.aliasAt(i);
      if (pb.isAliasReported(a))
         aliasesToExport_.push_back(a);
   }
}

ExporterHDF5::~ExporterHDF5()
{
}

void ExporterHDF5::write_solver_info(H5::Group *g, const Solver &s) const
{
   // Create string datatype
   // Use fixed-length string type
   H5::StrType H5Str(H5::PredType::C_S1, MAX_STR_LEN);
   // Create variable descriptions
   H5::CompType SolverInfo(sizeof(SolverDesc));
   SolverInfo.insertMember("realpaver_version", HOFFSET(SolverDesc, realpaver_version),
                           H5Str);
   SolverInfo.insertMember("problem_name", HOFFSET(SolverDesc, problem_name), H5Str);
   SolverInfo.insertMember("total_time", HOFFSET(SolverDesc, total_time),
                           H5::PredType::NATIVE_DOUBLE);
   SolverInfo.insertMember("preprocessing_time", HOFFSET(SolverDesc, preprocessing_time),
                           H5::PredType::NATIVE_DOUBLE);
   SolverInfo.insertMember("nb_var_fixed", HOFFSET(SolverDesc, nb_var_fixed),
                           H5::PredType::NATIVE_INT);
   SolverInfo.insertMember("nb_ctr_inactive", HOFFSET(SolverDesc, nb_ctr_inactive),
                           H5::PredType::NATIVE_INT);
   SolverInfo.insertMember("search_status", HOFFSET(SolverDesc, search_status),
                           H5::PredType::NATIVE_INT);
   SolverInfo.insertMember("solution_status", HOFFSET(SolverDesc, solution_status),
                           H5::PredType::NATIVE_INT);

   SolverDesc solverInfo[1];
   strcpy(solverInfo[0].realpaver_version, REALPAVER_VERSION);
   strcpy(solverInfo[0].problem_name, s.getProblem().getName().c_str());
   solverInfo[0].total_time = s.getSolvingTime();
   solverInfo[0].preprocessing_time = s.getPreprocessor()->elapsedTime();
   solverInfo[0].nb_var_fixed = s.getPreprocessor()->nbFixedVars();
   solverInfo[0].nb_ctr_inactive = s.getPreprocessor()->nbInactiveCtrs();
   solverInfo[0].search_status = s.nbPendingNodes() > 0;
   solverInfo[0].solution_status = s.nbSolutions() > 0;

   // Define dataspace (1D array with size equal to number of strings)
   hsize_t solver_dims[1] = {1};
   H5::DataSpace solver_dspace(1, solver_dims);

   // Create dataset
   H5::DataSet str_dset = g->createDataSet("SOLVER", SolverInfo, solver_dspace);

   // Write data to dataset
   str_dset.write(solverInfo, SolverInfo);
}

void ExporterHDF5::write_variables_info(H5::Group *g, const DomainBox &box) const
{
   Scope s = box.scope();
   size_t N = s.size() + aliasesToExport_.size();

   // Create string datatype
   // Use fixed-length string type
   H5::StrType H5Str(H5::PredType::C_S1, MAX_STR_LEN);
   // Create variable descriptions
   H5::CompType VarInfo(sizeof(VarDesc));
   VarInfo.insertMember("name", HOFFSET(VarDesc, name), H5Str);
   VarInfo.insertMember("abstol", HOFFSET(VarDesc, abstol), H5::PredType::NATIVE_DOUBLE);
   VarInfo.insertMember("reltol", HOFFSET(VarDesc, reltol), H5::PredType::NATIVE_DOUBLE);
   VarInfo.insertMember("lower", HOFFSET(VarDesc, lower_bound),
                        H5::PredType::NATIVE_DOUBLE);
   VarInfo.insertMember("upper", HOFFSET(VarDesc, upper_bound),
                        H5::PredType::NATIVE_DOUBLE);

   std::vector<VarDesc> varInfo(N);
   for (size_t i = 0; i < s.size(); i++)
   {
      Variable v = s.var(i);
      strcpy(varInfo[i].name, v.getName().c_str());
      varInfo[i].abstol = v.getTolerance().getAbsTol();
      varInfo[i].reltol = v.getTolerance().getRelTol();
      varInfo[i].lower_bound = v.getDomain()->intervalHull().left();
      varInfo[i].upper_bound = v.getDomain()->intervalHull().right();
   }
   for (size_t k = 0; k < aliasesToExport_.size(); k++)
   {
      strcpy(varInfo[s.size() + k].name, aliasesToExport_[k].name().c_str());
      varInfo[s.size() + k].abstol = -1; // since it is an alias, no tolerance
      varInfo[s.size() + k].reltol = -1; // since it is an alias, no tolerance
      Interval d = aliasesToExport_[k].term().eval(box)->intervalHull();
      varInfo[s.size() + k].lower_bound = d.left();
      varInfo[s.size() + k].upper_bound = d.right();
   }

   // Define dataspace (1D array with size equal to number of strings)
   hsize_t var_dims[1] = {N};
   H5::DataSpace var_dspace(1, var_dims);

   // Create dataset
   H5::DataSet str_dset = g->createDataSet("VARIABLES", VarInfo, var_dspace);

   // Write data to dataset
   str_dset.write(varInfo.data(), VarInfo);
}

void ExporterHDF5::write_solutions(H5::Group *g, const Space &space) const
{
   const hsize_t nbNod = space.nbSolNodes();
   if (nbNod > 0)
   {
      const int nbVar = space.getSolNode(0)->scope().size();
      const hsize_t nbDim = nbVar + aliasesToExport_.size();

      H5::CompType NodeInfo(sizeof(NodeDesc));
      NodeInfo.insertMember("id", HOFFSET(NodeDesc, id), H5::PredType::NATIVE_INT);
      NodeInfo.insertMember("parent_id", HOFFSET(NodeDesc, parent_id),
                            H5::PredType::NATIVE_INT);
      NodeInfo.insertMember("depth", HOFFSET(NodeDesc, depth), H5::PredType::NATIVE_INT);
      NodeInfo.insertMember("proof", HOFFSET(NodeDesc, proof), H5::PredType::NATIVE_INT);

      hsize_t dimsf_sol_l[rank] = {nbNod, nbDim}; // dataset for lower bounds
      hsize_t dimsf_sol_u[rank] = {nbNod, nbDim}; // dataset for upper bounds
      H5::DataSpace dspace_sol_l(rank, dimsf_sol_l);
      H5::DataSpace dspace_sol_u(rank, dimsf_sol_u);
      H5::DataSet dset_sol_l =
          g->createDataSet("LOWER_BOUNDS", H5::PredType::NATIVE_DOUBLE, dspace_sol_l);
      H5::DataSet dset_sol_u =
          g->createDataSet("UPPER_BOUNDS", H5::PredType::NATIVE_DOUBLE, dspace_sol_u);

      hsize_t node_dims[1] = {nbNod};
      H5::DataSpace dspace_node(1, node_dims);
      // Create dataset
      H5::DataSet node_dset = g->createDataSet("NODES", NodeInfo, dspace_node);

      double l_values[nbNod][nbDim];
      double u_values[nbNod][nbDim];
      std::vector<NodeDesc> nodeInfo(nbNod);
      for (size_t i = 0; i < nbNod; i++)
      {
         SharedNode sol = space.getSolNode(i);
         nodeInfo[i].proof = proof2int(sol->getProof());
         nodeInfo[i].id = sol->index();
         nodeInfo[i].parent_id = sol->parent();
         nodeInfo[i].depth = sol->depth();
         for (size_t j = 0; j < nbVar; j++)
         {
            Interval d = sol->box()->get(j)->intervalHull();
            l_values[i][j] = d.left();
            u_values[i][j] = d.right();
         }
         for (size_t k = 0; k < aliasesToExport_.size(); k++)
         {
            Interval d = aliasesToExport_[k].term().eval(*(sol->box()))->intervalHull();
            l_values[i][nbVar + k] = d.left();
            u_values[i][nbVar + k] = d.right();
         }
      }
      dset_sol_l.write(l_values, H5::PredType::NATIVE_DOUBLE);
      dset_sol_u.write(u_values, H5::PredType::NATIVE_DOUBLE);
      node_dset.write(nodeInfo.data(), NodeInfo);
   }
}

void ExporterHDF5::write_pendings(H5::Group *g, const Space &space) const
{
   const hsize_t nbNod = space.nbPendingNodes();
   if (nbNod > 0)
   {
      const int nbVar = space.getPendingNode(0)->scope().size();
      const hsize_t nbDim = nbVar + aliasesToExport_.size();

      H5::CompType NodeInfo(sizeof(NodeDesc));
      NodeInfo.insertMember("id", HOFFSET(NodeDesc, id), H5::PredType::NATIVE_INT);
      NodeInfo.insertMember("parent_id", HOFFSET(NodeDesc, parent_id),
                            H5::PredType::NATIVE_INT);
      NodeInfo.insertMember("depth", HOFFSET(NodeDesc, depth), H5::PredType::NATIVE_INT);
      NodeInfo.insertMember("proof", HOFFSET(NodeDesc, proof), H5::PredType::NATIVE_INT);

      hsize_t dimsf_sol_l[rank] = {nbNod, nbDim}; // dataset for lower bounds
      hsize_t dimsf_sol_u[rank] = {nbNod, nbDim}; // dataset for upper bounds
      H5::DataSpace dspace_sol_l(rank, dimsf_sol_l);
      H5::DataSpace dspace_sol_u(rank, dimsf_sol_u);
      H5::DataSet dset_sol_l =
          g->createDataSet("LOWER_BOUNDS", H5::PredType::NATIVE_DOUBLE, dspace_sol_l);
      H5::DataSet dset_sol_u =
          g->createDataSet("UPPER_BOUNDS", H5::PredType::NATIVE_DOUBLE, dspace_sol_u);

      hsize_t node_dims[1] = {nbNod};
      H5::DataSpace dspace_node(1, node_dims);
      // Create dataset
      H5::DataSet node_dset = g->createDataSet("NODES", NodeInfo, dspace_node);

      double l_values[nbNod][nbDim];
      double u_values[nbNod][nbDim];
      std::vector<NodeDesc> nodeInfo(nbNod);
      for (size_t i = 0; i < nbNod; i++)
      {
         SharedNode sol = space.getPendingNode(i);
         nodeInfo[i].proof = proof2int(sol->getProof());
         nodeInfo[i].id = sol->index();
         nodeInfo[i].parent_id = sol->parent();
         nodeInfo[i].depth = sol->depth();
         for (size_t j = 0; j < nbVar; j++)
         {
            Interval d = sol->box()->get(j)->intervalHull();
            l_values[i][j] = d.left();
            u_values[i][j] = d.right();
         }
         for (size_t k = 0; k < aliasesToExport_.size(); k++)
         {
            Interval d = aliasesToExport_[k].term().eval(*(sol->box()))->intervalHull();
            l_values[i][nbVar + k] = d.left();
            u_values[i][nbVar + k] = d.right();
         }
      }
      dset_sol_l.write(l_values, H5::PredType::NATIVE_DOUBLE);
      dset_sol_u.write(u_values, H5::PredType::NATIVE_DOUBLE);
      node_dset.write(nodeInfo.data(), NodeInfo);
   }
}

/// Reporting method for a complete space to a h5 file
void ExporterHDF5::save(const Solver &solver, const std::string filename) const
{
   Scope s = solver.getProblem().scope();
   const Space *space = solver.getSpace();
   try
   {
      H5::H5File *file = new H5::H5File(filename, H5F_ACC_TRUNC);

      H5::Group *group = new H5::Group(file->openGroup("/")); // Get default root group

      write_solver_info(group, solver);
      DomainBox box(s);
      write_variables_info(group, box);

      H5::Group gsol = group->createGroup("SOLUTIONS");
      write_solutions(&gsol, *space);
      H5::Group gpend = group->createGroup("PENDINGS");
      write_pendings(&gpend, *space);

      delete group;
   } // end of try block
   // catch failure caused by the H5File operations
   catch (H5::FileIException error)
   {
      error.printErrorStack();
   }
   // catch failure caused by the DataSet operations
   catch (H5::DataSetIException error)
   {
      error.printErrorStack();
   }
   // catch failure caused by the DataSpace operations
   catch (H5::DataSpaceIException error)
   {
      error.printErrorStack();
   }
   // catch failure caused by the DataSpace operations
   catch (H5::DataTypeIException error)
   {
      error.printErrorStack();
   }
   catch (H5::AttributeIException error)
   {
      error.printErrorStack();
   }
}

} // namespace realpaver
