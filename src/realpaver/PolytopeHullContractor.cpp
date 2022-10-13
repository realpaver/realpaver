///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/PolytopeHullContractor.hpp"

namespace realpaver {

PolytopeHullContractor::PolytopeHullContractor(SharedDag dag, Scope sco)
      :  dag_(dag),
         sco_(sco),
         bs_()
{
   ASSERT(dag_ != nullptr, "Null pointer in a Polytope Hull contractor");

   ASSERT(!sco.isEmpty(), "Empty scope in a Polytope Hull contractor");

   ASSERT(dag->scope().contains(sco),
          "Bad scope in a Polytope Hull contractor");

   bs_ = sco.toBitset();
}

bool PolytopeHullContractor::dependsOn(const Bitset& bs) const
{
   return bs_.overlaps(bs);
}

Scope PolytopeHullContractor::scope() const
{
   return sco_;
}

Proof PolytopeHullContractor::contract(IntervalRegion& reg)
{
   LPSolver solver;

   // linearizes the constraints
   dag_->eval(reg);
   dag_->linearize(solver);

   bool first = true;
   OptimizationStatus status;

   for (auto v : sco_)
   {
      Interval x = reg.get(v);
      DagNode* node = dag_->findVarNode(v.id());

      LinVar lv = solver.getLinVar(node->indexLinVar());
      LinExpr e({1.0}, {lv});
      solver.setObj(e);

      // reduction of the left bound
      solver.setMinimization();
      if (first) solver.optimize();
      else       solver.reoptimize();
      status = solver.getStatus();

      if (status == OptimizationStatus::Infeasible) return Proof::Empty;
      if (status == OptimizationStatus::Optimal)
      {
         x &= Interval::moreThan(solver.getObjVal());
      }

      first = false;

      // reduction of the right bound
      solver.setMaximization();
      solver.reoptimize();
      status = solver.getStatus();

      if (status == OptimizationStatus::Infeasible) return Proof::Empty;
      if (status == OptimizationStatus::Optimal)
      {
         x &= Interval::lessThan(solver.getObjVal());
      }

      reg.set(v, x);
   }

   return Proof::Maybe;
}

void PolytopeHullContractor::print(std::ostream& os) const
{
   os << "Polytope Hull contractor";
}

} // namespace
