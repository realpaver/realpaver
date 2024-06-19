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
     img_(img),
     cst_(),
     var_()
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
   delete[] symb_;
   delete[] itv_;

   for (size_t i=0; i<nb_; ++i)
      if (arg_[i] != nullptr)
         delete[] arg_[i];

   delete[] arg_;
}

size_t FlatFunction::insertCst(const Interval& val)
{
   size_t i = nb_++;
   symb_[i] = NodeSymbol::Cst;
   arg_[i] = new size_t[1];
   arg_[i][0] = cst_.size();
   cst_.push_back(val);
   return i;
}

size_t FlatFunction::insertVar(const Variable& v)
{
   size_t i = nb_++;
   symb_[i] = NodeSymbol::Var;
   arg_[i] = new size_t[2];
   arg_[i][0] = v.id();
   arg_[i][1] = var_.size();
   var_.push_back(v);
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

size_t FlatFunction::insertPow(NodeSymbol symb, size_t ic, int e)
{
   size_t i = nb_++;
   symb_[i] = symb;
   arg_[i] = new size_t[2];
   arg_[i][0] = ic;
   arg_[i][1] = (size_t)e;
   return i;   
}

Interval FlatFunction::eval(const IntervalVector& V) const
{
   for (size_t i=0; i<nb_; ++i)
   {
      switch(symb_[i])
      {
         case NodeSymbol::Cst:
            itv_[i] = cst_[arg_[i][0]];
            break;

         case NodeSymbol::Var:
            itv_[i] = V[arg_[i][0]];
            break;

         case NodeSymbol::Add:
            itv_[i] = itv_[arg_[i][0]] + itv_[arg_[i][1]];
            break;

         case NodeSymbol::Sub:
            itv_[i] = itv_[arg_[i][0]] - itv_[arg_[i][1]];
            break;

         case NodeSymbol::Mul:
            itv_[i] = itv_[arg_[i][0]] * itv_[arg_[i][1]];
            break;

         case NodeSymbol::Div:
            itv_[i] = itv_[arg_[i][0]] / itv_[arg_[i][1]];
            break;

         case NodeSymbol::Min:
            itv_[i] = min(itv_[arg_[i][0]], itv_[arg_[i][1]]);
            break;

         case NodeSymbol::Max:
            itv_[i] = max(itv_[arg_[i][0]], itv_[arg_[i][1]]);
            break;

         case NodeSymbol::Usb:
            itv_[i] = -itv_[arg_[i][0]];
            break;

         case NodeSymbol::Abs:
            itv_[i] = abs(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sgn:
            itv_[i] = sgn(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sqr:
            itv_[i] = sqr(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sqrt:
            itv_[i] = sqrt(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Pow:
            itv_[i] = pow(itv_[arg_[i][0]], arg_[i][1]);
            break;

         case NodeSymbol::Exp:
            itv_[i] = exp(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Log:
            itv_[i] = log(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Cos:
            itv_[i] = cos(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sin:
            itv_[i] = sin(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Tan:
            itv_[i] = tan(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Cosh:
            itv_[i] = cosh(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sinh:
            itv_[i] = sinh(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Tanh:
            itv_[i] = tanh(itv_[arg_[i][0]]);
            break;
      }
   }

   return itv_[nb_-1];
}

Interval FlatFunction::eval(const IntervalBox& B) const
{
   if (B.isVectorizable())
   {
      return eval(static_cast<const IntervalVector&>(B));
   }

   for (size_t i=0; i<nb_; ++i)
   {
      switch(symb_[i])
      {
         case NodeSymbol::Cst:
            itv_[i] = cst_[arg_[i][0]];
            break;

         case NodeSymbol::Var:
            itv_[i] = B.get(var_[arg_[i][1]]);
            break;

         case NodeSymbol::Add:
            itv_[i] = itv_[arg_[i][0]] + itv_[arg_[i][1]];
            break;

         case NodeSymbol::Sub:
            itv_[i] = itv_[arg_[i][0]] - itv_[arg_[i][1]];
            break;

         case NodeSymbol::Mul:
            itv_[i] = itv_[arg_[i][0]] * itv_[arg_[i][1]];
            break;

         case NodeSymbol::Div:
            itv_[i] = itv_[arg_[i][0]] / itv_[arg_[i][1]];
            break;

         case NodeSymbol::Min:
            itv_[i] = min(itv_[arg_[i][0]], itv_[arg_[i][1]]);
            break;

         case NodeSymbol::Max:
            itv_[i] = max(itv_[arg_[i][0]], itv_[arg_[i][1]]);
            break;

         case NodeSymbol::Usb:
            itv_[i] = -itv_[arg_[i][0]];
            break;

         case NodeSymbol::Abs:
            itv_[i] = abs(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sgn:
            itv_[i] = sgn(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sqr:
            itv_[i] = sqr(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sqrt:
            itv_[i] = sqrt(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Pow:
            itv_[i] = pow(itv_[arg_[i][0]], arg_[i][1]);
            break;

         case NodeSymbol::Exp:
            itv_[i] = exp(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Log:
            itv_[i] = log(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Cos:
            itv_[i] = cos(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sin:
            itv_[i] = sin(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Tan:
            itv_[i] = tan(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Cosh:
            itv_[i] = cosh(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Sinh:
            itv_[i] = sinh(itv_[arg_[i][0]]);
            break;

         case NodeSymbol::Tanh:
            itv_[i] = tanh(itv_[arg_[i][0]]);
            break;
      }
   }

   return itv_[nb_-1];
}

Proof FlatFunction::contract(IntervalBox& B) const
{
   if (B.isVectorizable())
   {
      IntervalVector& V = static_cast<IntervalVector&>(B);
      Interval e = eval(V);

      if (e.isEmpty() || img_.isDisjoint(e))
         return Proof::Empty;

      if (img_.contains(e))
         return Proof::Inner;

      itv_[nb_-1] &= img_;
      return backward(V);
   }
   else
   {
      Interval e = eval(B);

      if (e.isEmpty() || img_.isDisjoint(e))
         return Proof::Empty;

      if (img_.contains(e))
         return Proof::Inner;

      itv_[nb_-1] &= img_;
      return backward(B);
   }
}

Proof FlatFunction::backward(IntervalBox& B) const
{
   Proof res = Proof::Maybe;

   for (int i=(int)nb_-1; i>= 0; --i)
   {
      switch(symb_[i])
      {
         case NodeSymbol::Cst:
            // nothing
            break;

         case NodeSymbol::Var:
            {
               Interval x = B.get(var_[arg_[i][1]]) & itv_[i];
               B.set(var_[arg_[i][1]], x);
               res = x.isEmpty() ? Proof::Empty : Proof::Maybe;
            }
            break;

         case NodeSymbol::Add:
            itv_[arg_[i][0]] = addPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = addPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Sub:
            itv_[arg_[i][0]] = subPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = subPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Mul:
            itv_[arg_[i][0]] = mulPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = mulPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Div:
            itv_[arg_[i][0]] = divPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = divPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Min:
            itv_[arg_[i][0]] = minPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = minPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Max:
            itv_[arg_[i][0]] = maxPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = maxPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Usb:
            itv_[arg_[i][0]] = usubPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Abs:
            itv_[arg_[i][0]] = absPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Sgn:
            itv_[arg_[i][0]] = sgnPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Sqr:
            itv_[arg_[i][0]] = sqrPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Sqrt:
            itv_[arg_[i][0]] = sqrtPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Pow:
            itv_[arg_[i][0]] = powPX(itv_[arg_[i][0]], arg_[i][1], itv_[i]);
            break;

         case NodeSymbol::Exp:
            itv_[arg_[i][0]] = expPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Log:
            itv_[arg_[i][0]] = logPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Cos:
            itv_[arg_[i][0]] = cosPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Sin:
            itv_[arg_[i][0]] = sinPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Tan:
            itv_[arg_[i][0]] = tanPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Cosh:
            itv_[arg_[i][0]] = coshPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Sinh:
            itv_[arg_[i][0]] = sinhPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Tanh:
            itv_[arg_[i][0]] = tanhPX(itv_[arg_[i][0]], itv_[i]);
            break;
      }
   }

   return res;
}

Proof FlatFunction::backward(IntervalVector& V) const
{
   Proof res = Proof::Maybe;

   for (int i=(int)nb_-1; i>= 0; --i)
   {
      switch(symb_[i])
      {
         case NodeSymbol::Cst:
            // nothing
            break;

         case NodeSymbol::Var:
            V[arg_[i][0]] &= itv_[i];
            res = V[arg_[i][0]].isEmpty() ?
                     Proof::Empty : Proof::Maybe;
            break;

         case NodeSymbol::Add:
            itv_[arg_[i][0]] = addPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = addPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Sub:
            itv_[arg_[i][0]] = subPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = subPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Mul:
            itv_[arg_[i][0]] = mulPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = mulPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Div:
            itv_[arg_[i][0]] = divPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = divPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Min:
            itv_[arg_[i][0]] = minPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = minPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Max:
            itv_[arg_[i][0]] = maxPX(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            itv_[arg_[i][1]] = maxPY(itv_[arg_[i][0]], itv_[arg_[i][1]], itv_[i]);
            break;

         case NodeSymbol::Usb:
            itv_[arg_[i][0]] = usubPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Abs:
            itv_[arg_[i][0]] = absPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Sgn:
            itv_[arg_[i][0]] = sgnPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Sqr:
            itv_[arg_[i][0]] = sqrPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Sqrt:
            itv_[arg_[i][0]] = sqrtPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Pow:
            itv_[arg_[i][0]] = powPX(itv_[arg_[i][0]], arg_[i][1], itv_[i]);
            break;

         case NodeSymbol::Exp:
            itv_[arg_[i][0]] = expPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Log:
            itv_[arg_[i][0]] = logPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Cos:
            itv_[arg_[i][0]] = cosPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Sin:
            itv_[arg_[i][0]] = sinPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Tan:
            itv_[arg_[i][0]] = tanPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Cosh:
            itv_[arg_[i][0]] = coshPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Sinh:
            itv_[arg_[i][0]] = sinhPX(itv_[arg_[i][0]], itv_[i]);
            break;

         case NodeSymbol::Tanh:
            itv_[arg_[i][0]] = tanhPX(itv_[arg_[i][0]], itv_[i]);
            break;
      }
   }

   return res;
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
   FlatFunctionCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunctionCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);   
}
   
void FlatFunctionCreator::apply(const TermMul* t)
{
   FlatFunctionCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunctionCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);
}
   
void FlatFunctionCreator::apply(const TermDiv* t)
{
   FlatFunctionCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunctionCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);
}
   
void FlatFunctionCreator::apply(const TermMin* t)
{
   FlatFunctionCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunctionCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);
}
   
void FlatFunctionCreator::apply(const TermMax* t)
{
   FlatFunctionCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunctionCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);
}
   
void FlatFunctionCreator::apply(const TermUsb* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermAbs* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermSgn* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermSqr* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermSqrt* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermPow* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertPow(t->symbol(), vc.idx_, t->exponent());
}
   
void FlatFunctionCreator::apply(const TermExp* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermLog* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermCos* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermSin* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermTan* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermCosh* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermSinh* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunctionCreator::apply(const TermTanh* t)
{
   FlatFunctionCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
} // namespace
