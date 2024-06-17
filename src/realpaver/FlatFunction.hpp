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

#include "realpaver/Term.hpp"

namespace realpaver {

class DagFun;

class FlatFunction {
public:
   FlatFunction(const Term& t, const Interval& img);

   ~FlatFunction();

   Interval eval(const IntervalBox& B) const;

   size_t insertCst(const Interval& val);
   size_t insertVar(const Variable& v);
   size_t insertUnary(NodeSymbol symb, size_t ic);
   size_t insertBinary(NodeSymbol symb, size_t il, size_t ir);

private:
   Scope scop_;
   Interval img_;

   NodeSymbol* symb_;
   size_t nb_, capa_;

   size_t** arg_;

   Interval* itv_;
};

/*----------------------------------------------------------------------------*/

class FlatFunctionCreator : public TermVisitor {
public:
   /// Creates a deriver with respect to v
   FlatFunctionCreator(FlatFunction* f);

   void apply(const TermCst* t) override;
   void apply(const TermVar* t) override;
   void apply(const TermAdd* t) override;
   void apply(const TermSub* t) override;
   void apply(const TermMul* t) override;
   void apply(const TermDiv* t) override;
   void apply(const TermMin* t) override;
   void apply(const TermMax* t) override;
   void apply(const TermUsb* t) override;
   void apply(const TermAbs* t) override;
   void apply(const TermSgn* t) override;
   void apply(const TermSqr* t) override;
   void apply(const TermSqrt* t) override;
   void apply(const TermPow* t) override;
   void apply(const TermExp* t) override;
   void apply(const TermLog* t) override;
   void apply(const TermCos* t) override;
   void apply(const TermSin* t) override;
   void apply(const TermTan* t) override;
   void apply(const TermCosh* t) override;
   void apply(const TermSinh* t) override;
   void apply(const TermTanh* t) override;

private:
   FlatFunction* f_;
   size_t idx_;
};

} // namespace

#endif
