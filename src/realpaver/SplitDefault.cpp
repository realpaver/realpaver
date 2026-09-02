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
 * @file   SplitDefault.cpp
 * @brief  Default splitting strategies
 * @author Laurent Granvilliers
 * @date   29 April 2026
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Exception.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/SplitDefault.hpp"

namespace realpaver {

SplitDefault::SplitDefault(Scope scop, std::unique_ptr<DomainSlicerMap> smap,
                           std::unique_ptr<Selector> slc, std::unique_ptr<Selector> sld)
    : Split(scop, std::move(smap))
    , slc_(std::move(slc))
    , sld_(std::move(sld))
    , cycleLength_(0)
    , cycleLengthRR_(0)
    , discreteFirst_(false)
    , nbRR_(0)
    , nb_(0)
{
   THROW_IF(slc_ == nullptr, "Bad selector");
   THROW_IF(sld_ == nullptr, "Bad selector");
   THROW_IF(scop.isEmpty(), "Bad scopes");

   double hybrid_rr = Params::GetDblParam("SPLIT_DEFAULT_RR_CYCLE");
   double hybrid_spe = Params::GetDblParam("SPLIT_DEFAULT_SPECIFIC_CYCLE");

   THROW_IF(hybrid_rr < 0.0, "Bad parameter value SPLIT_DEFAULT_RR_CYCLE");
   THROW_IF(hybrid_spe < 0.0, "Bad parameter value SPLIT_DEFAULT_SPECIFIC_CYCLE");

   discreteFirst_ = (Params::GetStrParam("SPLIT_DISCRETE_FIRST") == "YES");

   int nb_rr = hybrid_rr * scop.size();
   int nb_spe = hybrid_spe * scop.size();

   if (nb_rr + nb_spe == 0)
   {
      nb_rr = 1;
   }

   setCycleLengthRR(nb_rr);
   setCycleLength(nb_rr + nb_spe);
}

int SplitDefault::cycleLength() const
{
   return cycleLength_;
}

void SplitDefault::setCycleLength(int len)
{
   REALPAVER_ASSERT(len >= 0, "Bad length of cycle");
   cycleLength_ = len;
}

int SplitDefault::cycleLengthRR() const
{
   return cycleLengthRR_;
}

void SplitDefault::setCycleLengthRR(int len)
{
   REALPAVER_ASSERT(len >= 0, "Bad length of cycle");
   cycleLengthRR_ = len;
}

void SplitDefault::setDiscreteFirst(bool ok)
{
   discreteFirst_ = ok;
}

std::pair<bool, Variable> SplitDefault::selectRR(SharedNode node, NodeContext &context)
{
   DomainBox *box = node->box();

   std::shared_ptr<NodeInfo> info =
       context.getInfo(node->index(), NodeInfoType::SplitVar);

   // assigns an iterator pointing to the next variable
   Scope::const_iterator it;
   if (info == nullptr)
   {
      it = scop_.begin();
   }
   else
   {
      NodeInfoVar *infovar = static_cast<NodeInfoVar *>(info.get());
      it = scop_.find(infovar->getVar());
      ++it;
      if (it == scop_.end())
         it = scop_.begin();
   }

   bool found = false;
   size_t nb = 0;

   while (!found && nb < scop_.size())
   {
      if (box->isSplitable(*it))
      {
         found = true;
      }
      else
      {
         ++nb;
         ++it;
         if (it == scop_.end())
            it = scop_.begin();
      }
   }

   return std::make_pair(found, *it);
}

void SplitDefault::applyImpl(SharedNode node, NodeContext &context)
{
   Variable v;
   bool found = false;

   if (nb_ % cycleLength_ == 0)
   {
      nbRR_ = cycleLengthRR_;
   }
   ++nb_;

   if (nbRR_ > 0)
   {
      --nbRR_;
      auto p = selectRR(node, context);
      found = p.first;
      if (found)
         v = p.second;
   }
   else
   {
      if (sld_->scope().isEmpty())
      {
         // selects a continuous variable
         found = slc_->apply(node, context);
         if (found)
            v = slc_->getSelectedVar();
      }
      else if (slc_->scope().isEmpty())
      {
         // selects a discrete variable
         found = sld_->apply(node, context);
         if (found)
            v = sld_->getSelectedVar();
      }
      else
      {
         // selects between a continuous variable and a discrete variable
         bool continuous = (!discreteFirst_) && (node->index() % 2 == 0);

         if (continuous)
         {
            // selects a continuous variable
            found = slc_->apply(node, context);
            if (found)
               v = slc_->getSelectedVar();
         }
         else
         {
            // selects a discrete variable
            found = sld_->apply(node, context);
            if (found)
               v = sld_->getSelectedVar();
         }

         // if no variable has been found by a selector, applies the other one
         if (!found)
         {
            if (continuous)
            {
               found = sld_->apply(node, context);
               if (found)
                  v = sld_->getSelectedVar();
            }
            else
            {
               found = slc_->apply(node, context);
               if (found)
                  v = slc_->getSelectedVar();
            }
         }
      }
   }

   if (found)
   {
      // splits the variable domain
      splitOne(node, v);

      LOG_INTER("Hybrid split selects " << v.getName() << " in node " << node->index());

      if (getNbNodes() < 2)
         return;

      // assigns the split variable in the sub-nodes
      std::shared_ptr<NodeInfoVar> info = std::make_shared<NodeInfoVar>(v);

      for (SharedNode &aux : cont_)
         context.insert(aux->index(), info);
   }
}

} // namespace realpaver
