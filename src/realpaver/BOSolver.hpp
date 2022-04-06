///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BO_SOLVER_HPP
#define REALPAVER_BO_SOLVER_HPP

#include "realpaver/BOLocalSolver.hpp"
#include "realpaver/BOModel.hpp"
#include "realpaver/BOSpace.hpp"
#include "realpaver/BOSplit.hpp"
#include "realpaver/Contractor.hpp"
#include "realpaver/ContractorPool.hpp"
#include "realpaver/LPSolver.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Preprocessor.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Timer.hpp"

namespace realpaver {

class BOSolver {
public:
   /// Creates a solver
   /// @param problem a bound optimization problem
   BOSolver(Problem& problem);

   /// Destructor
   ~BOSolver();

   /// Optimization method
   /// @return true if an optimal solution is found
   bool optimize();

   /// @return the status of the last optimization stage
   OptimizationStatus getStatus() const;

   /// @return the enclosure of the optimum after the optimization process
   Interval getObjEnclosure() const;

   /// @return the best point found by the optimization process
   RealPoint getBestSolution() const;

   /// @return the preprocessing time in seconds
   double getPreprocessingTime() const;

   /// @return the solving time in seconds
   double getSolvingTime() const;

   /// @return the number of nodes processed
   int getNbNodes() const;

   /// @return the number of nodes that remains in the optimization space
   int getNbPendingNodes() const;

   /// Assigns the parameters in this
   /// @param prm parameters
   void setParam(const Param& prm);

   ///@{
   /// Management of parameters
   /// Each method is delegated to the Param instance enclosed in this.
   void loadParam(const std::string& filename);
   int getIntParam(const string& name);
   void setIntParam(const string& name, int val);
   double getDblParam(const string& name);
   void setDblParam(const string& name, double val);
   Tolerance getTolParam(const string& name);
   void setTolParam(const string& name, const Tolerance& val);
   std::string getStrParam(const string& name);
   void setStrParam(const string& name, const std::string& val);
   void printParam(std::ostream& os);
   ///@}

private:
   Problem problem_;             // initial problem
   Problem preprob_;             // problem resulting from preprocessing
   Problem solprob_;             // problem resulting from presolving
   Param param_;                 // parameters

   BOModel* model_;              // solving model
   BOLocalSolver* localSolver_;  // local optimization
   BOSplit* split_;              // splitting strategy
   SharedContractor contractor_; // contraction strategy

   LPSolver* lpsolver_;

   SharedIntervalRegion init_;   // initial region
   OptimizationStatus status_;   // status resulting from an optimization
   IntervalRegion sol_;          // best solution found (incumbent solution)
   Interval objval_;             // enclosure of the global minimum
   double upper_;                // refined upper bound of the global minimum
   int nbnodes_;                 // number of nodes processed (BB algorithm)
   int nbpending_;               // number of pending nodes (BB algorithm)
   Tolerance otol_;              // tolerance on the global optimum

   typedef Preprocessor::VarVarMapType VarVarMapType;

   // Maps every variable of the preprocessed problem to the associated
   // variable in the initial problem
   VarVarMapType vmap21_;

   // Maps every variable of the presolved problem to the associated
   // variable in the initial problem
   VarVarMapType vmap31_;

   Timer ptimer_;      // timer for the preprocessing phase
   Timer stimer_;      // timer for the solving phase

   // Methods that make the solving components
   void makeLocalSolver();
   void makeSplit();
   void makeContractor();
   void makeHC4();
   void makeMaxCIDHC4();

   // Solving methods
   bool preprocess();
   bool presolve();
   void calculateLowerInit(SharedBONode& node);
   void calculateUpperInit(SharedBONode& node);
   void calculateLower(SharedBONode& node);
   void calculateUpper(SharedBONode& node);
   void saveIncumbent(const RealPoint& pt);
   void solve();
   void branchAndBound();
   bool bbStep(BOSpace& space, BOSpace& sol);
   void findInitialBounds(SharedBONode& node);
   Proof reducePolytope(SharedBONode& node);
};

} // namespace

#endif
