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
 * @file   Contractor3BCID.cpp
 * @brief  3BCID contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorVar3BCID.hpp"
#include "realpaver/IntervalSlicer.hpp"

namespace realpaver {

ContractorVar3BCID::ContractorVar3BCID(SharedContractor op, Variable v,
                                       size_t ns3B, size_t nsCID)
      : op_(op),
        v_(v),
        ns3B_(ns3B),
        nsCID_(nsCID),
        varMnWidth_(Param::GetDblParam("VAR3BCID_MIN_WIDTH"))
{
   ASSERT(ns3B >= 2, "Bas number of slices in a var3BCID contractor");
   ASSERT(nsCID >= 2, "Bas number of slices in a var3BCID contractor");
}

Variable ContractorVar3BCID::getVar() const
{
   return v_;
}

void ContractorVar3BCID::setVar(Variable v)
{
   ASSERT(scope().contains(v),
          "Bad variable " << v << " in a var3BCID contractor");

   v_ = v;
}

Scope ContractorVar3BCID::scope() const
{
   return op_->scope();
}

double ContractorVar3BCID::varMinWidth() const
{
   return varMnWidth_;
}

void ContractorVar3BCID::setVarMinWidth(double val)
{
   varMnWidth_ = val;
}

Proof ContractorVar3BCID::contract(IntervalBox& B)
{
   Interval dom = B.get(v_);

   // not handles too small domains
   if (dom.width()<varMnWidth_) return Proof::Maybe;

   // not handles infinite domains
   if (dom.isInf()) return Proof::Maybe;

   // width of slices
   double w3B = dom.width() / ns3B_;
   size_t n3B = ns3B_;

   // not handles too small slices
   if (w3B < varMnWidth_)
   {
      n3B = (size_t)(dom.width()/varMnWidth_);
      if (n3B < 2) n3B = 2;
   }

//~ std::cout << "n3B : " << n3B << std::endl;

   // 3B
   IntervalBox save(B);
   IntervalPartitionMaker slicer(n3B);
   slicer.apply(dom);
   n3B = slicer.nbSlices();

   // left to right
   size_t i = 0;
   bool leftFound = false;
   Interval leftSlice;

   while (!leftFound && (i<n3B))
   {
      if (i>0) B = save;
      leftSlice = slicer.slice(i);

//~ std::cout << "left " << i << " : " << leftSlice << std::endl;      
      
      B.set(v_, leftSlice);
      Proof certif = op_->contract(B);

      if (certif == Proof::Empty)
      {

//~ std::cout << "    EMPTY" << std::endl;

         ++i;
      }
      else
      {

//~ std::cout << "    LEFT " << B << std::endl;

         leftFound = true;
      }
   }

   if (!leftFound)
   {

//~ std::cout << "RESULT : EMPTY" << std::endl;

      return Proof::Empty;
   }

   // right to left
   size_t j = n3B-1;

   // the left consistent slice is also the rightmost one
   // B corresponds to this reduced slice
   if (i == j)
   {

//~ std::cout << "RESULT : ONLY LEFT SLICE " << B << std::endl;

      return Proof::Maybe;
   }

   bool rightFound = false;
   Interval rightSlice;

   // newbox := last reduced slice
   IntervalBox newbox(B);

   while (!rightFound && (j > i))
   {
      B = save;
      rightSlice = slicer.slice(j);

//~ std::cout << "right " << j << " : " << rightSlice << std::endl;

      B.set(v_, rightSlice);
      Proof certif = op_->contract(B);

      if (certif == Proof::Empty)
      {

//~ std::cout << "    EMPTY" << std::endl;

         --j;
      }
      else
      {

//~ std::cout << "    RIGHT " << B << std::endl;

         rightFound = true;
      }
   }

   if (!rightFound)
   {

//~ std::cout << "NO RIGHT FOUND " << std::endl;

      // only one consistent facet (the one found from left to right)
      B = newbox;

//~ std::cout << "RESULT " << B << std::endl;

      return Proof::Maybe;
   }

   if (i == j-1)
   {
      // only two consistent facets
      B.glue(newbox);

//~ std::cout << "TWO FOUND " << B << std::endl;

      return Proof::Maybe;
   }

   // save the hull of the consistent facets
   newbox.glue(B);

   // varCID on the remaining box, use savebox
   save.set(v_, Interval(leftSlice.right(), rightSlice.left()));
   bool reduced = contractCID(save, newbox);

   if (reduced)
   {
      B = newbox;
   }
   else
   {
      B = save;
      B.set(v_, leftSlice | rightSlice);
   }

   return Proof::Maybe;
}

bool ContractorVar3BCID::contractCID(IntervalBox& Bcid, IntervalBox& B3bCid)
{
   if (nsCID_ == 0 || Bcid.equals(B3bCid, v_))
      return false;

//~ std::cout << "contract CID on Bcid " << Bcid << std::endl;
//~ std::cout << "with B3bCid " << B3bCid << std::endl;



   IntervalBox B(Bcid);
   Interval dom(Bcid.get(v_));

   IntervalPartitionMaker slicer(nsCID_);
   slicer.apply(dom);
   size_t nCID = slicer.nbSlices();

   for (size_t i=0; i<nCID; ++i)
   {
      if (i>0) B = Bcid;
      B.set(v_, slicer.slice(i));

//~ std::cout << "to be cided " << B << std::endl;

      Proof certif = op_->contract(B);
      
      if (certif != Proof::Empty)
      {

//~ std::cout << "    result " << B << std::endl;

         B3bCid.glue(B);

//~ std::cout << "    glue " << B3bCid << std::endl;
         
         if (Bcid.equals(B3bCid, v_))
            return false;
      }
   }
   return true;
}

void ContractorVar3BCID::print(std::ostream& os) const
{
   os << "var3BCID contractor on " << v_.getName();
}
   
} // namespace
