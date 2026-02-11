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
 * @file   AffineCreator.cpp
 * @brief  Creators of affine forms
 * @author Laurent Granvilliers
 * @date   16 Mar 2025
 */

#include "realpaver/AffineCreator.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

AffineCreator::AffineCreator(SharedDag dag, bool minrange)
    : dag_(dag)
    , minrange_(minrange)
    , v_(dag_->nbNodes())
    , lfun_()
{
   REALPAVER_ASSERT(dag != nullptr, "Null pointer");
}

size_t AffineCreator::nbFuns() const
{
   return lfun_.size();
}

bool AffineCreator::hasFun(size_t i) const
{
   return v_[dag_->fun(lfun_[i])->rootNode()->index()] != nullptr;
}

const AffineForm &AffineCreator::fun(size_t i) const
{
   return *v_[dag_->fun(lfun_[i])->rootNode()->index()];
}

std::unique_ptr<AffineForm> AffineCreator::moveFun(size_t i)
{
   return std::move(v_[dag_->fun(lfun_[i])->rootNode()->index()]);
}

bool AffineCreator::hasFun(const Variable &v) const
{
   return v_[dag_->findVarNode(v.id())->index()] != nullptr;
}

const AffineForm &AffineCreator::fun(const Variable &v) const
{
   return *v_[dag_->findVarNode(v.id())->index()];
}

const AffineForm &AffineCreator::funEpsilon(size_t i) const
{
   return fun(dag_->scope().var(i));
}

void AffineCreator::useMinrange(bool minrange)
{
   minrange_ = minrange;
}

bool AffineCreator::create(const IntervalBox &B, const Bitset &active)
{
   REALPAVER_ASSERT(B.scope().contains(dag_->scope()), "Bad scopes");

   AffineForm::useMinrange(minrange_);

   lfun_.clear();
   for (size_t i = 0; i < dag_->nbFuns(); ++i)
   {
      if (active.get(dag_->fun(i)->ctrId()))
      {
         bool ok = makeFun(B, i);
         if (!ok)
            return false;
         LOG_LOW("Affine relaxation for constraint @ " << dag_->fun(i)->ctrId());
      }
   }

   return true;
}

bool AffineCreator::create(const IntervalBox &B, size_t i)
{
   REALPAVER_ASSERT(B.scope().contains(dag_->scope()), "Bad scopes");
   REALPAVER_ASSERT(i < dag_->nbFuns(), "Bad index of function @ " << i);

   AffineForm::useMinrange(minrange_);

   lfun_.clear();
   return makeFun(B, i);
}

bool AffineCreator::makeFun(const IntervalBox &B, size_t i)
{
   DagFun *f = dag_->fun(i);
   for (size_t j = 0; j < f->nbNodes(); ++j)
   {
      DagNode *node = f->node(j);
      size_t k = node->index();
      if (v_[k] == nullptr)
      {
         try
         {
            makeNode(k, B);
         }
         catch (Exception &e)
         {
            LOG_LOW("Affine form of node " << k << " invalid");
            return false;
         }
      }
   }
   lfun_.push_back(i);
   return true;
}

size_t AffineCreator::dagIndex(size_t i) const
{
   REALPAVER_ASSERT(i < lfun_.size(), "Bad access in lfun");
   return lfun_[i];
}

void AffineCreator::makeNode(int i, const IntervalBox &B)
{
   DagNode *node = dag_->node(i);
   std::unique_ptr<AffineForm> f = nullptr;

   switch (node->symbol())
   {
   case DagSymbol::Cst: {
      f = std::make_unique<AffineForm>(static_cast<DagConst *>(node)->getValue());
      break;
   }
   case DagSymbol::Var: {
      const Variable &v = static_cast<DagVar *>(node)->getVar();
      f = std::make_unique<AffineForm>(dag_->scope().index(v), B.get(v));
      break;
   }
   case DagSymbol::Add: {
      size_t l = node->left()->index(), r = node->right()->index();
      f = std::make_unique<AffineForm>(*v_[l] + *v_[r]);
      break;
   }
   case DagSymbol::Sub: {
      size_t l = node->left()->index(), r = node->right()->index();
      f = std::make_unique<AffineForm>(*v_[l] - *v_[r]);
      break;
   }
   case DagSymbol::Mul: {
      size_t l = node->left()->index(), r = node->right()->index();
      f = std::make_unique<AffineForm>(*v_[l] * *v_[r]);
      break;
   }
   case DagSymbol::Div: {
      size_t l = node->left()->index(), r = node->right()->index();
      f = std::make_unique<AffineForm>(*v_[l] / *v_[r]);
      break;
   }
   case DagSymbol::Min: {
      size_t l = node->left()->index(), r = node->right()->index();
      f = std::make_unique<AffineForm>(min(*v_[l], *v_[r]));
      break;
   }
   case DagSymbol::Max: {
      size_t l = node->left()->index(), r = node->right()->index();
      f = std::make_unique<AffineForm>(max(*v_[l], *v_[r]));
      break;
   }
   case DagSymbol::Usb: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(-(*v_[c]));
      break;
   }
   case DagSymbol::Abs: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(abs(*v_[c]));
      break;
   }
   case DagSymbol::Sgn: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(sgn(*v_[c]));
      break;
   }
   case DagSymbol::Sqr: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(sqr(*v_[c]));
      break;
   }
   case DagSymbol::Sqrt: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(sqrt(*v_[c]));
      break;
   }
   case DagSymbol::Pow: {
      size_t c = node->child()->index();
      int e = static_cast<DagPow *>(node)->exponent();
      f = std::make_unique<AffineForm>(pow(*v_[c], e));
      break;
   }
   case DagSymbol::Exp: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(exp(*v_[c]));
      break;
   }
   case DagSymbol::Log: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(log(*v_[c]));
      break;
   }
   case DagSymbol::Cos: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(cos(*v_[c]));
      break;
   }
   case DagSymbol::Sin: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(sin(*v_[c]));
      break;
   }
   case DagSymbol::Tan: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(tan(*v_[c]));
      break;
   }
   case DagSymbol::Cosh: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(cosh(*v_[c]));
      break;
   }
   case DagSymbol::Sinh: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(sinh(*v_[c]));
      break;
   }
   case DagSymbol::Tanh: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(tanh(*v_[c]));
      break;
   }
   case DagSymbol::Acos: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(acos(*v_[c]));
      break;
   }
   case DagSymbol::Asin: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(asin(*v_[c]));
      break;
   }
   case DagSymbol::Atan: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(atan(*v_[c]));
      break;
   }
   case DagSymbol::Acosh: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(acosh(*v_[c]));
      break;
   }
   case DagSymbol::Asinh: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(asinh(*v_[c]));
      break;
   }
   case DagSymbol::Atanh: {
      size_t c = node->child()->index();
      f = std::make_unique<AffineForm>(atanh(*v_[c]));
      break;
   }
   case DagSymbol::Lin: {
      DagLin *aux = static_cast<DagLin *>(node);
      f = std::make_unique<AffineForm>(aux->getCst());

      // loop on the linear terms
      for (size_t j = 0; j < aux->nbTerms(); ++j)
      {
         // affine form of the variable in this term
         const AffineForm &g = *v_[aux->varNode(j)->index()];

         // adds the affine form of this term in f
         f->push(AffineForm(aux->coef(j)) * g);
      }
      break;
   }
   }
   v_[i] = std::move(f);
}

Term AffineCreator::funToTerm(size_t i) const
{
   AffineForm &f = const_cast<AffineForm &>(fun(i));
   Interval x = f.constantTerm();
   Term res(0);
   for (auto it = f.begin(); it != f.end(); ++it)
   {
      Variable v = dag_->scope().var(AffineForm::var(it));
      AffineForm &g = const_cast<AffineForm &>(fun(v));

      Interval c = AffineForm::itv(it), a = g.constantTerm(),
               b = AffineForm::itv(g.begin()), d = c / b;
      x += -a * d;
      res += d * v;
   }
   res += x + f.errorTerm() * Interval::minusOnePlusOne();
   return res;
}

Variable AffineCreator::varToVar(size_t i) const
{
   return dag_->scope().var(i);
}

} // namespace realpaver
