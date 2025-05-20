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
 * @date   16/03/2025
 */

#include "realpaver/AffineCreator.hpp"
#include "realpaver/AssertDebug.hpp"

namespace realpaver {

AffineCreator::AffineCreator(SharedDag dag, bool minrange)
    : dag_(dag)
    , minrange_(minrange)
    , v_(dag_->nbNodes())
    , lfun_(dag->nbFuns())
{
   ASSERT(dag != nullptr, "Null pointer");

   for (size_t i = 0; i < dag->nbFuns(); ++i)
      lfun_[i] = i;
}

AffineCreator::AffineCreator(SharedDag dag, const IndexList &lfun, bool minrange)
    : dag_(dag)
    , minrange_(minrange)
    , v_(dag_->nbNodes())
    , lfun_(lfun)
{
   ASSERT(dag != nullptr, "Null pointer");
   ASSERT(!lfun.empty(), "Empty list of indexes");
}

AffineCreator::AffineCreator(SharedDag dag, size_t i, bool minrange)
    : dag_(dag)
    , minrange_(minrange)
    , v_(dag_->nbNodes())
    , lfun_(1)
{
   ASSERT(dag != nullptr, "Null pointer");
   ASSERT(i < dag->nbFuns(), "Bad function index in a DAG @ " << i);

   lfun_[0] = i;
}

size_t AffineCreator::nbFuns() const
{
   return lfun_.size();
}

const AffineForm &AffineCreator::fun(size_t i) const
{
   return *v_[dag_->fun(lfun_[i])->rootNode()->index()];
}

std::unique_ptr<AffineForm> AffineCreator::moveFun(size_t i)
{
   return std::move(v_[dag_->fun(lfun_[i])->rootNode()->index()]);
}

const AffineForm &AffineCreator::fun(const Variable &v) const
{
   return *v_[dag_->findVarNode(v.id())->index()];
}

void AffineCreator::useMinrange(bool minrange)
{
   minrange_ = minrange;
}

void AffineCreator::create(const IntervalBox &B)
{
   ASSERT(B.scope().contains(dag_->scope()), "Bad scopes");

   AffineForm::useMinrange(minrange_);

   if (lfun_.size() == dag_->nbFuns())
   {
      for (int i = 0; i < dag_->nbNodes(); ++i)
         makeNode(i, B);
   }
   else
   {
      for (int i = 0; i < lfun_.size(); ++i)
      {
         DagFun *fun = dag_->fun(lfun_[i]);
         for (size_t j = 0; j < fun->nbNodes(); ++j)
         {
            DagNode *node = fun->node(j);
            size_t k = node->index();
            if (v_[k] == nullptr)
               makeNode(k, B);
         }
      }
   }
}

void AffineCreator::makeNode(int i, const IntervalBox &B)
{
   DagNode *node = dag_->node(i);
   std::unique_ptr<AffineForm> f = nullptr;

   switch (node->symbol())
   {
   case DagSymbol::Cst: {
      f = std::make_unique<AffineForm>(static_cast<DagConst *>(node)->getConst());
      break;
   }
   case DagSymbol::Var: {
      const Variable &v = static_cast<DagVar *>(node)->getVar();
      f = std::make_unique<AffineForm>(dag_->scope().index(v), B.get(v));
      break;
   }
   case DagSymbol::Add: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t l = aux->left()->index(), r = aux->right()->index();
      f = std::make_unique<AffineForm>(*v_[l] + *v_[r]);
      break;
   }
   case DagSymbol::Sub: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t l = aux->left()->index(), r = aux->right()->index();
      f = std::make_unique<AffineForm>(*v_[l] - *v_[r]);
      break;
   }
   case DagSymbol::Mul: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t l = aux->left()->index(), r = aux->right()->index();
      f = std::make_unique<AffineForm>(*v_[l] * *v_[r]);
      break;
   }
   case DagSymbol::Div: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t l = aux->left()->index(), r = aux->right()->index();
      f = std::make_unique<AffineForm>(*v_[l] / *v_[r]);
      break;
   }
   case DagSymbol::Min: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t l = aux->left()->index(), r = aux->right()->index();
      f = std::make_unique<AffineForm>(min(*v_[l], *v_[r]));
      break;
   }
   case DagSymbol::Max: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t l = aux->left()->index(), r = aux->right()->index();
      f = std::make_unique<AffineForm>(max(*v_[l], *v_[r]));
      break;
   }
   case DagSymbol::Usb: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(-(*v_[c]));
      break;
   }
   case DagSymbol::Abs: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(abs(*v_[c]));
      break;
   }
   case DagSymbol::Sgn: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(sgn(*v_[c]));
      break;
   }
   case DagSymbol::Sqr: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(sqr(*v_[c]));
      break;
   }
   case DagSymbol::Sqrt: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(sqrt(*v_[c]));
      break;
   }
   case DagSymbol::Pow: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      int e = static_cast<DagPow *>(node)->exponent();
      f = std::make_unique<AffineForm>(pow(*v_[c], e));
      break;
   }
   case DagSymbol::Exp: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(exp(*v_[c]));
      break;
   }
   case DagSymbol::Log: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(log(*v_[c]));
      break;
   }
   case DagSymbol::Cos: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(cos(*v_[c]));
      break;
   }
   case DagSymbol::Sin: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(sin(*v_[c]));
      break;
   }
   case DagSymbol::Tan: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(tan(*v_[c]));
      break;
   }
   case DagSymbol::Cosh: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(cosh(*v_[c]));
      break;
   }
   case DagSymbol::Sinh: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(sinh(*v_[c]));
      break;
   }
   case DagSymbol::Tanh: {
      DagAdd *aux = static_cast<DagAdd *>(node);
      size_t c = aux->child()->index();
      f = std::make_unique<AffineForm>(tanh(*v_[c]));
      break;
   }
   case DagSymbol::Lin: {
      DagLin *aux = static_cast<DagLin *>(node);
      f = std::make_unique<AffineForm>(aux->getCst());

      // loop on the linear terms
      for (int j = 0; j < aux->nbTerms(); ++j)
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

} // namespace realpaver
