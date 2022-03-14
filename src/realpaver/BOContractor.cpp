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
                           SharedContractor op, 
                           SharedIntervalRegion init)
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

Proof BOContractor::contract(IntervalRegion& reg)
{
   // true if the region crosses the initial boundary at the left bound of v_
   bool initLB = reg.get(v_).left() == init_->get(v_).left();

   // true if the region crosses the initial boundary at the right bound of v_
   bool initRB = reg.get(v_).right() == init_->get(v_).right();

   // true if X crosses the initial boundary
   bool initB  = initLB || initRB;

   // just finds stationary points if X does not cross the initial region
   if (!initB)
   {
      return op_->contract(reg);
   }

   // copies the box
   IntervalRegion copy(reg);

   // applies the contractor to find stationary points
   Proof proof = op_->contract(reg);

   if (proof == Proof::Empty)
   {
      // monotone function => finds the sign of the derivative at some
      // point of the region
      RealPoint pt( copy.midpoint() );
      Interval ef = f_->eval(pt);

      // resets the region
      reg.setOnScope(copy, scope());

      // instanciates the variable
      if (ef.isCertainlyLeZero())
      {
         double val = copy.get(v_).right();
         reg.set(v_, val);
      }

      else if (ef.isCertainlyGeZero())
      {
         double val = copy.get(v_).left();
         reg.set(v_, val);
      }

      //else
         //LOG_INFO("!!! unable to fix " << v_.name() << " but monotone");
   }
   else
   {
      bool keepLB = false,
           keepRB = false;

      if (initLB && reg.get(v_).left() != copy.get(v_).left())
      {
         RealPoint lc( copy.lCorner() );
         Interval ef = f_->eval(lc);
         if (ef.isCertainlyGeZero()) keepLB = true;
      }

      if (initRB && reg.get(v_).right() != copy.get(v_).right())
      {
         RealPoint rc( copy.rCorner() );
         Interval ef = f_->eval(rc);
         if (ef.isCertainlyLeZero()) keepRB = true;
      }

      if (keepLB || keepRB)
      {
         reg.setOnScope(copy, scope());
         Interval dom = reg.get(v_);

         if (keepLB) dom.setLeft(copy.get(v_).left()); 
         if (keepRB) dom.setRight(copy.get(v_).right());
         reg.set(v_, dom);
      }
   }

   return Proof::Maybe;
}

void BOContractor::print(std::ostream& os) const
{
   os << "BO contractor " << f_->index() << " / " << v_.getName();
}

} // namespace
