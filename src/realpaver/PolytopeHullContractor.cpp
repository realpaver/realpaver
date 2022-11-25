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
#include "realpaver/RltRelaxation.hpp"

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

PolytopeCreator::PolytopeCreator(SharedDag dag)
      : dag_(dag),
        scope_(dag->scope()),
        bs_(),
        mnv_(),
        lfun_()
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");

   for (size_t i=0; i<dag->nbFuns(); ++i)
      lfun_.push_back(i);

   bs_ = scope_.toBitset();
}

PolytopeCreator::PolytopeCreator(SharedDag dag, const IndexList& lfun)
      : dag_(dag),
        scope_(),
        bs_(),
        mnv_(),
        lfun_()
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");
   ASSERT(!lfun.empty(), "No list of functions in a polytope maker");

   for (size_t i : lfun)
   {
      ASSERT(i<dag_->nbFuns(), "Bad function index in a polytope maker");

      lfun_.push_back(i);
      scope_.insert(dag->fun(i)->scope());
   }

   bs_ = scope_.toBitset();
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

size_t PolytopeCreator::nodeToLinVar(DagNode* node) const
{
   auto it = mnv_.find(node->index());
   return it->second;
}

///////////////////////////////////////////////////////////////////////////////

PolytopeRLTCreator::PolytopeRLTCreator(SharedDag dag)
      : PolytopeCreator(dag)
{}

PolytopeRLTCreator::PolytopeRLTCreator(SharedDag dag, const IndexList& lfun)
      : PolytopeCreator(dag, lfun)
{}

bool PolytopeRLTCreator::make(LPModel& lpm, const IntervalRegion& reg)
{
   if (!dag_->intervalEval(reg)) return false;

   if (lfun_.size() == dag_->nbFuns())
   {
      for (size_t i=0; i<dag_->nbNodes(); ++i)
      {
         DagNode* node = dag_->node(i);
         Interval val = node->val();

         // creates a linear variable for this node
         LinVar v = lpm.makeVar(val.left(), val.right());
    
         // inserts the couple of indexes in the map
         mnv_.insert(std::make_pair(node->index(), v.getIndex()));

         // constrains this variable
         RltVisitor vis(&lpm, &mnv_);
         node->acceptVisitor(vis);
      }
   }
   else
   {
      for (size_t i : lfun_)
      {
         DagFun* f = dag_->fun(i);
         for (size_t j=0; j<f->nbNodes(); ++j)
         {
            DagNode* node = f->node(j);
            auto it = mnv_.find(node->index());
            if (it == mnv_.end())
            {
               Interval val = node->val();

               // creates a linear variable for this node
               LinVar v = lpm.makeVar(val.left(), val.right());
    
               // inserts the couple of indexes in the map
               mnv_.insert(std::make_pair(node->index(), v.getIndex()));
            }

            // constrains this variable
            RltVisitor vis(&lpm, &mnv_);
            node->acceptVisitor(vis);            
         }
      }
   }

   // takes into account the root nodes
   for (size_t i : lfun_)
   {
      DagFun* f = dag_->fun(i);
      DagNode* node = f->rootNode();

      auto it = mnv_.find(node->index());
      LinVar v = lpm.getLinVar(it->second);

      Interval x = v.getDomain() & f->getImage();
      if (x.isEmpty()) return false;
      v.setDomain(x);
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

PolytopeHullContractor::PolytopeHullContractor(SharedDag dag,
                                               PolytopeCreatorStyle style)
      : creator_(nullptr)
{
   if (style == PolytopeCreatorStyle::RLT)
      creator_ = new PolytopeRLTCreator(dag);

   else
      THROW("Polytope maker not yet implemented: " << style);
}

PolytopeHullContractor::PolytopeHullContractor(SharedDag dag,
                                               const IndexList& lfun,
                                               PolytopeCreatorStyle style)
      : creator_(nullptr)
{
   if (style == PolytopeCreatorStyle::RLT)
      creator_ = new PolytopeRLTCreator(dag, lfun);

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
   LPSolver solver;

   // linearizes the constraints
   if (!creator_->make(solver, reg)) return Proof::Maybe;

   bool first = true;
   OptimizationStatus status;

   for (auto v : creator_->scope())
   {
      Interval x = reg.get(v);
      DagNode* node = creator_->dag()->findVarNode(v.id());

      LinVar lv = solver.getLinVar(creator_->nodeToLinVar(node));
      LinExpr e({1.0}, {lv});
      solver.setObj(e);

DEBUG(solver << "\n\n");

      // reduction of the left bound
      solver.setMinimization();
      if (first) solver.optimize();
      else solver.reoptimize();

      status = solver.getStatus();


DEBUG("status " << status);
DEBUG("obj " << solver.getObjVal());


      if (status == OptimizationStatus::Infeasible) return Proof::Empty;
      if (status == OptimizationStatus::Optimal)
      {
         x &= Interval::moreThan(solver.getObjVal());
      }

      first = false;

      // reduction of the right bound
      solver.setMaximization();

DEBUG(solver << "\n\n");


      solver.reoptimize();

      status = solver.getStatus();

DEBUG("status " << status);
DEBUG("obj " << solver.getObjVal());

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
