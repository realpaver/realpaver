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
   symb_ = new TermSymbol[capa_];
   arg_ = new size_t*[capa_];
   itv_ = new Interval[capa_];
   nb_ = 0;

   FlatFunTermCreator creator(this);
   t.acceptVisitor(creator);
}

FlatFunction::FlatFunction(const DagFun* f)
   : scop_(f->scope()),
     img_(f->getImage()),
     cst_(),
     var_()
{
   capa_ = 8;
   symb_ = new TermSymbol[capa_];
   arg_ = new size_t*[capa_];
   itv_ = new Interval[capa_];
   nb_ = 0;

   FlatFunDagCreator creator(this);
   f->rootNode()->acceptVisitor(creator);
}

void FlatFunction::extendCapacity()
{
   if (nb_ >= capa_)
   {
      // doubles the capacity
      capa_ *= 2;
      TermSymbol* symb2 = new TermSymbol[capa_];
      size_t** arg2 = new size_t*[capa_];
      Interval* itv2 = new Interval[capa_];

      // copy of data
      for (size_t i=0; i<nb_; ++i)
      {
         symb2[i] = symb_[i];
         
         if (arg_[i] == nullptr)
         {
            arg2[i] = nullptr;
         }
         else
         {
            size_t n = arg_[i][0];
            arg2[i] = new size_t[n];
            for (size_t j=0; j<n; ++j)
               arg2[i][j] = arg_[i][j];
         }
         
         itv2[i] = itv_[i];
      }

      // deallocation
      destroy();

      // copy of pointers
      symb_ = symb2;
      arg_ = arg2;
      itv_ = itv2;
   }
}

void FlatFunction::destroy()
{
  delete[] symb_;
   delete[] itv_;

   for (size_t i=0; i<nb_; ++i)
      if (arg_[i] != nullptr)
         delete[] arg_[i];

   delete[] arg_;
}

FlatFunction::~FlatFunction()
{
   destroy();
}

size_t FlatFunction::insertCst(const Interval& val)
{
   extendCapacity();

   size_t i = nb_++;
   symb_[i] = TermSymbol::Cst;
   arg_[i] = new size_t[2];
   arg_[i][0] = 2;
   arg_[i][1] = cst_.size();
   cst_.push_back(val);
   return i;
}

size_t FlatFunction::insertVar(const Variable& v)
{
   extendCapacity();

   size_t i = nb_++;
   symb_[i] = TermSymbol::Var;
   arg_[i] = new size_t[3];
   arg_[i][0] = 3;
   arg_[i][1] = v.id();
   arg_[i][2] = var_.size();
   var_.push_back(v);
   return i;
}

size_t FlatFunction::insertUnary(TermSymbol symb, size_t ic)
{
   extendCapacity();

   size_t i = nb_++;
   symb_[i] = symb;
   arg_[i] = new size_t[2];
   arg_[i][0] = 2;
   arg_[i][1] = ic;
   return i;
}

size_t FlatFunction::insertBinary(TermSymbol symb, size_t il, size_t ir)
{
   extendCapacity();

   size_t i = nb_++;
   symb_[i] = symb;
   arg_[i] = new size_t[3];
   arg_[i][0] = 3;
   arg_[i][1] = il;
   arg_[i][2] = ir;
   return i;   
}

size_t FlatFunction::insertPow(TermSymbol symb, size_t ic, int e)
{
   extendCapacity();

   size_t i = nb_++;
   symb_[i] = symb;
   arg_[i] = new size_t[3];
   arg_[i][0] = 3;
   arg_[i][1] = ic;
   arg_[i][2] = (size_t)e;
   return i;   
}

Interval FlatFunction::eval(const IntervalVector& V) const
{
   for (size_t i=0; i<nb_; ++i)
   {
      switch(symb_[i])
      {
         case TermSymbol::Cst:
            itv_[i] = cst_[arg_[i][1]];
            break;

         case TermSymbol::Var:
            itv_[i] = V[arg_[i][1]];
            break;

         case TermSymbol::Add:
            itv_[i] = itv_[arg_[i][1]] + itv_[arg_[i][2]];
            break;

         case TermSymbol::Sub:
            itv_[i] = itv_[arg_[i][1]] - itv_[arg_[i][2]];
            break;

         case TermSymbol::Mul:
            itv_[i] = itv_[arg_[i][1]] * itv_[arg_[i][2]];
            break;

         case TermSymbol::Div:
            itv_[i] = itv_[arg_[i][1]] / itv_[arg_[i][2]];
            break;

         case TermSymbol::Min:
            itv_[i] = min(itv_[arg_[i][1]], itv_[arg_[i][2]]);
            break;

         case TermSymbol::Max:
            itv_[i] = max(itv_[arg_[i][1]], itv_[arg_[i][2]]);
            break;

         case TermSymbol::Usb:
            itv_[i] = -itv_[arg_[i][1]];
            break;

         case TermSymbol::Abs:
            itv_[i] = abs(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Sgn:
            itv_[i] = sgn(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Sqr:
            itv_[i] = sqr(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Sqrt:
            itv_[i] = sqrt(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Pow:
            itv_[i] = pow(itv_[arg_[i][1]], arg_[i][2]);
            break;

         case TermSymbol::Exp:
            itv_[i] = exp(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Log:
            itv_[i] = log(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Cos:
            itv_[i] = cos(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Sin:
            itv_[i] = sin(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Tan:
            itv_[i] = tan(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Cosh:
            itv_[i] = cosh(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Sinh:
            itv_[i] = sinh(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Tanh:
            itv_[i] = tanh(itv_[arg_[i][1]]);
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
         case TermSymbol::Cst:
            itv_[i] = cst_[arg_[i][1]];
            break;

         case TermSymbol::Var:
            itv_[i] = B.get(var_[arg_[i][2]]);
            break;

         case TermSymbol::Add:
            itv_[i] = itv_[arg_[i][1]] + itv_[arg_[i][2]];
            break;

         case TermSymbol::Sub:
            itv_[i] = itv_[arg_[i][1]] - itv_[arg_[i][2]];
            break;

         case TermSymbol::Mul:
            itv_[i] = itv_[arg_[i][1]] * itv_[arg_[i][2]];
            break;

         case TermSymbol::Div:
            itv_[i] = itv_[arg_[i][1]] / itv_[arg_[i][2]];
            break;

         case TermSymbol::Min:
            itv_[i] = min(itv_[arg_[i][1]], itv_[arg_[i][2]]);
            break;

         case TermSymbol::Max:
            itv_[i] = max(itv_[arg_[i][1]], itv_[arg_[i][2]]);
            break;

         case TermSymbol::Usb:
            itv_[i] = -itv_[arg_[i][1]];
            break;

         case TermSymbol::Abs:
            itv_[i] = abs(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Sgn:
            itv_[i] = sgn(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Sqr:
            itv_[i] = sqr(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Sqrt:
            itv_[i] = sqrt(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Pow:
            itv_[i] = pow(itv_[arg_[i][1]], arg_[i][2]);
            break;

         case TermSymbol::Exp:
            itv_[i] = exp(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Log:
            itv_[i] = log(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Cos:
            itv_[i] = cos(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Sin:
            itv_[i] = sin(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Tan:
            itv_[i] = tan(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Cosh:
            itv_[i] = cosh(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Sinh:
            itv_[i] = sinh(itv_[arg_[i][1]]);
            break;

         case TermSymbol::Tanh:
            itv_[i] = tanh(itv_[arg_[i][1]]);
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
         case TermSymbol::Cst:
            // nothing
            break;

         case TermSymbol::Var:
            {
               Interval x = B.get(var_[arg_[i][2]]) & itv_[i];
               B.set(var_[arg_[i][2]], x);
               res = x.isEmpty() ? Proof::Empty : Proof::Maybe;
            }
            break;

         case TermSymbol::Add:
            itv_[arg_[i][1]] = addPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = addPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Sub:
            itv_[arg_[i][1]] = subPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = subPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Mul:
            itv_[arg_[i][1]] = mulPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = mulPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Div:
            itv_[arg_[i][1]] = divPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = divPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Min:
            itv_[arg_[i][1]] = minPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = minPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Max:
            itv_[arg_[i][1]] = maxPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = maxPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Usb:
            itv_[arg_[i][1]] = usubPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Abs:
            itv_[arg_[i][1]] = absPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Sgn:
            itv_[arg_[i][1]] = sgnPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Sqr:
            itv_[arg_[i][1]] = sqrPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Sqrt:
            itv_[arg_[i][1]] = sqrtPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Pow:
            itv_[arg_[i][1]] = powPX(itv_[arg_[i][1]], arg_[i][2], itv_[i]);
            break;

         case TermSymbol::Exp:
            itv_[arg_[i][1]] = expPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Log:
            itv_[arg_[i][1]] = logPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Cos:
            itv_[arg_[i][1]] = cosPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Sin:
            itv_[arg_[i][1]] = sinPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Tan:
            itv_[arg_[i][1]] = tanPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Cosh:
            itv_[arg_[i][1]] = coshPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Sinh:
            itv_[arg_[i][1]] = sinhPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Tanh:
            itv_[arg_[i][1]] = tanhPX(itv_[arg_[i][1]], itv_[i]);
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
         case TermSymbol::Cst:
            // nothing
            break;

         case TermSymbol::Var:
            V[arg_[i][1]] &= itv_[i];
            res = V[arg_[i][1]].isEmpty() ?
                     Proof::Empty : Proof::Maybe;
            break;

         case TermSymbol::Add:
            itv_[arg_[i][1]] = addPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = addPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Sub:
            itv_[arg_[i][1]] = subPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = subPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Mul:
            itv_[arg_[i][1]] = mulPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = mulPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Div:
            itv_[arg_[i][1]] = divPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = divPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Min:
            itv_[arg_[i][1]] = minPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = minPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Max:
            itv_[arg_[i][1]] = maxPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = maxPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case TermSymbol::Usb:
            itv_[arg_[i][1]] = usubPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Abs:
            itv_[arg_[i][1]] = absPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Sgn:
            itv_[arg_[i][1]] = sgnPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Sqr:
            itv_[arg_[i][1]] = sqrPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Sqrt:
            itv_[arg_[i][1]] = sqrtPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Pow:
            itv_[arg_[i][1]] = powPX(itv_[arg_[i][1]], arg_[i][2], itv_[i]);
            break;

         case TermSymbol::Exp:
            itv_[arg_[i][1]] = expPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Log:
            itv_[arg_[i][1]] = logPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Cos:
            itv_[arg_[i][1]] = cosPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Sin:
            itv_[arg_[i][1]] = sinPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Tan:
            itv_[arg_[i][1]] = tanPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Cosh:
            itv_[arg_[i][1]] = coshPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Sinh:
            itv_[arg_[i][1]] = sinhPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case TermSymbol::Tanh:
            itv_[arg_[i][1]] = tanhPX(itv_[arg_[i][1]], itv_[i]);
            break;
      }
   }

   return res;
}

/*----------------------------------------------------------------------------*/

FlatFunTermCreator::FlatFunTermCreator(FlatFunction* f)
      : f_(f)
{}

void FlatFunTermCreator::apply(const TermCst* t)
{
   idx_ = f_->insertCst(t->getVal());
}
   
void FlatFunTermCreator::apply(const TermVar* t)
{
   idx_ = f_->insertVar(t->var());
}

void FlatFunTermCreator::apply(const TermAdd* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);
}
   
void FlatFunTermCreator::apply(const TermSub* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);   
}
   
void FlatFunTermCreator::apply(const TermMul* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);
}
   
void FlatFunTermCreator::apply(const TermDiv* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);
}
   
void FlatFunTermCreator::apply(const TermMin* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);
}
   
void FlatFunTermCreator::apply(const TermMax* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(t->symbol(), vl.idx_, vr.idx_);
}
   
void FlatFunTermCreator::apply(const TermUsb* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermAbs* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermSgn* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermSqr* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermSqrt* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermPow* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertPow(t->symbol(), vc.idx_, t->exponent());
}
   
void FlatFunTermCreator::apply(const TermExp* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermLog* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermCos* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermSin* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermTan* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermCosh* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermSinh* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermTanh* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(t->symbol(), vc.idx_);
}

/*----------------------------------------------------------------------------*/

FlatFunDagCreator::FlatFunDagCreator(FlatFunction* f)
      : f_(f)
{}

void FlatFunDagCreator::apply(const DagConst* d)
{
   idx_ = f_->insertCst(d->getConst());
}

void FlatFunDagCreator::apply(const DagVar* d)
{
   idx_ = f_->insertVar(d->getVar());
}

void FlatFunDagCreator::apply(const DagAdd* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(TermSymbol::Add, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagSub* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(TermSymbol::Sub, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagMul* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(TermSymbol::Mul, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagDiv* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(TermSymbol::Div, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagMin* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(TermSymbol::Min, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagMax* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(TermSymbol::Max, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagUsb* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Usb, vc.idx_);
}

void FlatFunDagCreator::apply(const DagAbs* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Abs, vc.idx_);   
}

void FlatFunDagCreator::apply(const DagSgn* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Sgn, vc.idx_);
}

void FlatFunDagCreator::apply(const DagSqr* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Sqr, vc.idx_);
}

void FlatFunDagCreator::apply(const DagSqrt* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Sqrt, vc.idx_);
}

void FlatFunDagCreator::apply(const DagPow* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertPow(TermSymbol::Pow, vc.idx_, d->exponent());
}

void FlatFunDagCreator::apply(const DagExp* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Exp, vc.idx_);
}

void FlatFunDagCreator::apply(const DagLog* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Log, vc.idx_);
}

void FlatFunDagCreator::apply(const DagCos* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Cos, vc.idx_);
}

void FlatFunDagCreator::apply(const DagSin* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Sin, vc.idx_);
}

void FlatFunDagCreator::apply(const DagTan* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Tan, vc.idx_);
}

void FlatFunDagCreator::apply(const DagCosh* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Cosh, vc.idx_);
}

void FlatFunDagCreator::apply(const DagSinh* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Sinh, vc.idx_);
}

void FlatFunDagCreator::apply(const DagTanh* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(TermSymbol::Tanh, vc.idx_);
}

} // namespace
