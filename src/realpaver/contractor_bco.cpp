// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/contractor_bco.hpp"

namespace realpaver {

BcoContractor::BcoContractor(Dag* dag, size_t i, const Variable& v,
                             const SharedContractor& op, 
                             const SharedBox& init) :
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

Proof BcoContractor::contract(Box& B)
{
   bool initLB = B[v_].left() == init_->operator[](v_).left(),
        initRB = B[v_].right() == init_->operator[](v_).right(),
        initB  = initLB || initRB;

   // just finds stationary points if B does not intersect
   // the initial region for this variable
   if (!initB)
   {
      return op_->contract(B);
   }

   // copies the box
   Box copy( B );

   // applies the contractor to find stationary points
   Proof proof = op_->contract(B);

   if (proof == Proof::Empty)
   {
      // monotone function => finds the sign of the derivative at some
      // point of B
      RealVector P( copy.midpoint() );
      Interval x = f_->eval(P);

      // resets B
      B.set(copy, scope());

      // instanciates the variable
      if (x.isCertainlyLeZero())
      {
         double val = copy[v_].right();
         B.set(v_, val);
      }

      else if (x.isCertainlyGeZero())
      {
         double val = copy[v_].left();
         B.set(v_, val);
      }

      else
         LOG("!!! unable to fix " << v_.name() << " but monotone");
   }
   else
   {
      bool keepLB = false,
           keepRB = false;

      if (initLB && B[v_].left() != copy[v_].left())
      {
         RealVector P( copy.lCorner() );
         Interval x = f_->eval(P);
         if (x.isCertainlyGeZero())
            keepLB = true;
      }

      if (initRB && B[v_].right() != copy[v_].right())
      {
         RealVector P( copy.rCorner() );
         Interval x = f_->eval(P);
         if (x.isCertainlyLeZero())
            keepRB = true;
      }

      if (keepLB || keepRB)
      {
         B.set(copy, scope());
         Interval x = B[v_];

         if (keepLB)
            x.setLeft(copy[v_].left()); 

         if (keepRB)
            x.setRight(copy[v_].right());

         B.set(v_, x);
      }
   }
   
   return Proof::Maybe;
}

void BcoContractor::print(std::ostream& os) const
{
   os << "BCO contractor " << f_->index() << " / " << v_.name();
}

} // namespace
