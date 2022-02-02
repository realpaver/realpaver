// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/contractor_bco.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

BcoContractor::BcoContractor(Dag* dag, size_t i, const Variable& v,
                             const SharedContractor& op, 
                             const SharedIntervalVector& init) :
   f_(dag->fun(i)),
   v_(v),
   op_(op),
   init_(init)
{}

bool BcoContractor::dependsOn(const Bitset& bs) const
{
   return f_->dependsOn(bs);
}

Scope BcoContractor::scope() const
{
   return f_->scope();
}

Proof BcoContractor::contract(IntervalVector& X)
{
   bool initLB = X[v_].left() == init_->operator[](v_).left(),
        initRB = X[v_].right() == init_->operator[](v_).right(),
        initB  = initLB || initRB;

   // just finds stationary points if B does not intersect
   // the initial region for this variable
   if (!initB)
   {
      return op_->contract(X);
   }

   // copies the box
   IntervalVector copy( X );

   // applies the contractor to find stationary points
   Proof proof = op_->contract(X);

   if (proof == Proof::Empty)
   {
      // monotone function => finds the sign of the derivative at some
      // point of B
      RealVector P( copy.midpoint() );
      Interval ef = f_->eval(P);

      // resets B
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

void BcoContractor::print(std::ostream& os) const
{
   os << "BCO contractor " << f_->index() << " / " << v_.getName();
}

} // namespace
