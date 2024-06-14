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

FlatFunction::FlatFunction(DagFun* f) : scop_(f->scope())
{
   capa_ = 8;
   symb_ = new NodeSymbol[capa_];
   nb_ = 0;

   arg_ = new size_t[capa_];
   child_ = new size_t*[capa_];
   var_ = new size_t[capa_];

   makeFun(f);
   
   itv_ = new Interval[nb_];
}

void FlatFunction::makeFun(DagFun* f)
{
   img_ = f->getImage();

   for (size_t i=0; i<f->nbNodes(); ++i)
   {
      DagNode* node = f->node(i);

      if (capa_ == nb_)
      {
         extendCapacity();
      }

      symb_[nb_] = node->symbol();

      switch(node->symbol())
      {
         case NodeSymbol::Cst:
            arg_[nb_] = 0;
            break;

         case NodeSymbol::Var:
            break;

         case NodeSymbol::Add:
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

         case NodeSymbol::Lin:
            break;

         case NodeSymbol::Cosh:
            break;

         case NodeSymbol::Sinh:
            break;

         case NodeSymbol::Tanh:
            break;

      }
   }
}

Interval FlatFunction::eval(const IntervalBox& B) const
{
   for (size_t i=0; i<nb_; ++i)
   {
      switch(symb_[nb_])
      {
         case NodeSymbol::Cst:
            arg_[nb_] = 0;
            break;

         case NodeSymbol::Var:
            break;

         case NodeSymbol::Add:
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

         case NodeSymbol::Lin:
            break;

         case NodeSymbol::Cosh:
            break;

         case NodeSymbol::Sinh:
            break;

         case NodeSymbol::Tanh:
            break;

      }
   }

   return itv_[nb_];
}

void FlatFunction::extendCapacity()
{
   
}

} // namespace
