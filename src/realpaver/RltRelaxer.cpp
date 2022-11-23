///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/RltRelaxer.hpp"

namespace realpaver {

RltRelaxer::RltRelaxer(SharedDag dag) : dag_(dag), mpi_()
{
   ASSERT(dag != nullptr && dag->nbNodes() > 0,
          "No dag given as input of a RLT relaxer");
}

bool RltRelaxer::make(LPModel& lpm, const IntervalRegion& reg)
{

   if (!dag_->intervalEval(reg)) return false;

   for (size_t i=0; i<dag_->nbNodes(); ++i)
   {
      DagNode* node = dag_->node(i);
      Interval val = node->val();

      // creates a linear variable for this node
      LinVar v = lpm.makeVar(val.left(), val.right());
      mpi_.insert(std::make_pair(node->index(), v.getIndex()));

      // constrains this variable
      RltVisitor vis(&lpm, &mpi_);
      node->acceptVisitor(vis);
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

RltVisitor::RltVisitor(LPModel* lpm, std::unordered_map<size_t, size_t>* mpi)
       : lpm_(lpm),
         mpi_(mpi)
{}

size_t RltVisitor::indexLinVar(const DagNode* node) const
{
   auto it = mpi_->find(node->index());
   return it->second;
}

void RltVisitor::apply(const DagConst* node)
{
   // nothing to do
}

void RltVisitor::apply(const DagVar* node)
{
   if (node->getVar().isDiscrete())
   {
      LinVar lv = lpm_->getLinVar(indexLinVar(node));
      lv.setInteger();
   }
}

void RltVisitor::apply(const DagAdd* node)
{
   LinVar z = lpm_->getLinVar(indexLinVar(node)),
          x = lpm_->getLinVar(indexLinVar(node->left())),
          y = lpm_->getLinVar(indexLinVar(node->right()));

   // z = x + y => z - x - y = 0
   LinExpr e( {1.0, -1.0, -1.0}, {z, x, y} );
   lpm_->addCtr(0.0, e, 0.0);
}

void RltVisitor::apply(const DagSub* node)
{
   LinVar z = lpm_->getLinVar(indexLinVar(node)),
          x = lpm_->getLinVar(indexLinVar(node->left())),
          y = lpm_->getLinVar(indexLinVar(node->right()));

   // z = x - y => z - x + y = 0
   LinExpr e( {1.0, -1.0, 1.0}, {z, x, y} );
   lpm_->addCtr(0.0, e, 0.0);
}

void RltVisitor::apply(const DagMul* node)
{
   LinVar z = lpm_->getLinVar(indexLinVar(node)),
          x = lpm_->getLinVar(indexLinVar(node->left())),
          y = lpm_->getLinVar(indexLinVar(node->right()));

   // z = x*y, a <= x <= b, c <= y <= d
   double a = node->left()->val().left(),
          b = node->left()->val().right(),
          c = node->right()->val().left(),
          d = node->right()->val().right();

   bool xvar = (a != b),   // left subterm not fixed?
        yvar = (c != d);   // right subterm not fixed?

   if (xvar && yvar)
   {
      // Mc Cormick relaxation
      Interval A(a), B(b), C(c), D(d);

      // first constraint: (x-a)*(y-c) >= 0, z - c*x - a*y >= -a*c
      Interval I1 = -A*C;
      LinExpr e1( {1.0, -c, -a}, {z, x, y} );
      lpm_->addCtr(I1.left(), e1);

      // second constraint: (x-a)*(y-d) <= 0, z - d*x - a*y <= -a*d
      Interval I2 = -A*D;
      LinExpr e2( {1.0, -d, -a}, {z, x, y} );
      lpm_->addCtr(e2, I2.right());

      // third constraint: (x-b)*(y-c) <= 0, z - c*x - b*y <= -b*c
      Interval I3 = -B*C;
      LinExpr e3( {1.0, -c, -b}, {z, x, y} );
      lpm_->addCtr(e3, I3.right());

      // fourth constraint: (x-b)*(y-d) >= 0, z - d*x - b*y >= -b*d
      Interval I4 = -B*D;
      LinExpr e4( {1.0, -d, -b}, {z, x, y} );
      lpm_->addCtr(I4.left(), e4);
   }
   else if (!xvar)
   {
      // z = x*y with x fixed => z - a*y = 0
      LinExpr e( {1.0, -a}, {z, y} );
      lpm_->addCtr(0.0, e, 0.0);
   }
   else if (!yvar)
   {
      // z = x*y with y fixed => z - c*x = 0
      LinExpr e( {1.0, -c}, {z, x} );
      lpm_->addCtr(0.0, e, 0.0);      
   }
}

void RltVisitor::apply(const DagDiv* node)
{ /* TODO */ }

void RltVisitor::apply(const DagMin* node)
{ /* TODO */ }

void RltVisitor::apply(const DagMax* node)
{ /* TODO */ }

void RltVisitor::apply(const DagUsb* node)
{ /* TODO */ }

void RltVisitor::apply(const DagAbs* node)
{ /* TODO */ }

void RltVisitor::apply(const DagSgn* node)
{ /* TODO */ }

void RltVisitor::apply(const DagSqr* node)
{ /* TODO */ }

void RltVisitor::apply(const DagSqrt* node)
{ /* TODO */ }

void RltVisitor::apply(const DagPow* node)
{ /* TODO */ }

void RltVisitor::apply(const DagExp* node)
{ /* TODO */ }

void RltVisitor::apply(const DagLog* node)
{ /* TODO */ }

void RltVisitor::apply(const DagCos* node)
{ /* TODO */ }

void RltVisitor::apply(const DagSin* node)
{ /* TODO */ }

void RltVisitor::apply(const DagTan* node)
{ /* TODO */ }


} // namespace
