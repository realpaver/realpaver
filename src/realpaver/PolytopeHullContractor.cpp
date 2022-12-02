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
        mvv_(),
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

size_t PolytopeCreator::linVarIndex(Variable v) const
{
   auto it = mvv_.find(v.id());
   return it->second;
}

///////////////////////////////////////////////////////////////////////////////

PolytopeRLTCreator::PolytopeRLTCreator(SharedDag dag)
      : PolytopeCreator(dag),
        mnv_()
{}

PolytopeRLTCreator::PolytopeRLTCreator(SharedDag dag, const IndexList& lfun)
      : PolytopeCreator(dag, lfun)
{}

size_t PolytopeRLTCreator::nodeToLinVar(DagNode* node) const
{
   auto it = mnv_.find(node->index());
   return it->second;
}

void PolytopeRLTCreator::createLinVar(LPModel& lpm, DagNode* node)
{
   Interval val = node->val();

   // creates a linear variable for this node
   LinVar v = lpm.makeVar(val.left(), val.right());
    
   // inserts the couple (node index, lin var index)
   mnv_.insert(std::make_pair(node->index(), v.getIndex()));

   // for a variable node, inserts the couple (var index, lin var index)
   DagVar* vnode = dynamic_cast<DagVar*>(node);
   if (vnode != nullptr)
      mvv_.insert(std::make_pair(vnode->getVar().id(), v.getIndex()));
}

bool PolytopeRLTCreator::make(LPModel& lpm, const IntervalRegion& reg)
{
   if (!dag_->intervalEval(reg)) return false;

   if (lfun_.size() == dag_->nbFuns())
   {
      for (size_t i=0; i<dag_->nbNodes(); ++i)
      {
         DagNode* node = dag_->node(i);

         // creates the linear variable
         createLinVar(lpm, node);

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
               // creates the linear variable
               createLinVar(lpm, dag_->node(i));

               // constrains this variable
               RltVisitor vis(&lpm, &mnv_);
               node->acceptVisitor(vis);            
            }
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

PolytopeTaylorCreator::PolytopeTaylorCreator(SharedDag dag)
      : PolytopeCreator(dag),
        corner_()
{
   corner_ = Bitset(1 + scope().maxIndex(), 0);
}

PolytopeTaylorCreator::PolytopeTaylorCreator(SharedDag dag,
                                             const IndexList& lfun)
      : PolytopeCreator(dag, lfun),
        corner_()
{
   corner_ = Bitset(1 + scope().maxIndex(), 0);
}

bool PolytopeTaylorCreator::make(LPModel& lpm, const IntervalRegion& reg)
{
   if (!dag_->intervalEval(reg)) return false;
   Scope sco = scope();

   // creates the linear variables
   for (auto v : sco)
   {
      Interval val = reg.get(v);
      LinVar lv = lpm.makeVar(val.left(), val.right());
      mvv_.insert(std::make_pair(v.id(), lv.getIndex()));
   }

   // makes the opposite corners
   // current strategy: coner of left bounds and corner of right bounds
   RealPoint c1(sco), c2(sco);
   for (auto v : sco)
   {
      Interval dom = reg.get(v);
      if (corner_.get(v.id()))
      {
         c1.set(v, dom.right());
         c2.set(v, dom.left());
      }

      else
      {
         c1.set(v, dom.left());
         c2.set(v, dom.right());
      }
   }

   // generates the constraints
   for (size_t i : lfun_)
   {
      DagFun* f = dag_->fun(i);
      Interval img = f->getImage();

      // differentiates the function
      f->intervalDiff();

      // evaluation at corners
      Interval fc1 = f->intervalEval(c1),
               fc2 = f->intervalEval(c2);

      // lower bounding functions
      if (!Double::isInf(img.right()))
      {
         Interval u1 = img.right() - fc1,
                  u2 = img.right() - fc2;

         LinExpr lo1, lo2;

         for (auto v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));
            Interval z = f->intervalDeriv(v);

DEBUG("df / " << v.getName() << " : " << z);

            if (corner_.get(v.id()))
            {
               lo1.addTerm(z.right(), lv);
               u1 += z.right() * Interval(c1.get(v));

               lo2.addTerm(z.left(), lv);
               u2 += z.left() * Interval(c2.get(v));
            }
            else
            {
               lo1.addTerm(z.left(), lv);
               u1 += z.left() * Interval(c1.get(v));

               lo2.addTerm(z.right(), lv);
               u2 += z.right() * Interval(c2.get(v));
            }
         }

         lpm.addCtr(lo1, u1.right());
         lpm.addCtr(lo2, u2.right());
      }

      // upper bounding functions
      if (!Double::isInf(img.left()))
      {
         Interval u1 = -fc1;
      }

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

   else if (style == PolytopeCreatorStyle::Taylor)
      creator_ = new PolytopeTaylorCreator(dag);

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

   else if (style == PolytopeCreatorStyle::Taylor)
      creator_ = new PolytopeTaylorCreator(dag, lfun);

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

DEBUG("PolytopeHullContractor");

   // linearizes the constraints
   if (!creator_->make(solver, reg)) return Proof::Maybe;

   bool first = true;
   OptimizationStatus status;

   for (auto v : creator_->scope())
   {
      Interval x = reg.get(v);
      LinVar lv = solver.getLinVar(creator_->linVarIndex(v));
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
