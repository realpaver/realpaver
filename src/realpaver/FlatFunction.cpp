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
 * @file   FlatFunction.cpp
 * @brief  Function with fast hc4Revise
 * @author Laurent Granvilliers
 * @date   2024-6-11
*/

#include "realpaver/Dag.hpp"
#include "realpaver/FlatFunction.hpp"

namespace realpaver {

FlatFunction::FlatFunction(const Term& t, const Interval& img)
   : scop_(),
     img_(img)
{
   t.makeScope(scop_);
   capa_ = t.nbNodes();
   symb_ = new NodeSymbol[capa_];
   arg_ = new size_t*[capa_];
   itv_ = new Interval[capa_];
   nb_ = 0;

   FlatFunctionCreator creator(this);
   t.acceptVisitor(creator);
}

FlatFunction::~FlatFunction()
{
   // TODO
}

size_t FlatFunction::insertCst(const Interval& val)
{
   size_t i = nb_++;
   symb_[i] = NodeSymbol::Cst;
   itv_[i] = val;
   arg_[i] = nullptr;
   return i;
}

size_t FlatFunction::insertVar(const Variable& v)
{
   size_t i = nb_++;
   symb_[i] = NodeSymbol::Var;
   arg_[i] = new size_t[1];
   arg_[i][0] = v.id();
   return i;
}

size_t FlatFunction::insertUnary(NodeSymbol symb, size_t ic)
{
   size_t i = nb_++;
   symb_[i] = symb;
   arg_[i] = new size_t[1];
   arg_[i][0] = ic;
   return i;
}

size_t FlatFunction::insertBinary(NodeSymbol symb, size_t il, size_t ir)
{
   size_t i = nb_++;
   symb_[i] = symb;
   arg_[i] = new size_t[2];
   arg_[i][0] = il;
   arg_[i][1] = ir;
   return i;   
}

Interval FlatFunction::eval(const IntervalBox& B) const
{
   const IntervalVector& V = static_cast<const IntervalVector&>(B);

   for (size_t i=0; i<nb_; ++i)
   {
      switch(symb_[i])
      {
         case NodeSymbol::Cst:
            // nothing    but TODO
            break;

         case NodeSymbol::Var:
            //itv_[i] = B.get(arg_[i][0]);
            itv_[i] = V[arg_[i][0]];
            break;

         case NodeSymbol::Add:
            itv_[i] = itv_[arg_[i][0]] + itv_[arg_[i][1]];
            break;

         case NodeSymbol::Sub:
            break;

         case NodeSymbol::Mul:
            break;

         case NodeSymbol::Div:
            break;

         case NodeSymbol::Min:
            break;

         case NodeSymbol::Max:
            break;

         case NodeSymbol::Usb:
            break;

         case NodeSymbol::Abs:
            break;

         case NodeSymbol::Sgn:
            break;

         case NodeSymbol::Sqr:
            itv_[i] = sqr(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sqrt:
            break;

         case NodeSymbol::Pow:
            break;

         case NodeSymbol::Exp:
            break;

         case NodeSymbol::Log:
            break;

         case NodeSymbol::Cos:
            break;

         case NodeSymbol::Sin:
            break;

         case NodeSymbol::Tan:
            break;

         case NodeSymbol::Cosh:
            break;

         case NodeSymbol::Sinh:
            break;

         case NodeSymbol::Tanh:
            break;
      }
   }

   return itv_[nb_-1];
}

/*----------------------------------------------------------------------------*/

FlatFunctionCreator::FlatFunctionCreator(FlatFunction* f)
      : f_(f)
{}

void FlatFunctionCreator::apply(const TermCst* t)
{
   idx_ = f_->insertCst(t->getVal());
}
   
void FlatFunctionCreator::apply(const TermVar* t)
{
   idx_ = f_->insertVar(t->var());
}

void FlatFunctionCreator::apply(const TermAdd* t)
{
   FlatFunctionCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunctionCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);
}
   
void FlatFunctionCreator::apply(const TermSub* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermMul* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermDiv* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermMin* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermMax* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermUsb* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermAbs* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermSgn* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermSqr* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermSqrt* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermPow* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermExp* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermLog* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermCos* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermSin* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermTan* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermCosh* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermSinh* t)
{
   
}
   
void FlatFunctionCreator::apply(const TermTanh* t)
{
   
}
   
} // namespace
