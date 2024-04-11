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
 * @file   ContractorPolytope.cpp
 * @brief  Polytope Hull contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorPolytope.hpp"
#include "realpaver/IntervalVector.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/RLTRelaxation.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, const PolytopeStyle& style)
{
   switch(style)
   {
      case PolytopeStyle::Affine: return os << "Affine maker";
      case PolytopeStyle::RLT:    return os << "RLT maker";
      case PolytopeStyle::Taylor: return os << "Taylor maker";
      default:                           os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

PolytopeCreator::PolytopeCreator(SharedDag dag)
      : dag_(dag),
        scop_(dag->scope()),
        mvv_(),
        lfun_(),
        eqtol_(Param::GetDblParam("RELAXATION_EQ_TOL"))
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");

   for (size_t i=0; i<dag->nbFuns(); ++i)
      lfun_.push_back(i);

   scop_ = ScopeBank::getInstance()->insertScope(scop_);
}

PolytopeCreator::PolytopeCreator(SharedDag dag, const IndexList& lfun)
      : dag_(dag),
        scop_(),
        lfun_(),
        eqtol_(Param::GetDblParam("RELAXATION_TOL"))
{
   ASSERT(dag_ != nullptr, "No dag in a polytope maker");
   ASSERT(!lfun.empty(), "No list of functions in a polytope maker");

   for (size_t i : lfun)
   {
      ASSERT(i<dag_->nbFuns(), "Bad function index in a polytope maker");

      lfun_.push_back(i);
      scop_.insert(dag->fun(i)->scope());
   }

   scop_ = ScopeBank::getInstance()->insertScope(scop_);
}

PolytopeCreator::~PolytopeCreator()
{}

SharedDag PolytopeCreator::dag() const
{
   return dag_;
}

Scope PolytopeCreator::scope() const
{
   return scop_;
}

size_t PolytopeCreator::linVarIndex(Variable v) const
{
   auto it = mvv_.find(v.id());
   return it->second;
}

double PolytopeCreator::getRelaxEqTol() const
{
   return eqtol_;
}

void PolytopeCreator::setRelaxEqTol(double tol)
{
   ASSERT(tol >= 0.0,
          "The relaxation tolerance must be positive: " << tol << ".");

   eqtol_ = tol;
}

/*----------------------------------------------------------------------------*/

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

bool PolytopeRLTCreator::make(LPModel& lpm, const IntervalBox& B)
{
   if (!dag_->intervalEval(B)) return false;

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

      Interval img = f->getImage();      
      if (img.isSingleton())
         img += Interval(-eqtol_, eqtol_);

      Interval x = v.getDomain() & img;
      if (x.isEmpty()) return false;
      v.setDomain(x);
   }

   return true;
}

/*----------------------------------------------------------------------------*/

PolytopeTaylorCreator::PolytopeTaylorCreator(SharedDag dag)
      : PolytopeCreator(dag),
        corner_()
{
   // current strategy: corner of left bounds (each bit = 0)
   corner_ = Bitset(scope().size());
   corner_.setAllZero();
}

PolytopeTaylorCreator::PolytopeTaylorCreator(SharedDag dag,
                                             const IndexList& lfun)
      : PolytopeCreator(dag, lfun),
        corner_()
{
   // current strategy: corner of left bounds (each bit = 0)
   corner_ = Bitset(scope().size());
   corner_.setAllZero();
}

bool PolytopeTaylorCreator::make(LPModel& lpm, const IntervalBox& B)
{
   // creates the linear variables
   for (const auto& v : scop_)
   {
      Interval val = B.get(v);
      LinVar lv = lpm.makeVar(val.left(), val.right());
      lv.setName(v.getName());
      mvv_.insert(std::make_pair(v.id(), lv.getIndex()));
   }

   // makes the opposite corners
   RealPoint c1(scop_), c2(scop_);
   for (const auto& v : scop_)
   {
      Interval dom = B.get(v);
      if (corner_.get(scop_.index(v)))
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

   // evaluates the functions at both corners
   IntervalVector fc1(lfun_.size()),
                  fc2(lfun_.size());
   for (size_t i : lfun_)
   {
      DagFun* f = dag_->fun(i);
      Interval x1 = f->intervalEval(c1),
               x2 = f->intervalEval(c2);

      if (x1.isEmpty() || x2.isEmpty()) return false;

      fc1.set(i, x1);
      fc2.set(i, x2);
   }

   // interval evaluation on the given box, used to calculate
   // the derivatives thereafter
   if (!dag_->intervalEval(B)) return false;

   // generates the constraints
   for (size_t i : lfun_)
   {
      DagFun* f = dag_->fun(i);
      Interval img = f->getImage();
      if (img.isSingleton())
         img += Interval(-eqtol_, eqtol_);

      // differentiates the function
      f->intervalDiff();

      // lower bounding constraints
      // assume that the right bound of the image of the function is finite
      // we generate two linear constraints, one per corner
      // the first one has the form lo1 <= u1 where lo1 is the non constant
      // part of the constraint and u1 is the constant part
      // the second one lo2 <= u2 is built similarly
      if (!Double::isInf(img.right()))
      {
         Interval u1 = img.right() - fc1.get(i),  // U - f(c1)
                  u2 = img.right() - fc2.get(i);  // U - f(c2)

         LinExpr lo1, lo2;

         for (const auto& v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));
            Interval z = f->intervalDeriv(v);
            if (z.isEmpty() || z.isInf()) return false;

            if (corner_.get(scop_.index(v)))
            {  // right bound used for this variable (bit = 1)

               // first corner => right bound of the derivative
               lo1.addTerm(z.right(), lv);
               u1 += z.right() * Interval(c1.get(v));

               // second (opposite) corner => left bound of the derivative
               lo2.addTerm(z.left(), lv);
               u2 += z.left() * Interval(c2.get(v));
            }
            else
            {  // left bound used for this variable (bit = 0)

               // first corner => left bound of the derivative
               lo1.addTerm(z.left(), lv);
               u1 += z.left() * Interval(c1.get(v));

               // second (opposite) corner => right bound of the derivative
               lo2.addTerm(z.right(), lv);
               u2 += z.right() * Interval(c2.get(v));
            }
         }

         lpm.addCtr(lo1, u1.right());
         lpm.addCtr(lo2, u2.right());
      }

      // upper bounding constraints
      // assume that the left bound of the image of the function is finite
      // we generate two linear constraints, one per corner
      // the first one has the form up1 >= l1 where up1 is the non constant
      // part of the constraint and l1 is the constant part
      // the second one up2 >= l2 is built similarly
      if (!Double::isInf(img.left()))
      {
         Interval l1 = img.left() - fc1.get(i),
                  l2 = img.left() - fc2.get(i);

         LinExpr up1, up2;

         for (const auto& v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));
            Interval z = f->intervalDeriv(v);

            if (corner_.get(scop_.index(v)))
            {  // right bound used for this variable (bit = 1)

               // first corner => left bound of the derivative
               up1.addTerm(z.left(), lv);
               l1 += z.left() * Interval(c1.get(v));

               // second (opposite) corner => right bound of the derivative
               up2.addTerm(z.right(), lv);
               l2 += z.right() * Interval(c2.get(v));
            }
            else
            {  // left bound used for this variable (bit = 1)

               // first corner => right bound of the derivative
               up1.addTerm(z.right(), lv);
               l1 += z.right() * Interval(c1.get(v));

               // second (opposite) corner => left bound of the derivative
               up2.addTerm(z.left(), lv);
               l2 += z.left() * Interval(c2.get(v));               
            }
         }

         lpm.addCtr(l1.left(), up1);
         lpm.addCtr(l2.left(), up2);
      }
   }
   return true;
}

/*----------------------------------------------------------------------------*/

ContractorPolytope::ContractorPolytope(SharedDag dag,
                                       PolytopeStyle style)
      : creator_(nullptr)
{
   if (style == PolytopeStyle::RLT)
      creator_ = new PolytopeRLTCreator(dag);

   else if (style == PolytopeStyle::Taylor)
      creator_ = new PolytopeTaylorCreator(dag);

   else
      THROW("Polytope maker not yet implemented: " << style);
}

ContractorPolytope::ContractorPolytope(SharedDag dag, const IndexList& lfun,
                                       PolytopeStyle style)
      : creator_(nullptr)
{
   if (style == PolytopeStyle::RLT)
      creator_ = new PolytopeRLTCreator(dag, lfun);

   else if (style == PolytopeStyle::Taylor)
      creator_ = new PolytopeTaylorCreator(dag, lfun);

   else
      THROW("Polytope maker not yet implemented: " << style);
}

ContractorPolytope::~ContractorPolytope()
{
   if (creator_ != nullptr) delete creator_;
}

Scope ContractorPolytope::scope() const
{
   return creator_->scope();
}

Proof ContractorPolytope::contract(IntervalBox& B)
{
   Proof proof = contractImpl(B);
   return proof;
}

// TODO
#include <iomanip>

Proof ContractorPolytope::contractImpl(IntervalBox& B)
{
   LPSolver solver;

   // linearizes the constraints
   if (!creator_->make(solver, B)) return Proof::Maybe;

   // first is true if a call to solver.optimize() is required, false false
   // after a successfull optimization, then the next call to the solver can use
   // reoptimize() after just changing the objective function
   bool first = true;
   OptimizationStatus status;

   for (const auto& v : creator_->scope())
   {
      Interval x = B.get(v);
      LinVar lv = solver.getLinVar(creator_->linVarIndex(v));
      LinExpr e({1.0}, {lv});
      solver.setObj(e);

      // reduction of the left bound
      solver.setMinimization();

      status = solver.getStatus();
      if (status == OptimizationStatus::Optimal)
      {
         x &= Interval::moreThan(solver.getSafeObjVal());
         if (x.isEmpty()) return Proof::Empty;   
         first = false;
      }
      else if (status == OptimizationStatus::Infeasible)
      {
         if (solver.isSafeInfeasible()) return Proof::Empty;
         first = true;
      }
      else
      {
         first = true;
      }

      // reduction of the right bound
      solver.setMaximization();
      if (first) solver.optimize();
      else solver.reoptimize();

      status = solver.getStatus();
      if (status == OptimizationStatus::Optimal)
      {
         x &= Interval::lessThan(solver.getSafeObjVal());
         if (x.isEmpty()) return Proof::Empty;
         first = false;
      }
      else if (status == OptimizationStatus::Infeasible)
      {
         if (solver.isSafeInfeasible()) return Proof::Empty;
         first = true;
      }
      else
      {
         first = true;
      }

      B.set(v, x);
   }

   return Proof::Maybe;
}

void ContractorPolytope::print(std::ostream& os) const
{
   os << "Polytope Hull contractor";
}

double ContractorPolytope::getRelaxEqTol() const
{
   return creator_->getRelaxEqTol();
}

void ContractorPolytope::setRelaxEqTol(double tol)
{
   creator_->setRelaxEqTol(tol);
}

} // namespace
