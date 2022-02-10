///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOContractor.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

BOContractor::BOContractor(Dag* dag, size_t i, const Variable& v,
                           const SharedContractor& op, 
                           const SharedIntervalVector& init)
      : f_(dag->fun(i)),
        v_(v),
        op_(op),
        init_(init)
{}

bool BOContractor::dependsOn(const Bitset& bs) const
{
   return f_->dependsOn(bs);
}

Scope BOContractor::scope() const
{
   return f_->scope();
}

Proof BOContractor::contract(IntervalVector& X)
{
   // true if X  crosses the initial boundary at the left bound of v_
   bool initLB = X[v_].left() == init_->operator[](v_).left();

   // true if X crosses the initial boundary at the right bound of v_
   bool initRB = X[v_].right() == init_->operator[](v_).right();

   // true if X crosses the initial boundary
   bool initB  = initLB || initRB;

   // just finds stationary points if X does not cross the initial region
   if (!initB)
   {
      return op_->contract(X);
   }

   // copies the box
   IntervalVector copy(X);

   // applies the contractor to find stationary points
   Proof proof = op_->contract(X);

   if (proof == Proof::Empty)
   {
      // monotone function => finds the sign of the derivative at some
      // point of X
      RealVector P( copy.midpoint() );
      Interval ef = f_->eval(P);

      // resets X
      X.setOnScope(copy, scope());

      // instanciates the variable
      if (ef.isCertainlyLeZero())
      {
         double val = copy[v_].right();
         X.set(v_, val);
      }

      else if (ef.isCertainlyGeZero())
      {
         double val = copy[v_].left();
         X.set(v_, val);
      }

      //else
         //LOG_INFO("!!! unable to fix " << v_.name() << " but monotone");
   }
   else
   {
      bool keepLB = false,
           keepRB = false;

      if (initLB && X[v_].left() != copy[v_].left())
      {
         RealVector P( copy.lCorner() );
         Interval ef = f_->eval(P);
         if (ef.isCertainlyGeZero())
            keepLB = true;
      }

      if (initRB && X[v_].right() != copy[v_].right())
      {
         RealVector P( copy.rCorner() );
         Interval ef = f_->eval(P);
         if (ef.isCertainlyLeZero())
            keepRB = true;
      }

      if (keepLB || keepRB)
      {
         X.setOnScope(copy, scope());
         Interval dom = X[v_];

         if (keepLB)
            dom.setLeft(copy[v_].left()); 

         if (keepRB)
            dom.setRight(copy[v_].right());

         X.set(v_, dom);
      }
   }
   
   return Proof::Maybe;
}

void BOContractor::print(std::ostream& os) const
{
   os << "BO contractor " << f_->index() << " / " << v_.getName();
}

} // namespace
