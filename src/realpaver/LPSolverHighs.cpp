///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/LPSolverHighs.hpp"

namespace realpaver {

LPSolver::LPSolver() :
   simplex_(nullptr)
{}

LPSolver::~LPSolver()
{
   if (simplex_!= nullptr) delete simplex_;
}

void LPSolver::makeVars()
{
   int n = getNbLinVars();

   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      int j = v.getIndex();
      // simplex_->addVar(v.getLB(),v.getUB());
      simplex_->addVar(-simplex_->getInfinity(),simplex_->getInfinity());

      if (!v.isContinuous())
        simplex_->changeColIntegrality(j, HighsVarType::kInteger);
   }
}

void LPSolver::makeCtrs()
{
   int m = getNbLinCtrs();
   for (int i=0; i<m; ++i)
   {
      LinCtr c = getLinCtr(i);
      LinExpr e = c.getExpr();
      simplex_->addRow(c.getLB(),c.getUB(),e.getNbTerms(),e.getIndexVars(),e.getCoefs());
   }
   
   // RAPHAEL
   // bound constraints
   int n = getNbLinVars();
   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      LinExpr e = {{1.0}, {v}};
      simplex_->addRow(v.getLB(), v.getUB(),e.getNbTerms(), e.getIndexVars(), e.getCoefs());
   }
}

void LPSolver::makeObj()
{
   LinExpr obj = getObjExpr();
   int n = obj.getNbTerms();

   for (int i=0; i<n; ++i){
      LinVar v = getLinVar(i);
      int j = v.getIndex();
      simplex_->changeColCost(j,obj.getCoef(i));
   }

   ObjSense sense = isMinimization() ? ObjSense::kMinimize : ObjSense::kMaximize;
   simplex_->changeObjectiveSense(sense);
}

void LPSolver::makeHighsSimplex()
{
   if (simplex_ != nullptr) delete simplex_;
   simplex_ = new Highs();

   makeVars();
   makeCtrs();
   makeObj();
}

bool LPSolver::run()
{
   HighsStatus return_status;
   return_status = simplex_->setOptionValue("time_limit",getMaxSeconds());
   assert(return_status==HighsStatus::kOk);
   const HighsInt max_iter = getMaxIter();
   return_status = simplex_->setOptionValue("simplex_iteration_limit",max_iter);
   assert(return_status==HighsStatus::kOk);
   return_status = simplex_->setOptionValue("log_to_console",false);
   assert(return_status==HighsStatus::kOk);
   return_status = simplex_->setOptionValue("output_flag",false);
   assert(return_status==HighsStatus::kOk);

   return_status = simplex_->presolve();
   assert(return_status==HighsStatus::kOk);

   return_status = simplex_->run();
   assert(return_status==HighsStatus::kOk);
   

   const HighsModelStatus& model_status = simplex_->getModelStatus();
   if (model_status==HighsModelStatus::kOptimal)
   {
      setObjVal(simplex_->getInfo().objective_function_value);

      int n = getNbLinVars();
      int m = getNbLinCtrs();

      const HighsSolution& solution = simplex_->getSolution();
      for (int i=0; i<n; ++i)
      {
         LinVar v = getLinVar(i);
         v.setObjVal(solution.col_value[i]);
      }

      // RAPHAEL
      // assigns the values of the dual variables (multipliers)
      // first for the primal constraints
      for (int i=0; i<m; ++i)
      {
         LinCtr c = getLinCtr(i);
         c.setMultiplier(solution.row_dual[i]);
      }
      // second for the primal bound constraints
      for (int i=0; i<n; ++i)
      {
         LinVar v = getLinVar(i);
         v.setMultiplier(solution.row_dual[m+i]);
      }

      setStatus(OptimizationStatus::Optimal);
      return true;
   }
   else
   {
      setStatus(OptimizationStatus::Other);

      if (model_status==HighsModelStatus::kInfeasible ||
            model_status==HighsModelStatus::kUnboundedOrInfeasible ||
            model_status==HighsModelStatus::kUnbounded)
         setStatus(OptimizationStatus::Infeasible);

      if (model_status==HighsModelStatus::kTimeLimit)
         setStatus(OptimizationStatus::StopOnIterLimit);

      if (model_status==HighsModelStatus::kIterationLimit)
         setStatus(OptimizationStatus::StopOnTimeLimit);

      return false;
   }
}

bool LPSolver::optimize()
{
   makeHighsSimplex();
   return run();
}

bool LPSolver::reoptimize()
{
   makeObj();
   return run();
}

} // namespace
