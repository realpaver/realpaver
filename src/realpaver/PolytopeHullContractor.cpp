///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/PolytopeHullContractor.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, const PolytopeCreatorStyle& style)
{
   switch(style)
   {
      case PolytopeCreatorStyle::Affine: return os << "affine maker";
      case PolytopeCreatorStyle::RLT:    return os << "rlt maker";
      case PolytopeCreatorStyle::Taylor: return os << "taylor maker";
      default:                           os.setstate(std::ios::failbit);
   }
   return os;
}

///////////////////////////////////////////////////////////////////////////////

PolytopeCreator::PolytopeCreator(SharedDag dag, Scope scope)
      : dag_(dag),
        scope_(scope),
        bs_()
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");
   ASSERT(!scope.isEmpty(), "Empty scope in a polytope maker");
   ASSERT(dag->scope().contains(scope), "Bad scope in a polytope maker");

   bs_ = scope.toBitset();
}

PolytopeCreator::~PolytopeCreator()
{}

SharedDag PolytopeCreator::dag() const
{
   return dag_;
}

Scope PolytopeCreator::scope() const
{
   return scope_;
}

bool PolytopeCreator::dependsOn(const Bitset& bs) const
{
   return bs_.overlaps(bs);
}

///////////////////////////////////////////////////////////////////////////////

PolytopeRLTCreator::PolytopeRLTCreator(SharedDag dag, Scope scope)
      : PolytopeCreator(dag, scope)
{}

bool PolytopeRLTCreator::make(LPModel& lpm, const IntervalRegion& reg)
{
   if (!dag_->intervalEval(reg)) return false;

   dag_->linearize(lpm);
   return true;
}

///////////////////////////////////////////////////////////////////////////////

PolytopeHullContractor::PolytopeHullContractor(SharedDag dag, Scope sco,
                                               PolytopeCreatorStyle style)
      : creator_(nullptr)
{
   if (style == PolytopeCreatorStyle::RLT)
      creator_ = new PolytopeRLTCreator(dag, sco);

   else
      THROW("Polytope maker not yet implemented: " << style);
}

PolytopeHullContractor::~PolytopeHullContractor()
{
   if (creator_ != nullptr) delete creator_;
}

bool PolytopeHullContractor::dependsOn(const Bitset& bs) const
{
   return creator_->dependsOn(bs);
}

Scope PolytopeHullContractor::scope() const
{
   return creator_->scope();
}

Proof PolytopeHullContractor::contract(IntervalRegion& reg)
{
   LOG_LOW("Polytope hull contractor on: " << reg);

   LPSolver solver;

   // linearizes the constraints
   if (!creator_->make(solver, reg)) return Proof::Maybe;

   bool first = true;
   OptimizationStatus status;

   for (auto v : creator_->scope())
   {
      Interval x = reg.get(v);
      DagNode* node = creator_->dag()->findVarNode(v.id());

      LinVar lv = solver.getLinVar(node->indexLinVar());
      LinExpr e({1.0}, {lv});
      solver.setObj(e);

      // reduction of the left bound
      solver.setMinimization();
      if (first) solver.optimize();
      else solver.reoptimize();

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

   LOG_LOW("Reduced region: " << reg);
   return Proof::Maybe;
}

void PolytopeHullContractor::print(std::ostream& os) const
{
   os << "Polytope Hull contractor";
}

} // namespace
