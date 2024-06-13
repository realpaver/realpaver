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
 * @file   FlatFunction.hpp
 * @brief  Function with fast hc4Revise
 * @author Laurent Granvilliers
 * @date   2024-6-11
*/

#ifndef REALPAVER_FLAT_FUNCTION_HPP
#define REALPAVER_FLAT_FUNCTION_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/Term.hpp"

namespace realpaver {

class DagFun;

class FlatFunction : public Contractor {
public:
   FlatFunction(DagFun* f);

private:
   Scope scop_;

   NodeSymbol* symb_;
   size_t capa_;
   size_t nb_;

   size_t* arg_;
   size_t** child_;
   size_t* var_;

   Interval img_;
   Interval* val_;
   Interval* dom_;

   void makeFun(DagFun* f);
   void extendCapacity();
};

} // namespace

#endif
