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
 * @file   ConstraintFixer.cpp
 * @brief  Rewriting of constraints
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintFixer.hpp"

namespace realpaver {

ConstraintFixer::ConstraintFixer(VarVarMapType* vvm, VarIntervalMapType* vim,
                                 const DomainBox& box)
      : vvm_(vvm),
        vim_(vim),
        box_(box),
        c_()
{}

Constraint ConstraintFixer::getConstraint() const
{
   return c_;
}

void ConstraintFixer::apply(const ArithCtrEq* c)
{
   TermFixer vl(vvm_, vim_);
   c->left().acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   c->right().acceptVisitor(vr);

   c_ = (vl.getTerm() == vr.getTerm());
}

void ConstraintFixer::apply(const ArithCtrLe* c)
{
   TermFixer vl(vvm_, vim_);
   c->left().acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   c->right().acceptVisitor(vr);

   c_ = (vl.getTerm() <= vr.getTerm());
}

void ConstraintFixer::apply(const ArithCtrLt* c)
{
   TermFixer vl(vvm_, vim_);
   c->left().acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   c->right().acceptVisitor(vr);

   c_ = (vl.getTerm() < vr.getTerm());
}

void ConstraintFixer::apply(const ArithCtrGe* c)
{
   TermFixer vl(vvm_, vim_);
   c->left().acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   c->right().acceptVisitor(vr);

   c_ = (vl.getTerm() >= vr.getTerm());
}

void ConstraintFixer::apply(const ArithCtrGt* c)
{
   TermFixer vl(vvm_, vim_);
   c->left().acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   c->right().acceptVisitor(vr);

   c_ = (vl.getTerm() > vr.getTerm());
}

void ConstraintFixer::apply(const ArithCtrIn* c)
{
   TermFixer vis(vvm_, vim_);
   c->term().acceptVisitor(vis);

   c_ = in(vis.getTerm(), c->image());
}

void ConstraintFixer::apply(const TableCtr* c)
{
   // checks the consistent assignments with respect to the variable domains
   Bitset consistent(c->nbRows());
   consistent.setAllZero();

   for (size_t i=0; i<c->nbRows(); ++i)
   {
      size_t j = 0;
      bool cons = true;
      while (cons && j < c->nbCols())
      {
         auto it = vvm_->find(c->getVar(j));
         if (it != vvm_->end())
         {
            Variable v = it->first;
            if (c->getVal(i, j).isDisjoint(box_.get(v)->intervalHull()))
               cons = false;
         }
         ++j;
      }
      if (cons) consistent.setOne(i);
   }

   // rewrites the constraint
   Constraint::SharedRep srep = std::make_shared<TableCtr>();
   TableCtr* rep = static_cast<TableCtr*>(srep.get());

   for (size_t j=0; j<c->nbCols(); ++j)
   {
      auto it = vvm_->find(c->getVar(j));
      if (it != vvm_->end())
      {
         TableCtrCol col(it->second);

         for (size_t i=0; i<c->nbRows(); ++i)
            if (consistent.get(i))
               col.addValue(c->getVal(i, j));

         rep->addCol(col);
      }
   }
   c_ = Constraint(srep);
}

void ConstraintFixer::apply(const CondCtr* c)
{
   ConstraintFixer fg(vvm_, vim_, box_);
   c->guard().acceptVisitor(fg);

   ConstraintFixer bg(vvm_, vim_, box_);
   c->body().acceptVisitor(bg);

   Constraint guard = fg.getConstraint(),
              body = bg.getConstraint();

   c_ = cond(guard, body);
}

} // namespace
