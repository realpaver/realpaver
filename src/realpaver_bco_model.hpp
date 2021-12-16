// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_MODEL_BCO_HPP
#define REALPAVER_MODEL_BCO_HPP

#include "realpaver_bco_dag.hpp"
#include "realpaver_bco_local.hpp"
#include "realpaver_bco_space.hpp"
#include "realpaver_param.hpp"
#include "realpaver_problem.hpp"
#include "realpaver_timer.hpp"

namespace realpaver {

/*****************************************************************************
 * Result of bound constrained optimization.
 *****************************************************************************/
class BcoResult {
public:
   BcoResult();
   BcoResult(const BcoResult& res);
   BcoResult& operator=(const BcoResult& res);
   ~BcoResult();

   // management of the certificate of proof
   Proof getProof() const;
   void setProof(Proof proof);

   // management of the optimum
   Interval getOptimum() const;
   void setOptimum(const Interval& x);

   // management of the creation time
   size_t getCTime() const;
   void addCTime(size_t t);

   // management of the preprocessing time
   size_t getPTime() const;
   void addPTime(size_t t);

   // management of the solving time
   size_t getSTime() const;
   void addSTime(size_t t);

   // management of the box enclosed / optimizer
   Box* getBox() const;
   void setBox(const Box& B);

private:
   Proof proof_;
   Box* xstar_;
   Interval fstar_;
   size_t ctime_;
   size_t ptime_;
   size_t stime_;
};

/*****************************************************************************
 * Class of bound constrained optimization models.
 *****************************************************************************/
class BcoModel {
public:
   // constructor
   // inserts a variable z_ in P representing the objective function
   // creates a DAG from P
   BcoModel(Problem& P);

   // destructor
   ~BcoModel();

   // copy protection
   BcoModel(const BcoModel&) = delete;
   BcoModel& operator=(const BcoModel&) = delete;

   // makes contractors based on HC4
   void makeHc4Propagator();

   // makes contractors based on HC4
   void makeBc4Propagator();

   // preprocessing
   // tries to fix the variables v such that df / dv is constant
   BcoResult preprocess(const Param& param = Param());

   // solving
   BcoResult solve(const Param& param = Param());

   // returns the variable representing the objective function
   Variable objVar() const;

   // number of fixed variables after preprocessing
   size_t nbFixedVars() const;

   // initial box
   SharedBox initialBox() const;

   // returns the dag
   BcoDag* getBcoDag() const;

   // returns the number of decision variables in the objective function
   size_t dim() const;

private:
   Problem* prob_;   // problem
   BcoDag* bdag_;    // specific dag

   double L_;        // lower bound of the global optimum
   double U_;        // upper bound of the global optimum

   size_t nbFixed_;  // number of fixed variables after preprocessing

   // timer used to measure the creation time
   Timer ctimer_;

   // timer used to measure the preprocessing time
   Timer ptimer_;

   // timer used to measure the solving time
   Timer stimer_;
};

inline size_t BcoModel::nbFixedVars() const
{
   return nbFixed_;
}

inline SharedBox BcoModel::initialBox() const
{
   return bdag_->initialBox();;
}

inline Variable BcoModel::objVar() const
{
   return bdag_->objVar();
}

inline BcoDag* BcoModel::getBcoDag() const
{
   return bdag_;
}

inline void BcoModel::makeHc4Propagator()
{
   ctimer_.start();
   bdag_->makeHc4Propagator();
   ctimer_.stop();
}

inline void BcoModel::makeBc4Propagator()
{
   ctimer_.start();
   bdag_->makeBc4Propagator();
   ctimer_.stop();
}

inline size_t BcoModel::dim() const
{
   return bdag_->dim();
}

} // namespace

#endif
