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

std::ostream& operator<<(std::ostream& os, FlatSymbol op)
{
   switch(op)
   {
      case FlatSymbol::Cst:  return os << "cst";
      case FlatSymbol::Var:  return os << "var";
      case FlatSymbol::Add:  return os << "+";
      case FlatSymbol::AddL: return os << "+";
      case FlatSymbol::AddR: return os << "+";
      case FlatSymbol::Sub:  return os << "-";
      case FlatSymbol::SubL: return os << "-";
      case FlatSymbol::SubR: return os << "-";
      case FlatSymbol::Mul:  return os << "*";
      case FlatSymbol::MulL: return os << "*";
      case FlatSymbol::MulR: return os << "*";
      case FlatSymbol::Div:  return os << "/";
      case FlatSymbol::DivL: return os << "/";
      case FlatSymbol::DivR: return os << "/";
      case FlatSymbol::Min:  return os << "min";
      case FlatSymbol::Max:  return os << "max";
      case FlatSymbol::Usb:  return os << "-";
      case FlatSymbol::Abs:  return os << "abs";
      case FlatSymbol::Sgn:  return os << "sgn";
      case FlatSymbol::Sqr:  return os << "sqr";
      case FlatSymbol::Sqrt: return os << "sqrt";
      case FlatSymbol::Pow:  return os << "pow";
      case FlatSymbol::Exp:  return os << "exp";
      case FlatSymbol::Log:  return os << "log";
      case FlatSymbol::Cos:  return os << "cos";
      case FlatSymbol::Sin:  return os << "sin";
      case FlatSymbol::Tan:  return os << "tan";
      case FlatSymbol::Cosh: return os << "cosh";
      case FlatSymbol::Sinh: return os << "sinh";
      case FlatSymbol::Tanh: return os << "tanh";
      default:             os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

FlatFunction::FlatFunction(const Term& t, const Interval& img)
   : scop_(),
     img_(img),
     cst_(),
     var_()
{
   t.makeScope(scop_);
   capa_ = t.nbNodes();
   symb_ = new FlatSymbol[capa_];
   arg_ = new size_t*[capa_];
   itv_ = new Interval[capa_];
   dv_ = new Interval[capa_];
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
   symb_ = new FlatSymbol[capa_];
   arg_ = new size_t*[capa_];
   itv_ = new Interval[capa_];
   dv_ = new Interval[capa_];
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
      FlatSymbol* symb2 = new FlatSymbol[capa_];
      size_t** arg2 = new size_t*[capa_];
      Interval* itv2 = new Interval[capa_];
      Interval* dv2 = new Interval[capa_];

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
         dv2[i] = dv_[i];
      }

      // deallocation
      destroy();

      // copy of pointers
      symb_ = symb2;
      arg_ = arg2;
      itv_ = itv2;
      dv_ = dv2;
   }
}

void FlatFunction::destroy()
{
   delete[] symb_;
   delete[] itv_;
   delete[] dv_;

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
   symb_[i] = FlatSymbol::Cst;
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
   symb_[i] = FlatSymbol::Var;
   arg_[i] = new size_t[4];
   arg_[i][0] = 4;
   arg_[i][1] = v.id();          // identifier of v
   arg_[i][2] = var_.size();     // position of this occurrence in var_
   arg_[i][3] = scop_.index(v);  // index of v in scop_
   var_.push_back(v);
   return i;
}

size_t FlatFunction::insertUnary(FlatSymbol symb, size_t ic)
{
   extendCapacity();

   size_t i = nb_++;
   symb_[i] = symb;
   arg_[i] = new size_t[2];
   arg_[i][0] = 2;
   arg_[i][1] = ic;
   return i;
}

size_t FlatFunction::insertBinary(FlatSymbol symb, size_t il, size_t ir)
{
   extendCapacity();

   size_t i = nb_++;
   arg_[i] = new size_t[3];
   arg_[i][0] = 3;
   arg_[i][1] = il;
   arg_[i][2] = ir;

   symb_[i] = symb;

   if (symb == FlatSymbol::Add)
   {      
      if (symb_[il] == FlatSymbol::Cst)
         symb_[i] = FlatSymbol::AddL;

      else if (symb_[ir] == FlatSymbol::Cst)
         symb_[i] = FlatSymbol::AddR;
   }

   if (symb == FlatSymbol::Sub)
   {      
      if (symb_[il] == FlatSymbol::Cst)
         symb_[i] = FlatSymbol::SubL;

      else if (symb_[ir] == FlatSymbol::Cst)
         symb_[i] = FlatSymbol::SubR;
   }

   if (symb == FlatSymbol::Mul)
   {      
      if (symb_[il] == FlatSymbol::Cst)
         symb_[i] = FlatSymbol::MulL;

      else if (symb_[ir] == FlatSymbol::Cst)
         symb_[i] = FlatSymbol::MulR;
   }

   if (symb == FlatSymbol::Div)
   {      
      if (symb_[il] == FlatSymbol::Cst)
         symb_[i] = FlatSymbol::DivL;

      else if (symb_[ir] == FlatSymbol::Cst)
         symb_[i] = FlatSymbol::DivR;
   }

   return i;   
}

size_t FlatFunction::insertLin(const Interval& x, Variable v)
{
   size_t i = insertVar(v);

   if (x.isOne())
   {
      return i;
   }
   else
   {
      size_t j = insertCst(x);
      return insertBinary(FlatSymbol::MulL, j, i);
   }
}

size_t FlatFunction::insertPow(FlatSymbol symb, size_t ic, int e)
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

Scope FlatFunction::scope() const
{
   return scop_;
}

bool FlatFunction::dependsOn(const Variable& v) const
{
   return scop_.contains(v);
}

Interval FlatFunction::ival() const
{
   return itv_[nb_-1];;
}


Interval FlatFunction::iEval(const IntervalVector& V)
{
   for (size_t i=0; i<nb_; ++i)
   {
      switch(symb_[i])
      {
         case FlatSymbol::Cst:
            itv_[i] = cst_[arg_[i][1]];
            break;

         case FlatSymbol::Var:
            itv_[i] = V[arg_[i][1]];
            break;

         case FlatSymbol::Add:
         case FlatSymbol::AddL:
         case FlatSymbol::AddR:
            itv_[i] = itv_[arg_[i][1]] + itv_[arg_[i][2]];
            break;

         case FlatSymbol::Sub:
         case FlatSymbol::SubL:
         case FlatSymbol::SubR:
            itv_[i] = itv_[arg_[i][1]] - itv_[arg_[i][2]];
            break;

         case FlatSymbol::Mul:
         case FlatSymbol::MulL:
         case FlatSymbol::MulR:
            itv_[i] = itv_[arg_[i][1]] * itv_[arg_[i][2]];
            break;

         case FlatSymbol::Div:
         case FlatSymbol::DivL:
         case FlatSymbol::DivR:
            itv_[i] = itv_[arg_[i][1]] / itv_[arg_[i][2]];
            break;

         case FlatSymbol::Min:
            itv_[i] = min(itv_[arg_[i][1]], itv_[arg_[i][2]]);
            break;

         case FlatSymbol::Max:
            itv_[i] = max(itv_[arg_[i][1]], itv_[arg_[i][2]]);
            break;

         case FlatSymbol::Usb:
            itv_[i] = -itv_[arg_[i][1]];
            break;

         case FlatSymbol::Abs:
            itv_[i] = abs(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sgn:
            itv_[i] = sgn(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sqr:
            itv_[i] = sqr(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sqrt:
            itv_[i] = sqrt(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Pow:
            itv_[i] = pow(itv_[arg_[i][1]], arg_[i][2]);
            break;

         case FlatSymbol::Exp:
            itv_[i] = exp(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Log:
            itv_[i] = log(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Cos:
            itv_[i] = cos(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sin:
            itv_[i] = sin(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Tan:
            itv_[i] = tan(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Cosh:
            itv_[i] = cosh(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sinh:
            itv_[i] = sinh(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Tanh:
            itv_[i] = tanh(itv_[arg_[i][1]]);
            break;
      }
   }

   return itv_[nb_-1];
}

Interval FlatFunction::iEval(const IntervalBox& B)
{
   if (B.isVectorizable())
   {
      return iEval(static_cast<const IntervalVector&>(B));
   }

   for (size_t i=0; i<nb_; ++i)
   {
      switch(symb_[i])
      {
         case FlatSymbol::Cst:
            itv_[i] = cst_[arg_[i][1]];
            break;

         case FlatSymbol::Var:
            itv_[i] = B.get(var_[arg_[i][2]]);
            break;

         case FlatSymbol::Add:
         case FlatSymbol::AddL:
         case FlatSymbol::AddR:
            itv_[i] = itv_[arg_[i][1]] + itv_[arg_[i][2]];
            break;

         case FlatSymbol::Sub:
         case FlatSymbol::SubL:
         case FlatSymbol::SubR:
            itv_[i] = itv_[arg_[i][1]] - itv_[arg_[i][2]];
            break;

         case FlatSymbol::Mul:
         case FlatSymbol::MulL:
         case FlatSymbol::MulR:
            itv_[i] = itv_[arg_[i][1]] * itv_[arg_[i][2]];
            break;

         case FlatSymbol::Div:
         case FlatSymbol::DivL:
         case FlatSymbol::DivR:
            itv_[i] = itv_[arg_[i][1]] / itv_[arg_[i][2]];
            break;

         case FlatSymbol::Min:
            itv_[i] = min(itv_[arg_[i][1]], itv_[arg_[i][2]]);
            break;

         case FlatSymbol::Max:
            itv_[i] = max(itv_[arg_[i][1]], itv_[arg_[i][2]]);
            break;

         case FlatSymbol::Usb:
            itv_[i] = -itv_[arg_[i][1]];
            break;

         case FlatSymbol::Abs:
            itv_[i] = abs(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sgn:
            itv_[i] = sgn(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sqr:
            itv_[i] = sqr(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sqrt:
            itv_[i] = sqrt(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Pow:
            itv_[i] = pow(itv_[arg_[i][1]], arg_[i][2]);
            break;

         case FlatSymbol::Exp:
            itv_[i] = exp(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Log:
            itv_[i] = log(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Cos:
            itv_[i] = cos(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sin:
            itv_[i] = sin(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Tan:
            itv_[i] = tan(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Cosh:
            itv_[i] = cosh(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sinh:
            itv_[i] = sinh(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Tanh:
            itv_[i] = tanh(itv_[arg_[i][1]]);
            break;
      }
   }

   return itv_[nb_-1];
}

Proof FlatFunction::hc4Revise(IntervalBox& B)
{
   if (B.isVectorizable())
   {
      IntervalVector& V = static_cast<IntervalVector&>(B);
      Interval e = iEval(V);

      if (e.isEmpty() || img_.isDisjoint(e))
         return Proof::Empty;

      if (img_.contains(e))
         return Proof::Inner;

      itv_[nb_-1] &= img_;
      return hc4ReviseBackward(V);
   }
   else
   {
      Interval e = iEval(B);

      if (e.isEmpty() || img_.isDisjoint(e))
         return Proof::Empty;

      if (img_.contains(e))
         return Proof::Inner;

      itv_[nb_-1] &= img_;
      return hc4ReviseBackward(B);
   }
}

Proof FlatFunction::hc4ReviseNeg(IntervalBox& B)
{
   Interval e = iEval(B);

   if (e.isEmpty())
      return Proof::Empty;

   else if (img_.contains(e))
      return Proof::Empty;

   else if (!img_.overlaps(e))
      return Proof::Inner;

   else
   {
      if (img_.isSingleton() || img_.isUniverse())
         return Proof::Maybe;

      else if (img_.isInfLeft())
      {
         // projection over the root node
         itv_[nb_-1] = e & Interval::moreThan(img_.right());
         return hc4ReviseBackward(B);
      }

      else if (img_.isInfRight())
      {
         // projection over the root node
         itv_[nb_-1] = e & Interval::lessThan(img_.left());
         return hc4ReviseBackward(B);
      }

      else
      {
         // given the image [a, b] calculates the projections for each
         // part [-oo, a] and [b, +oo] and makes the interval disjunction

         // copies the interval values
         Interval* aux = new Interval[nb_];
         for (size_t i=0; i<nb_; ++i)
            aux[i] = itv_[i];

         // contracts the first part
         IntervalBox Xl(B, scop_);
         itv_[nb_-1] = e & Interval::lessThan(img_.left());
         Proof pl = hc4ReviseBackward(Xl);

         // restores the interval values
         for (size_t i=0; i<nb_; ++i)
            itv_[i] = aux[i];

         // contracts the second part
         IntervalBox Xr(B, scop_);   
         itv_[nb_-1] = e & Interval::moreThan(img_.right());
         Proof pr = hc4ReviseBackward(Xr);

         // deallocates the memory
         delete[] aux;

         // constructive disjunction
         Proof proof;

         if (pl == Proof::Empty)
            proof = pr;

         else if (pr == Proof::Empty)
            proof = pl;

         else
            proof = std::min(pl, pr);

         if (proof != Proof::Empty)
         {
            for (size_t i=0; i<var_.size(); ++i)
            {
               Variable v = var_[i];

               if (pl == Proof::Empty)
                  B.set(v, Xr.get(v));

               else if (pr == Proof::Empty)
                  B.set(v, Xl.get(v));

               else
                  B.set(v, Xl.get(v) | Xr.get(v));
            }
         }

         return proof;
      }
   }
}

Proof FlatFunction::hc4ReviseBackward(IntervalBox& B)
{
   Proof res = Proof::Maybe;

   for (int i=(int)nb_-1; i>= 0; --i)
   {
      switch(symb_[i])
      {
         case FlatSymbol::Cst:
            // nothing
            break;

         case FlatSymbol::Var:
            {
               Interval x = B.get(var_[arg_[i][2]]) & itv_[i];
               B.set(var_[arg_[i][2]], x);
               res = x.isEmpty() ? Proof::Empty : Proof::Maybe;
            }
            break;

         case FlatSymbol::Add:
            itv_[arg_[i][1]] = addPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = addPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::AddL:
            itv_[arg_[i][2]] = addPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::AddR:
            itv_[arg_[i][1]] = addPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Sub:
            itv_[arg_[i][1]] = subPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = subPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::SubL:
            itv_[arg_[i][2]] = subPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::SubR:
            itv_[arg_[i][1]] = subPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Mul:
            itv_[arg_[i][1]] = mulPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = mulPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::MulL:
            itv_[arg_[i][2]] = mulPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::MulR:
            itv_[arg_[i][1]] = mulPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Div:
            itv_[arg_[i][1]] = divPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = divPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::DivL:
            itv_[arg_[i][2]] = divPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::DivR:
            itv_[arg_[i][1]] = divPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Min:
            itv_[arg_[i][1]] = minPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = minPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Max:
            itv_[arg_[i][1]] = maxPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = maxPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Usb:
            itv_[arg_[i][1]] = usubPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Abs:
            itv_[arg_[i][1]] = absPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Sgn:
            itv_[arg_[i][1]] = sgnPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Sqr:
            itv_[arg_[i][1]] = sqrPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Sqrt:
            itv_[arg_[i][1]] = sqrtPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Pow:
            itv_[arg_[i][1]] = powPX(itv_[arg_[i][1]], arg_[i][2], itv_[i]);
            break;

         case FlatSymbol::Exp:
            itv_[arg_[i][1]] = expPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Log:
            itv_[arg_[i][1]] = logPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Cos:
            itv_[arg_[i][1]] = cosPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Sin:
            itv_[arg_[i][1]] = sinPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Tan:
            itv_[arg_[i][1]] = tanPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Cosh:
            itv_[arg_[i][1]] = coshPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Sinh:
            itv_[arg_[i][1]] = sinhPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Tanh:
            itv_[arg_[i][1]] = tanhPX(itv_[arg_[i][1]], itv_[i]);
            break;
      }
   }

   return res;
}

Proof FlatFunction::hc4ReviseBackward(IntervalVector& V)
{
   Proof res = Proof::Maybe;

   for (int i=(int)nb_-1; i>= 0; --i)
   {
      switch(symb_[i])
      {
         case FlatSymbol::Cst:
            // nothing
            break;

         case FlatSymbol::Var:
            V[arg_[i][1]] &= itv_[i];
            res = V[arg_[i][1]].isEmpty() ?
                     Proof::Empty : Proof::Maybe;
            break;

         case FlatSymbol::Add:
            itv_[arg_[i][1]] = addPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = addPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::AddL:
            itv_[arg_[i][2]] = addPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::AddR:
            itv_[arg_[i][1]] = addPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Sub:
            itv_[arg_[i][1]] = subPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = subPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::SubL:
            itv_[arg_[i][2]] = subPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::SubR:
            itv_[arg_[i][1]] = subPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Mul:
            itv_[arg_[i][1]] = mulPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = mulPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::MulL:
            itv_[arg_[i][2]] = mulPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::MulR:
            itv_[arg_[i][1]] = mulPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Div:
            itv_[arg_[i][1]] = divPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = divPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::DivL:
            itv_[arg_[i][2]] = divPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::DivR:
            itv_[arg_[i][1]] = divPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Min:
            itv_[arg_[i][1]] = minPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = minPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Max:
            itv_[arg_[i][1]] = maxPX(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            itv_[arg_[i][2]] = maxPY(itv_[arg_[i][1]], itv_[arg_[i][2]], itv_[i]);
            break;

         case FlatSymbol::Usb:
            itv_[arg_[i][1]] = usubPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Abs:
            itv_[arg_[i][1]] = absPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Sgn:
            itv_[arg_[i][1]] = sgnPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Sqr:
            itv_[arg_[i][1]] = sqrPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Sqrt:
            itv_[arg_[i][1]] = sqrtPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Pow:
            itv_[arg_[i][1]] = powPX(itv_[arg_[i][1]], arg_[i][2], itv_[i]);
            break;

         case FlatSymbol::Exp:
            itv_[arg_[i][1]] = expPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Log:
            itv_[arg_[i][1]] = logPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Cos:
            itv_[arg_[i][1]] = cosPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Sin:
            itv_[arg_[i][1]] = sinPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Tan:
            itv_[arg_[i][1]] = tanPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Cosh:
            itv_[arg_[i][1]] = coshPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Sinh:
            itv_[arg_[i][1]] = sinhPX(itv_[arg_[i][1]], itv_[i]);
            break;

         case FlatSymbol::Tanh:
            itv_[arg_[i][1]] = tanhPX(itv_[arg_[i][1]], itv_[i]);
            break;
      }
   }

   return res;
}

void FlatFunction::iDiff(const IntervalBox& B, IntervalVector& G)
{
   ASSERT(scop_.size() == G.size(),
          "The size of the gradient must be equal to the number " <<
          "of variables of the flat function");

   // forward phase: evaluates this on B
   Interval val = iEval(B);

   if (val.isEmpty())
   {
      G.setEmpty();
      return;
   }

   // backward phase
   iDiff();

   // fills the gradient
   G.setAll(Interval::zero());

   for (size_t i=0; i<nb_; ++i)      
      if (symb_[i] == FlatSymbol::Var)
         G[arg_[i][3]] += dv_[i];
}

void FlatFunction::iDiff()
{
   // derivative with respect to the root node
   dv_[nb_-1] = Interval::one();

   // differentiates the other nodes from the root to the leaves
   for (int i=(int)nb_-1; i>= 0; --i)
   {
      switch(symb_[i])
      {
         case FlatSymbol::Cst:
         case FlatSymbol::Var:
            break;

         case FlatSymbol::Add:
            // d(l+r)/dl = 1, d(l+r)/dr = 1
            dv_[arg_[i][1]] = dv_[i];
            dv_[arg_[i][2]] = dv_[i];
            break;

         case FlatSymbol::AddL:
            // d(l+r)/dr = 1
            dv_[arg_[i][2]] = dv_[i];
            break;

         case FlatSymbol::AddR:
            // d(l+r)/dl = 1
            dv_[arg_[i][1]] = dv_[i];
            break;

         case FlatSymbol::Sub:
            // d(l-r)/dl = 1, d(l-r)/dr = -1
            dv_[arg_[i][1]] = dv_[i];
            dv_[arg_[i][2]] = -dv_[i];
            break;

         case FlatSymbol::SubL:
            // d(l-r)/dr = -1
            dv_[arg_[i][2]] = -dv_[i];
            break;

         case FlatSymbol::SubR:
            // d(l-r)/dl = 1
            dv_[arg_[i][1]] = dv_[i];
            break;

         case FlatSymbol::Mul:
            // d(l*r)/dl = r, d(l*r)/dr = l
            dv_[arg_[i][1]] = itv_[arg_[i][2]]*dv_[i];
            dv_[arg_[i][2]] = itv_[arg_[i][1]]*dv_[i];
            break;

         case FlatSymbol::MulL:
            // d(l*r)/dr = l
            dv_[arg_[i][2]] = itv_[arg_[i][1]]*dv_[i];
            break;

         case FlatSymbol::MulR:
            // d(l*r)/dl = r
            dv_[arg_[i][1]] = itv_[arg_[i][2]]*dv_[i];
            break;

         case FlatSymbol::Div:
            // d(l/r)/dl = 1/r, d(l/r)/dr = -l/r^2
            dv_[arg_[i][1]] = dv_[i]/itv_[arg_[i][2]];
            dv_[arg_[i][2]] = (-dv_[i]*itv_[arg_[i][1]])/sqr(itv_[arg_[i][2]]);
            break;

         case FlatSymbol::DivL:
            // d(l/r)/dr = -l/r^2
            dv_[arg_[i][2]] = (-dv_[i]*itv_[arg_[i][1]])/sqr(itv_[arg_[i][2]]);
            break;

         case FlatSymbol::DivR:
            // d(l/r)/dl = 1/r
            dv_[arg_[i][1]] = dv_[i]/itv_[arg_[i][2]];
            break;

         case FlatSymbol::Min:
            if (itv_[arg_[i][1]].isCertainlyLt(itv_[arg_[i][2]]))
            {
               // d(min(l,r))/dl = 1, d(min(l,r))/dr = 0 if l < r
               dv_[arg_[i][1]] = dv_[i];
               dv_[arg_[i][2]] = Interval::zero();
            }
            else if (itv_[arg_[i][2]].isCertainlyLt(itv_[arg_[i][1]]))
            {
               // d(min(l,r))/dl = 0, d(min(l,r))/dr = 1 if r < l
               dv_[arg_[i][1]] = Interval::zero();
               dv_[arg_[i][2]] = dv_[i];
            }
            else
            {
               // d(min(l,r))/dl = d(min(l,r))/dr = [0,1] otherwise
               Interval x = Interval(0.0, 1.0)*dv_[i];
               dv_[arg_[i][1]] = x;
               dv_[arg_[i][2]] = x;
            }
            break;

         case FlatSymbol::Max:
            if (itv_[arg_[i][1]].isCertainlyGt(itv_[arg_[i][2]]))
            {
               // d(max(l,r))/dl = 1 and d(max(l,r))/dr = 0 if l > r
               dv_[arg_[i][1]] = dv_[i];
               dv_[arg_[i][2]] = Interval::zero();
            }
            else if (itv_[arg_[i][2]].isCertainlyGt(itv_[arg_[i][1]]))
            {
               // d(max(l,r))/dl = 0 and d(max(l,r))/dr = 1 if r > l
               dv_[arg_[i][1]] = Interval::zero();
               dv_[arg_[i][2]] = dv_[i];
            }
            else
            {
               // d(max(l,r))/dl = d(max(l,r))/dr = [0,1] otherwise
               Interval x = Interval(0.0, 1.0)*dv_[i];
               dv_[arg_[i][1]] = x;
               dv_[arg_[i][2]] = x;
            }
            break;
   
         case FlatSymbol::Usb:
            // d(-u)/du = -1
            dv_[arg_[i][1]] = -dv_[i];
            break;

         case FlatSymbol::Abs:
            // d(abs(u))/du = 1 if u>0, -1 if u<0, [-1,1] otherwise
            if (itv_[arg_[i][1]].isCertainlyGtZero())
            {
               dv_[arg_[i][1]] = dv_[i];
            }
            else if (itv_[arg_[i][1]].isCertainlyLtZero())
            {
               dv_[arg_[i][1]] = -dv_[i];
            }
            else
            {
               dv_[arg_[i][1]] = dv_[i] | (-dv_[i]);
            }
            break;

         case FlatSymbol::Sgn:
            // d(sgn(u))/du = 0 except at 0
            dv_[arg_[i][1]] = Interval::zero();
            break;

         case FlatSymbol::Sqr:
            // d(u^2)/du = 2u
            dv_[arg_[i][1]] = 2.0*itv_[arg_[i][1]]*dv_[i];   
            break;

         case FlatSymbol::Sqrt:
            // d(sqrt(u))/du = 0.5/sqrt(u)
            dv_[arg_[i][1]] = 0.5*dv_[i]*itv_[i];
            break;

         case FlatSymbol::Pow:
         {
            // d(u^n)/du = n * u^(n-1)
            int e = (int)arg_[i][2];
            dv_[arg_[i][1]] = Interval(e)*dv_[i]*pow(itv_[arg_[i][1]], e-1);
         }
            break;

         case FlatSymbol::Exp:
            // d(exp(u))/du = exp(u)
            dv_[arg_[i][1]] = dv_[i]*itv_[i];
            break;

         case FlatSymbol::Log:
            // d(log(u))/du = 1/u
            dv_[arg_[i][1]] = dv_[i] / itv_[arg_[i][1]];
            break;

         case FlatSymbol::Cos:
            // d(cos(u))/du = -sin(u)
            dv_[arg_[i][1]] = -dv_[i]*sin(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sin:
            // d(sin(u))/du = cos(u)
            dv_[arg_[i][1]] = dv_[i]*cos(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Tan:
            // d(tan(u))/du = 1+tan^2(u)
            dv_[arg_[i][1]] = dv_[i]*(1.0+sqr(itv_[i]));
            break;

         case FlatSymbol::Cosh:
            // d(cosh(u))/du = sinh(u)
            dv_[arg_[i][1]] = dv_[i]*sinh(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Sinh:
            // d(sinh(u))/du = cosh(u)
            dv_[arg_[i][1]] = dv_[i]*cosh(itv_[arg_[i][1]]);
            break;

         case FlatSymbol::Tanh:
            // d(tanh(u))/du = 1-tanh^2(u)
            dv_[arg_[i][1]] = dv_[i]*(1.0-sqr(itv_[i]));
            break;
      }
   }
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

   idx_ = f_->insertBinary(FlatSymbol::Add, vl.idx_, vr.idx_);
}
   
void FlatFunTermCreator::apply(const TermSub* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(FlatSymbol::Sub, vl.idx_, vr.idx_);   
}
   
void FlatFunTermCreator::apply(const TermMul* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(FlatSymbol::Mul, vl.idx_, vr.idx_);
}
   
void FlatFunTermCreator::apply(const TermDiv* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(FlatSymbol::Div, vl.idx_, vr.idx_);
}
   
void FlatFunTermCreator::apply(const TermMin* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(FlatSymbol::Min, vl.idx_, vr.idx_);
}
   
void FlatFunTermCreator::apply(const TermMax* t)
{
   FlatFunTermCreator vl(f_);
   t->left()->acceptVisitor(vl);

   FlatFunTermCreator vr(f_);
   t->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(FlatSymbol::Max, vl.idx_, vr.idx_);
}
   
void FlatFunTermCreator::apply(const TermUsb* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Usb, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermAbs* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Abs, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermSgn* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Sgn, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermSqr* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Sqr, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermSqrt* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Sqrt, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermPow* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertPow(FlatSymbol::Pow, vc.idx_, t->exponent());
}
   
void FlatFunTermCreator::apply(const TermExp* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Exp, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermLog* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Log, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermCos* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Cos, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermSin* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Sin, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermTan* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Tan, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermCosh* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Cosh, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermSinh* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Sinh, vc.idx_);
}
   
void FlatFunTermCreator::apply(const TermTanh* t)
{
   FlatFunTermCreator vc(f_);
   t->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Tanh, vc.idx_);
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

   idx_ = f_->insertBinary(FlatSymbol::Add, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagSub* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(FlatSymbol::Sub, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagMul* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(FlatSymbol::Mul, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagDiv* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(FlatSymbol::Div, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagMin* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(FlatSymbol::Min, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagMax* d)
{
   FlatFunDagCreator vl(f_);
   d->left()->acceptVisitor(vl);

   FlatFunDagCreator vr(f_);
   d->right()->acceptVisitor(vr);

   idx_ = f_->insertBinary(FlatSymbol::Max, vl.idx_, vr.idx_);
}

void FlatFunDagCreator::apply(const DagUsb* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Usb, vc.idx_);
}

void FlatFunDagCreator::apply(const DagAbs* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Abs, vc.idx_);   
}

void FlatFunDagCreator::apply(const DagSgn* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Sgn, vc.idx_);
}

void FlatFunDagCreator::apply(const DagSqr* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Sqr, vc.idx_);
}

void FlatFunDagCreator::apply(const DagSqrt* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Sqrt, vc.idx_);
}

void FlatFunDagCreator::apply(const DagPow* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertPow(FlatSymbol::Pow, vc.idx_, d->exponent());
}

void FlatFunDagCreator::apply(const DagExp* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Exp, vc.idx_);
}

void FlatFunDagCreator::apply(const DagLog* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Log, vc.idx_);
}

void FlatFunDagCreator::apply(const DagCos* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Cos, vc.idx_);
}

void FlatFunDagCreator::apply(const DagSin* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Sin, vc.idx_);
}

void FlatFunDagCreator::apply(const DagTan* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Tan, vc.idx_);
}

void FlatFunDagCreator::apply(const DagCosh* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Cosh, vc.idx_);
}

void FlatFunDagCreator::apply(const DagSinh* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Sinh, vc.idx_);
}

void FlatFunDagCreator::apply(const DagTanh* d)
{
   FlatFunDagCreator vc(f_);
   d->child()->acceptVisitor(vc);

   idx_ = f_->insertUnary(FlatSymbol::Tanh, vc.idx_);
}

void FlatFunDagCreator::apply(const DagLin* d)
{
   Interval cst = d->getCst();

   if (d->nbTerms() == 0)
   {
      idx_ = f_->insertCst(cst);
      return;
   }

   size_t jdx;

   // first linear term
   Variable v = d->varNode(0)->getVar();
   Interval x = d->coef(0),
            a = abs(x);

   size_t kdx = f_->insertLin(a, v);

   if (x.isPositive())
   {
      jdx = kdx;
   }
   else
   {
      jdx = f_->insertUnary(FlatSymbol::Usb, kdx);
   }

   // other linear terms
   for (size_t i=1; i<d->nbTerms(); ++i)
   {
      Variable v = d->varNode(i)->getVar();
      Interval x = d->coef(i),
               a = abs(x);

      size_t kdx = f_->insertLin(a, v);

      if (x.isPositive())
      {
         jdx = f_->insertBinary(FlatSymbol::Add, jdx, kdx);
      }
      else
      {
         jdx = f_->insertBinary(FlatSymbol::Sub, jdx, kdx);         
      }
   }

   // constant
   if (!cst.isZero())
   {
      size_t kdx = f_->insertCst(cst);
      jdx = f_->insertBinary(FlatSymbol::Add, jdx, kdx);
   }

   idx_ = jdx;
}

void FlatFunction::print(std::ostream& os) const
{
   for (size_t i=0; i<nb_; ++i)
   {
      os << i << ": ";

      if (symb_[i] == FlatSymbol::Cst)
      {
         os << cst_[arg_[i][1]];
      }
      else if (symb_[i] == FlatSymbol::Var)
      {
         os << var_[arg_[i][2]].getName();
      }
      else
      {
         os << symb_[i] << " ";
         for (size_t j=1; j<arg_[i][0]; ++j)
         {
            os << "(" << arg_[i][j] << ") ";
         }
      }

      os << std::endl;
   }
}

std::ostream& operator<<(std::ostream& os, const FlatFunction& f)
{
   f.print(os);
   return os;
}

} // namespace
