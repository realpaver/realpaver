///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Double.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Reformulation.hpp"

namespace realpaver {

DagNode::DagNode(Dag* dag, size_t index)
      : bitset_(),
        dag_(dag),
        index_(index),
        vpar_(),
        vsub_(),
        val_(),
        dv_(),
        rval_(0.0),
        rdv_(0.0),
        ilv_(-1)
{}

DagNode::~DagNode()
{}

DagNode* DagNode::left() const
{
   return dag_->node(vsub_[0]);
}

DagNode* DagNode::right() const
{
   return dag_->node(vsub_[1]);
}

DagNode* DagNode::child() const
{
   return dag_->node(vsub_[0]);
}

DagNode* DagNode::subNode(size_t i) const
{
   return dag_->node(vsub_[i]);
}

Interval DagNode::dom() const
{
   return dag_->dom(index_);
}

void DagNode::setDom(const Interval& x)
{
   dag_->setDom(index_, x);
}

void DagNode::reduceDom(const Interval& x)
{
   dag_->reduceDom(index_, x);
}

size_t DagNode::index() const
{
   return index_;
}

void DagNode::setIndex(size_t i)
{
   index_ = i;
}

size_t DagNode::parArity() const
{
   return vpar_.size();
}

size_t DagNode::parNode(size_t i) const
{
   return vpar_[i];
}

void DagNode::addParNode(size_t i)
{
   vpar_.push_back(i);
}

size_t DagNode::subArity() const
{
   return vsub_.size();   
}

size_t DagNode::subIndex(size_t i) const
{
   return vsub_[i];
}

void DagNode::addSubNode(size_t i)
{
   vsub_.push_back(i);
}

Dag* DagNode::dag() const
{
   return dag_;
}

const Bitset& DagNode::bitset() const
{
   return bitset_;
}

bool DagNode::dependsOn(const Bitset& bs) const
{
   return bitset_.overlaps(bs);
}

bool DagNode::dependsOn(const Variable& v) const
{
   return bitset_.get(v.id());
}

int DagNode::indexLinVar() const
{
   return ilv_;
}

void DagNode::setIndexLinVar(int i)
{
   ilv_ = i;
}

Interval DagNode::val() const
{
   return val_;
}

void DagNode::setVal(const Interval& x)
{
   val_ = x;
}

Interval DagNode::dv() const
{
   return dv_;
}

void DagNode::setDv(const Interval& x)
{
   dv_ = x;
}

void DagNode::addDv(const Interval& x)
{
   dv_ += x;
}

double DagNode::rval() const
{
   return rval_;
}

void DagNode::setRval(double x)
{
   rval_ = x;
}

double DagNode::rdv() const
{
   return rdv_;
}

void DagNode::setRdv(double x)
{
   rdv_ = x;
}

void DagNode::addRdv(double x)
{
   rdv_ = Double::add(rdv_, x);
}

void DagNode::linearize(LPModel& lm)
{
   ASSERT(!val_.isEmpty(), "Linearization of a DAG node with an empty domain");

   // creation of a linear variable for this node
   LinVar v = lm.makeVar(val_.left(), val_.right());
   setIndexLinVar(v.getIndex());

   if (LOG_ON)
   {
      std::ostringstream os;
      os << "x" << v.getIndex();
      v.setName(os.str());
   }

   // insertion of constraints
   if (val_.isFinite() && (!val_.isSingleton()))
      linearizeImpl(lm);
}

///////////////////////////////////////////////////////////////////////////////

DagConst::DagConst(Dag* dag, size_t index, const Interval& x)
      : DagNode(dag, index),
        x_(x)
{}

void DagConst::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagConst::print(std::ostream& os) const
{
   if (x_.isSingleton())
      os << x_.left();
   else
      os << x_;
}

void DagConst::eval(const IntervalRegion& reg)
{
   setVal(x_);
}

void DagConst::eval(const RealPoint& pt)
{
   setVal(x_);
}

void DagConst::evalOnly(const Variable& v, const Interval& x)
{
   setVal(x_);
}

void DagConst::proj(IntervalRegion& reg)
{
   // nothing to do
}

bool DagConst::diff()
{
   return true;
}

bool DagConst::diffOnly(const Variable& v)
{
   return true;
}

void DagConst::reval(const RealPoint& pt)
{
   setRval(x_.midpoint());
}

bool DagConst::rdiff()
{
   return true;
}

Interval DagConst::getConst() const
{
   return x_;
}

void DagConst::linearizeImpl(LPModel& lm)
{
   // nothing to do
}

///////////////////////////////////////////////////////////////////////////////

DagVar::DagVar(Dag* dag, size_t index, Variable v)
      : DagNode(dag, index),
        v_(v)
{
   bitset_ = Bitset(1 + v.id(), 0);
   bitset_.setOne(v.id());
}

void DagVar::print(std::ostream& os) const
{
   os << v_.getName();
}

void DagVar::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagVar::eval(const IntervalRegion& reg)
{
   setVal(reg.get(v_));
}

void DagVar::eval(const RealPoint& pt)
{
   setVal(pt.get(v_));
}

void DagVar::evalOnly(const Variable& v, const Interval& x)
{
   if (v_.id() == v.id())
      setVal(x);
}

void DagVar::proj(IntervalRegion& reg)
{
   reg.set(v_, reg.get(v_) & dom());
}

bool DagVar::diff()
{
   return true;
}

bool DagVar::diffOnly(const Variable& v)
{
   return true;
}

void DagVar::reval(const RealPoint& pt)
{
   setRval(pt.get(v_));
}

bool DagVar::rdiff()
{
   return true;
}

Variable DagVar::getVar() const
{
   return v_;
}

void DagVar::linearizeImpl(LPModel& lm)
{
   if (v_.isDiscrete())
   {
      LinVar lv = lm.getLinVar(indexLinVar());
      lv.setInteger();
   }
}

///////////////////////////////////////////////////////////////////////////////

DagOp::DagOp(Dag* dag, OpSymbol symb, const IndexList& lsub)
      : DagNode(dag),
        symb_(symb)
{
   for (size_t i : lsub)
   {
      addSubNode(i);
      bitset_ |= dag->node(i)->bitset();
   }
}

DagOp::~DagOp()
{}

void DagOp::setIndex(size_t index)
{
   DagNode::setIndex(index);

   for (size_t i=0; i<subArity(); ++i)
      subNode(i)->addParNode(index);
}

bool DagOp::eqSymbol(const DagOp* other) const
{
   return symb_ == other->symb_;
}

bool DagOp::eq(const DagOp* other) const
{
   if (!eqSymbol(other))
      return false;

   if (subArity() != other->subArity())
      return false;

   for (size_t i=0; i<subArity(); ++i)
      if (subIndex(i) != other->subIndex(i))
         return false;

   return true;
}

void DagOp::print(std::ostream& os) const
{
   os << symb_;
}

void DagOp::evalOnly(const Variable& v, const Interval& x)
{
   if (dependsOn(v))
   {
      for (size_t i=0; i<subArity(); ++i)
         subNode(i)->evalOnly(v, x);

      eval();
   }
}

bool DagOp::diffOnly(const Variable& v)
{
   bool res = true;

   if (dependsOn(v))
   {
      res = diff();

      for (size_t i=0; i<subArity(); ++i)
         res = res && subNode(i)->diffOnly(v);
   }

   return res;
}

OpSymbol DagOp::getSymbol() const
{
   return symb_;
}

void DagOp::eval(const IntervalRegion& reg)
{
   eval();
}

void DagOp::eval(const RealPoint& pt)
{
   eval();
}

void DagOp::reval(const RealPoint& pt)
{
   reval();
}

///////////////////////////////////////////////////////////////////////////////

DagAdd::DagAdd(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Add, lsub)
{}

void DagAdd::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagAdd::eval()
{
   setVal(left()->val()+right()->val());
}

void DagAdd::proj(IntervalRegion& reg)
{
   left()->reduceDom(addPX(left()->val(), right()->val(), dom()));
   right()->reduceDom(addPY(left()->val(), right()->val(), dom()));
}

bool DagAdd::diff()
{
   // d(l+r)/dl = 1, d(l+r)/dr = 1
   left()->addDv(dv());
   right()->addDv(dv());

   return true;
}

void DagAdd::reval()
{
   setRval(Double::add(left()->rval(), right()->rval()));
}

bool DagAdd::rdiff()
{
   // d(l+r)/dl = 1, d(l+r)/dr = 1
   left()->addRdv(rdv());
   right()->addRdv(rdv());

   return true;
}

void DagAdd::linearizeImpl(LPModel& lm)
{
   LinVar z = lm.getLinVar(indexLinVar()),
          x = lm.getLinVar(left()->indexLinVar()),
          y = lm.getLinVar(right()->indexLinVar());

   // z = x + y => z - x - y = 0
   LinExpr e( {1.0, -1.0, -1.0}, {z, x, y} );
   lm.addCtr(0.0, e, 0.0);
}

///////////////////////////////////////////////////////////////////////////////

DagSub::DagSub(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Sub, lsub)
{}

void DagSub::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSub::eval()
{
   setVal(left()->val()-right()->val());
}

void DagSub::proj(IntervalRegion& reg)
{
   left()->reduceDom(subPX(left()->val(), right()->val(), dom()));
   right()->reduceDom(subPY(left()->val(), right()->val(), dom()));
}

bool DagSub::diff()
{
   // d(l+r)/dl = 1, d(l+r)/dr = -1
   left()->addDv(dv());
   right()->addDv(-dv());

   return true;
}

void DagSub::reval()
{
   setRval(Double::sub(left()->rval(), right()->rval()));
}

bool DagSub::rdiff()
{
   // d(l+r)/dl = 1, d(l+r)/dr = -1
   left()->addRdv(rdv());
   right()->addRdv(-rdv());

   return true;
}

void DagSub::linearizeImpl(LPModel& lm)
{
   LinVar z = lm.getLinVar(indexLinVar()),
          x = lm.getLinVar(left()->indexLinVar()),
          y = lm.getLinVar(right()->indexLinVar());

   // z = x - y => z - x + y = 0
   LinExpr e( {1.0, -1.0, 1.0}, {z, x, y} );
   lm.addCtr(0.0, e, 0.0);
}

///////////////////////////////////////////////////////////////////////////////

DagMul::DagMul(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Mul, lsub)
{}

void DagMul::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagMul::eval()
{
   setVal(left()->val()*right()->val());
}

void DagMul::proj(IntervalRegion& reg)
{
   left()->reduceDom(mulPX(left()->val(), right()->val(), dom()));
   right()->reduceDom(mulPY(left()->val(), right()->val(), dom()));
}

bool DagMul::diff()
{
   // d(l*r)/dl = r, d(l*r)/dr = l
   left()->addDv(dv()*right()->val());
   right()->addDv(dv()*left()->val());

   return true;
}

void DagMul::reval()
{
   setRval(Double::mul(left()->rval(), right()->rval()));
}

bool DagMul::rdiff()
{
   // d(l*r)/dl = r, d(l*r)/dr = l
   left()->addRdv(Double::mul(rdv(), right()->rval()));
   right()->addRdv(Double::mul(rdv(), left()->rval()));

   return true;
}

void DagMul::linearizeImpl(LPModel& lm)
{
   LinVar z = lm.getLinVar(indexLinVar()),
          x = lm.getLinVar(left()->indexLinVar()),
          y = lm.getLinVar(right()->indexLinVar());

   // z = x*y, a <= x <= b, c <= y <= d
   double a = left()->val().left(),
          b = left()->val().right(),
          c = right()->val().left(),
          d = right()->val().right();

   bool xvar = (a != b),   // left subterm not fixed?
        yvar = (c != d);   // right subterm not fixed?

   if (xvar && yvar)
   {
      // Mc Cormick relaxation
      Interval A(a), B(b), C(c), D(d);

      // first constraint: (x-a)*(y-c) >= 0, z - c*x - a*y >= -a*c
      Interval I1 = -A*C;
      LinExpr e1( {1.0, -c, -a}, {z, x, y} );
      lm.addCtr(I1.left(), e1);

      // second constraint: (x-a)*(y-d) <= 0, z - d*x - a*y <= -a*d
      Interval I2 = -A*D;
      LinExpr e2( {1.0, -d, -a}, {z, x, y} );
      lm.addCtr(e2, I2.right());

      // third constraint: (x-b)*(y-c) <= 0, z - c*x - b*y <= -b*c
      Interval I3 = -B*C;
      LinExpr e3( {1.0, -c, -b}, {z, x, y} );
      lm.addCtr(e3, I3.right());

      // fourth constraint: (x-b)*(y-d) >= 0, z - d*x - b*y >= -b*d
      Interval I4 = -B*D;
      LinExpr e4( {1.0, -d, -b}, {z, x, y} );
      lm.addCtr(I4.left(), e4);
   }
   else if (!xvar)
   {
      // z = x*y with x fixed => z - a*y = 0
      LinExpr e( {1.0, -a}, {z, y} );
      lm.addCtr(0.0, e, 0.0);
   }
   else if (!yvar)
   {
      // z = x*y with y fixed => z - c*x = 0
      LinExpr e( {1.0, -c}, {z, x} );
      lm.addCtr(0.0, e, 0.0);      
   }
}

///////////////////////////////////////////////////////////////////////////////

DagDiv::DagDiv(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Div, lsub)
{}

void DagDiv::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagDiv::eval()
{
   setVal(left()->val()/right()->val());
}

void DagDiv::proj(IntervalRegion& reg)
{
   left()->reduceDom(divPX(left()->val(), right()->val(), dom()));
   right()->reduceDom(divPY(left()->val(), right()->val(), dom()));
}

bool DagDiv::diff()
{
   // d(l/r)/dl = 1/r, d(l/r)/dr = -l/r^2
   left()->addDv(dv()/right()->val());
   right()->addDv((-dv()*left()->val()) / sqr(right()->val()));

   return !right()->val().containsZero();
}

void DagDiv::reval()
{
   setRval(Double::div(left()->rval(), right()->rval()));
}

bool DagDiv::rdiff()
{
   // d(l/r)/dl = 1/r, d(l/r)/dr = -l/r^2
   left()->addRdv(Double::div(rdv(), right()->rval()));
   right()->addRdv(Double::div(Double::mul(Double::usb(rdv()), left()->rval()),
                   Double::sqr(right()->rval())));

   return right()->rval() != 0.0;
}

void DagDiv::linearizeImpl(LPModel& lm)
{
   LinVar z = lm.getLinVar(indexLinVar()),
          x = lm.getLinVar(left()->indexLinVar()),
          y = lm.getLinVar(right()->indexLinVar());

   // z = x/y <=> a <= x <= b, c <= y <= d
   double a = left()->val().left(),
          b = left()->val().right(),
          c = right()->val().left(),
          d = right()->val().right(),
          u = val().left(),
          v = val().right();

   bool xvar = (a != b),   // left subterm not fixed?
        yvar = (c != d);   // right subterm not fixed?

   if (yvar)
   {
      // Mc Cormick relaxation on x = y*z, c <= y <= d, u <= z <= v
      Interval C(c), D(d), U(u), V(v);

      // first constraint: (y-c)*(z-u) >= 0, x -cz -uy >= -cu
      Interval I1 = -C*U;
      LinExpr e1( {1.0, -c, -u}, {x, z, y} );
      lm.addCtr(I1.left(), e1);

      // second constraint: (y-c)*(z-v) <= 0, x -cz -vy <= -cv
      Interval I2 = -C*V;
      LinExpr e2( {1.0, -c, -v}, {x, z, y} );
      lm.addCtr(e2, I2.right());

      // third constraint: (y-d)*(z-u) <= 0, x -dz -uy <= -du
      Interval I3 = -D*U;
      LinExpr e3( {1.0, -d, -u}, {x, z, y} );
      lm.addCtr(e3, I3.right());

      // fourth constraint: (y-d)*(z-v) >= 0, x -dz -vy >= -dv
      Interval I4 = -D*V;
      LinExpr e4( {1.0, -d, -v}, {x, z, y} );
      lm.addCtr(I4.left(), e4);
   }
   else if (xvar)
   {
      // y fixed => x = y*z with y = c => x -cz = 0
      LinExpr e( {1.0, -c}, {x, z} );
      lm.addCtr(0.0, e, 0.0);      
   }
}

///////////////////////////////////////////////////////////////////////////////

DagMin::DagMin(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Min, lsub)
{}

void DagMin::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagMin::eval()
{
   setVal(min(left()->val(), right()->val()));
}

void DagMin::proj(IntervalRegion& reg)
{
   left()->reduceDom(minPX(left()->val(), right()->val(), dom()));
   right()->reduceDom(minPY(left()->val(), right()->val(), dom()));
}

bool DagMin::diff()
{
   if (left()->val().isCertainlyLt(right()->val()))
   {
      // d(min(l,r))/dl = 1, d(min(l,r))/dr = 0 if l < r
      left()->addDv(dv());
   }
   else if (right()->val().isCertainlyLt(left()->val()))
   {
      // d(min(l,r))/dl = 0, d(min(l,r))/dr = 1 if r < l
      right()->addDv(dv());
   }
   else
   {
      // d(min(l,r))/dl = d(min(l,r))/dr = [0,1] otherwise
      Interval x(Interval(0,1)*dv());
      left()->addDv(x);
      right()->addDv(x);
   }
   return true;
}

void DagMin::reval()
{
   setRval(Double::min(left()->rval(), right()->rval()));
}

bool DagMin::rdiff()
{
   if (left()->rval() < right()->rval())
   {
      // d(min(l,r))/dl = 1, d(min(l,r))/dr = 0 if l < r
      left()->addRdv(rdv());
      return true;
   }
   else if (right()->rval() < left()->rval())
   {
      // d(min(l,r))/dl = 0, d(min(l,r))/dr = 1 if r < l
      right()->addRdv(rdv());
      return true;
   }
   else
   {
      // d(min(l,r))/dl = d(min(l,r))/dr = [0,1] otherwise
      return false;
   }
}

void DagMin::linearizeImpl(LPModel& lm)
{
   LinVar z = lm.getLinVar(indexLinVar()),
          x = lm.getLinVar(left()->indexLinVar()),
          y = lm.getLinVar(right()->indexLinVar());

   // z = min(x, y), a <= x <= b, c <= y <= d
   double a = left()->val().left(),
          b = left()->val().right(),
          c = right()->val().left(),
          d = right()->val().right();

   if (b < c)
   {
      // z = x => z - x = 0
      LinExpr e( {1.0, -1.0}, {z, x} );
      lm.addCtr(0.0, e, 0.0);
   }
   else if (d < a)
   {
      // z = y => z - y = 0
      LinExpr f( {1.0, -1.0}, {z, y} );
      lm.addCtr(0.0, f, 0.0);
   }
   else
   {
      // z <= x => z - x <= 0
      LinExpr e( {1.0, -1.0}, {z, x} );
      lm.addCtr(e, 0.0);

      // z <= y => z - y <= 0
      LinExpr f( {1.0, -1.0}, {z, y} );
      lm.addCtr(f, 0.0);
   }
}

///////////////////////////////////////////////////////////////////////////////

DagMax::DagMax(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Max, lsub)
{}

void DagMax::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagMax::eval()
{
   setVal(max(left()->val(), right()->val()));
}

void DagMax::proj(IntervalRegion& reg)
{
   left()->reduceDom(maxPX(left()->val(), right()->val(), dom()));
   right()->reduceDom(maxPY(left()->val(), right()->val(), dom()));
}

bool DagMax::diff()
{
   if (left()->val().isCertainlyGt(right()->val()))
   {
      // d(max(l,r))/dl = 1 and d(max(l,r))/dr = 0 if l > r
      left()->addDv(dv());
   }
   else if (right()->val().isCertainlyGt(left()->val()))
   {
      // d(max(l,r))/dl = 0 and d(max(l,r))/dr = 1 if r > l
      right()->addDv(dv());
   }
   else
   {
      // d(max(l,r))/dl = d(max(l,r))/dr = [0,1] otherwise
      Interval x(Interval(0,1)*dv());
      left()->addDv(x);
      right()->addDv(x);
   }
   return true;
}

void DagMax::reval()
{
   setVal(Double::max(left()->rval(), right()->rval()));
}

bool DagMax::rdiff()
{
   if (left()->rval() > right()->rval())
   {
      // d(max(l,r))/dl = 1 and d(max(l,r))/dr = 0 if l > r
      left()->addRdv(rdv());
      return true;
   }
   else if (right()->rval() > left()->rval())
   {
      // d(max(l,r))/dl = 0 and d(max(l,r))/dr = 1 if r > l
      right()->addRdv(rdv());
      return true;
   }
   else
   {
      // d(max(l,r))/dl = d(max(l,r))/dr = [0,1] otherwise
      return false;
   }
}

void DagMax::linearizeImpl(LPModel& lm)
{
   LinVar z = lm.getLinVar(indexLinVar()),
          x = lm.getLinVar(left()->indexLinVar()),
          y = lm.getLinVar(right()->indexLinVar());

   // z = max(x, y), a <= x <= b, c <= y <= d
   double a = left()->val().left(),
          b = left()->val().right(),
          c = right()->val().left(),
          d = right()->val().right();

   if (d < a)
   {
      // z = x => z - x = 0
      LinExpr e( {1.0, -1.0}, {z, x} );
      lm.addCtr(0.0, e, 0.0);
   }
   else if (b < c)
   {
      // z = y => z - y = 0
      LinExpr f( {1.0, -1.0}, {z, y} );
      lm.addCtr(0.0, f, 0.0);
   }
   else
   {
      // z >= x => z - x >= 0
      LinExpr e( {1.0, -1.0}, {z, x} );
      lm.addCtr(0.0, e);
      
      // z >= y => z - y >= 0
      LinExpr f( {1.0, -1.0}, {z, y} );
      lm.addCtr(0.0, f);
   }
}

///////////////////////////////////////////////////////////////////////////////

DagUsb::DagUsb(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Usb, lsub)
{}

void DagUsb::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagUsb::eval()
{
   setVal(-child()->val());
}

void DagUsb::proj(IntervalRegion& reg)
{
   child()->reduceDom(usubPX(child()->val(), dom()));
}

bool DagUsb::diff()
{
   // d(-u)/du = -1
   child()->addDv(-dv());

   return true;
}

void DagUsb::reval()
{
   setRval(Double::usb(child()->rval()));
}

bool DagUsb::rdiff()
{
   // d(-u)/du = -1
   child()->addRdv(Double::usb(rdv()));

   return true;
}

void DagUsb::linearizeImpl(LPModel& lm)
{
   LinVar y = lm.getLinVar(indexLinVar()),
          x = lm.getLinVar(child()->indexLinVar());

   // y = -x => y + x = 0
   LinExpr e( {1.0, 1.0}, {y, x} );
   lm.addCtr(0.0, e, 0.0);
}

///////////////////////////////////////////////////////////////////////////////

DagAbs::DagAbs(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Abs, lsub)
{}

void DagAbs::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagAbs::eval()
{
   setVal(abs(child()->val()));
}

void DagAbs::proj(IntervalRegion& reg)
{
   child()->reduceDom(absPX(child()->val(), dom()));
}

bool DagAbs::diff()
{
   // d(abs(u))/du = 1 if u>0, -1 if u<0, [-1,1] otherwise
   if (child()->val().isCertainlyGtZero())
      child()->addDv(dv());

   else if (child()->val().isCertainlyLtZero())
      child()->addDv(-dv());

   else
      child()->addDv(dv() | (-dv()));

   return true;
}

void DagAbs::reval()
{
   setRval(Double::abs(child()->rval()));
}

bool DagAbs::rdiff()
{
   // d(abs(u))/du = 1 if u>0, -1 if u<0, [-1,1] otherwise
   if (child()->rval() > 0.0)
   {
      child()->addRdv(rdv());
      return true;
   }
   else if (child()->rval() < 0.0)
   {
      child()->addRdv(-rdv());
      return true;
   }
   else
   {
      return false;
   }
}

void DagAbs::linearizeImpl(LPModel& lm)
{
   size_t iy = indexLinVar(),
          ix = child()->indexLinVar();

   LinVar y = lm.getLinVar(iy),
          x = lm.getLinVar(ix);

   double a = child()->val().left(),
          b = child()->val().right();

   if (a >= 0.0)
   {
      // y = x => y - x = 0
      LinExpr e( {1.0, -1.0}, {y, x} );
      lm.addCtr(0.0, e, 0.0);
   }
   else if (b <= 0.0)
   {
      // y = -x => y + x = 0
      LinExpr e( {1.0, 1.0}, {y, x} );
      lm.addCtr(0.0, e, 0.0);
   }
   else
   {
      // underestimation: y >= x <=> y - x >= 0
      LinExpr e1( {1.0, -1.0}, {y, x} );
      lm.addCtr(0.0, e1);

      // underestimation: y >= -x <=> y + x >= 0
      LinExpr e2( {1.0, 1.0}, {y, x} );
      lm.addCtr(0.0, e2);

      // overestimation
      auto f  = [](const Interval& x) { return abs(x); };
      overConvex(lm, iy, ix, a, b, f);
   }
}

///////////////////////////////////////////////////////////////////////////////

DagSgn::DagSgn(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Sgn, lsub)
{}

void DagSgn::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSgn::eval()
{
   setVal(sgn(child()->val()));
}

void DagSgn::proj(IntervalRegion& reg)
{
   child()->reduceDom(sgnPX(child()->val(), dom()));
}

bool DagSgn::diff()
{
   // d(sgn(u))/du = 0 except at 0
   return true;
}

void DagSgn::reval()
{
   setRval(Double::sgn(child()->rval()));
}

bool DagSgn::rdiff()
{
   // d(sgn(u))/du = 0 except at 0
   return true;
}

void DagSgn::linearizeImpl(LPModel& lm)
{
   // nothing to do
}

///////////////////////////////////////////////////////////////////////////////

DagSqr::DagSqr(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Sqr, lsub)
{}

void DagSqr::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSqr::eval()
{
   setVal(sqr(child()->val()));
}

void DagSqr::proj(IntervalRegion& reg)
{
   child()->reduceDom(sqrPX(child()->val(), dom()));
}

bool DagSqr::diff()
{
   // d(u^2)/du = 2u
   child()->addDv(2.0*child()->val()*dv());

   return true;
}

void DagSqr::reval()
{
   setRval(Double::sqr(child()->rval()));
}

bool DagSqr::rdiff()
{
   // d(u^2)/du = 2u
   child()->addRdv(Double::mul(2.0, Double::mul(child()->rval(), rdv())));

   return true;
}

void DagSqr::linearizeImpl(LPModel& lm)
{
   size_t iy = indexLinVar(),
          ix = child()->indexLinVar();
   
   double a = child()->val().left(),
          b = child()->val().right();

   auto f  = [](const Interval& x) { return sqr(x); };
   auto df = [](const Interval& x) { return 2.0*x; };

   underConvex(lm, iy, ix, a, b, a, f, df);
   underConvex(lm, iy, ix, a, b, b, f, df);
   underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

   overConvex(lm, iy, ix, a, b, f);
}

///////////////////////////////////////////////////////////////////////////////

DagSqrt::DagSqrt(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Sqrt, lsub)
{}

void DagSqrt::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSqrt::eval()
{
   setVal(sqrt(child()->val()));
}

void DagSqrt::proj(IntervalRegion& reg)
{
   child()->reduceDom(sqrtPX(child()->val(), dom()));
}

bool DagSqrt::diff()
{
   // d(sqrt(u))/du = 0.5/sqrt(u)
   child()->addDv((0.5*dv())/val());

   return true;
}

void DagSqrt::reval()
{
   setRval(Double::sqrt(child()->rval()));
}

bool DagSqrt::rdiff()
{
   // d(sqrt(u))/du = 0.5/sqrt(u)
   child()->addRdv(Double::div(Double::mul(0.5, rdv()), rval()));

   return true;
}

void DagSqrt::linearizeImpl(LPModel& lm)
{
   size_t iy = indexLinVar(),
          ix = child()->indexLinVar();

   double a = child()->val().left(),
          b = child()->val().right();

   if (a < 0.0) return;

   auto f  = [](const Interval& x) { return sqrt(x); };
   auto df = [](const Interval& x) { return 1.0/(2.0*sqrt(x)); };

   if (a > 0.0)
      overConcave(lm, iy, ix, a, b, a, f, df);

   overConcave(lm, iy, ix, a, b, b, f, df);
   overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

   underConcave(lm, iy, ix, a, b, f);
}

///////////////////////////////////////////////////////////////////////////////

DagPow::DagPow(Dag* dag, int n, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Pow, lsub),
        n_(n)
{}

int DagPow::exponent() const
{
   return n_;
}

bool DagPow::eqSymbol(const DagOp* other) const
{
   const DagPow *aux = dynamic_cast<const DagPow*>(other);

   if (aux == nullptr)
      return false;

   else
      return n_ == aux->n_;
}

void DagPow::print(std::ostream& os) const
{
   os << "^" << n_;
}

void DagPow::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagPow::eval()
{
   setVal(pow(child()->val(), n_));
}

void DagPow::proj(IntervalRegion& reg)
{
   child()->reduceDom(powPX(child()->val(), n_, dom()));
}

bool DagPow::diff()
{
   // d(u^n)/du = n * u^(n-1)
   child()->addDv(n_*dv()*pow(child()->val(), n_-1));

   return true;
}

void DagPow::reval()
{
   setRval(Double::pow(child()->rval(), n_));
}

bool DagPow::rdiff()
{
   // d(u^n)/du = n * u^(n-1)
   child()->addRdv(Double::mul(n_,
                               Double::mul(rdv(),
                                       Double::pow(child()->rval(), n_-1))));

   return true;
}

void DagPow::linearizeImpl(LPModel& lm)
{
   size_t iy = indexLinVar(),
          ix = child()->indexLinVar();
   
   double a = child()->val().left(),
          b = child()->val().right();

   int n = exponent();

   auto f  = [&n](const Interval& x) { return pow(x, n); };
   auto df = [&n](const Interval& x) { return n*pow(x, n-1); };

   if (n % 2 == 0 || a >= 0.0)
   {
      // convex function
      underConvex(lm, iy, ix, a, b, a, f, df);
      underConvex(lm, iy, ix, a, b, b, f, df);
      underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      overConvex(lm, iy, ix, a, b, f);
   }
   else
   {
      if (b <= 0.0)
      {
         // odd power, concave function
         overConcave(lm, iy, ix, a, b, a, f, df);
         overConcave(lm, iy, ix, a, b, b, f, df);
         overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

         underConcave(lm, iy, ix, a, b, f);         
      }
      else
      {
         // odd power, concave over [a, 0] and convex in [0, b]
         LinVar y = lm.getLinVar(iy),
                x = lm.getLinVar(ix);

         // best way, not implemented: find a tangent at point c in [0, b]
         // (c unknown) passing through (a, f(a)); find another tangent
         // at point c' in [a, 0] (c unknown) passing through (b, f(b))

         // underestimation
         Interval fa(f(a));
         underLine(lm, iy, ix, a, fa.left(), b, 0.0);

         // overestimation
         Interval fb(f(b));
         overLine(lm, iy, ix, a, 0.0, b, fb.right());
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

DagExp::DagExp(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Exp, lsub)
{}

void DagExp::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagExp::eval()
{
   setVal(exp(child()->val()));
}

void DagExp::proj(IntervalRegion& reg)
{
   child()->reduceDom(expPX(child()->val(), dom()));
}

bool DagExp::diff()
{
   // d(exp(u))/du = exp(u)
   child()->addDv(dv()*val());

   return true;
}

void DagExp::reval()
{
   setRval(Double::exp(child()->rval()));
}

bool DagExp::rdiff()
{
   // d(exp(u))/du = exp(u)
   child()->addRdv(Double::mul(rdv(), rval()));

   return true;
}

void DagExp::linearizeImpl(LPModel& lm)
{
   size_t iy = indexLinVar(),
          ix = child()->indexLinVar();
   
   double a = child()->val().left(),
          b = child()->val().right();

   auto f  = [](const Interval& x) { return exp(x); };
   auto df = [](const Interval& x) { return exp(x); };

   underConvex(lm, iy, ix, a, b, a, f, df);
   underConvex(lm, iy, ix, a, b, b, f, df);
   underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

   overConvex(lm, iy, ix, a, b, f);
}

///////////////////////////////////////////////////////////////////////////////

DagLog::DagLog(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Log, lsub)
{}

void DagLog::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagLog::eval()
{
   setVal(log(child()->val()));
}

void DagLog::proj(IntervalRegion& reg)
{
   child()->reduceDom(logPX(child()->val(), dom()));
}

bool DagLog::diff()
{
   // d(log(u))/du = 1/u
   child()->addDv(dv()/child()->val());

   return child()->val().isCertainlyGtZero();
}

void DagLog::reval()
{
   setRval(Double::log(child()->rval()));
}

bool DagLog::rdiff()
{
   // d(log(u))/du = 1/u
   child()->addRdv(Double::div(rdv(), child()->rval()));

   return child()->val().isCertainlyGtZero();
}

void DagLog::linearizeImpl(LPModel& lm)
{
   if (val().isInf()) return;

   size_t iy = indexLinVar(),
          ix = child()->indexLinVar();

   double a = child()->val().left(),
          b = child()->val().right();

   auto f  = [](const Interval& x) { return log(x); };
   auto df = [](const Interval& x) { return 1.0/x; };

   overConcave(lm, iy, ix, a, b, a, f, df);
   overConcave(lm, iy, ix, a, b, b, f, df);
   overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

   underConcave(lm, iy, ix, a, b, f);
}

///////////////////////////////////////////////////////////////////////////////

DagCos::DagCos(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Cos, lsub)
{}

void DagCos::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagCos::eval()
{
   setVal(cos(child()->val()));
}

void DagCos::proj(IntervalRegion& reg)
{
   child()->reduceDom(cosPX(child()->val(), dom()));
}

bool DagCos::diff()
{
   // d(cos(u))/du = -sin(u)
   child()->addDv(-dv()*sin(child()->val()));

   return true;
}

void DagCos::reval()
{
   setRval(Double::cos(child()->rval()));
}

bool DagCos::rdiff()
{
   // d(cos(u))/du = -sin(u)
   child()->addRdv(Double::mul(Double::usb(rdv()),
                               Double::sin(child()->rval())));

   return true;
}

void DagCos::linearizeImpl(LPModel& lm)
{   
   size_t iy = indexLinVar(),
          ix = child()->indexLinVar();

   double a = child()->val().left(),
          b = child()->val().right();

   auto f  = [](const Interval& x) { return cos(x); };
   auto df = [](const Interval& x) { return -sin(x); };

   if (val().isPositive())
   {
      // concave function
      overConcave(lm, iy, ix, a, b, a, f, df);
      overConcave(lm, iy, ix, a, b, b, f, df);
      overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      underConcave(lm, iy, ix, a, b, f);      
   }
   else if (val().isNegative())
   {
      // convex function
      underConvex(lm, iy, ix, a, b, a, f, df);
      underConvex(lm, iy, ix, a, b, b, f, df);
      underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      overConvex(lm, iy, ix, a, b, f);
   }
   else if (Interval::minusOnePlusOne().strictlyContains(val()))
   {
      // concavo-convex function
      relaxConcavoConvexCosSin(lm, iy, ix, a, b, f, df);
   }
   // else there is a stationaty point => no relaxation
}

///////////////////////////////////////////////////////////////////////////////

DagSin::DagSin(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Sin, lsub)
{}

void DagSin::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSin::eval()
{
   setVal(sin(child()->val()));
}

void DagSin::proj(IntervalRegion& reg)
{
   child()->reduceDom(sinPX(child()->val(), dom()));
}

bool DagSin::diff()
{
   // d(sin(u))/du = cos(u)
   child()->addDv(dv()*cos(child()->val()));

   return true;
}

void DagSin::reval()
{
   setRval(Double::sin(child()->rval()));
}

bool DagSin::rdiff()
{
   // d(sin(u))/du = cos(u)
   child()->addRdv(Double::mul(rdv(), Double::cos(child()->rval())));

   return true;
}

void DagSin::linearizeImpl(LPModel& lm)
{
   size_t iy = indexLinVar(),
          ix = child()->indexLinVar();

   double a = child()->val().left(),
          b = child()->val().right();

   auto f  = [](const Interval& x) { return sin(x); };
   auto df = [](const Interval& x) { return cos(x); };

   if (val().isPositive())
   {
   // concave function
      overConcave(lm, iy, ix, a, b, a, f, df);
      overConcave(lm, iy, ix, a, b, b, f, df);
      overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      underConcave(lm, iy, ix, a, b, f);      
   }
   else if (val().isNegative())
   {
      // convex function
      underConvex(lm, iy, ix, a, b, a, f, df);
      underConvex(lm, iy, ix, a, b, b, f, df);
      underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      overConvex(lm, iy, ix, a, b, f);
   }
   else if (Interval::minusOnePlusOne().strictlyContains(val()))
   {
      // concavo-convex function
      relaxConcavoConvexCosSin(lm, iy, ix, a, b, f, df);
   }
   // else there is a stationary point => no relaxation
}

///////////////////////////////////////////////////////////////////////////////

DagTan::DagTan(Dag* dag, const IndexList& lsub)
      : DagOp(dag, OpSymbol::Tan, lsub)
{}

void DagTan::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagTan::eval()
{
   setVal(tan(child()->val()));
}

void DagTan::proj(IntervalRegion& reg)
{
   child()->reduceDom(tanPX(child()->val(), dom()));
}

bool DagTan::diff()
{
   // d(tan(u))/du = 1+tan^2(u)
   child()->addDv(dv()*(1.0+sqr(val())));

   return val().isFinite();
}

void DagTan::reval()
{
   setRval(Double::tan(child()->rval()));
}

bool DagTan::rdiff()
{
   // d(tan(u))/du = 1+tan^2(u)
   child()->addRdv(Double::mul(rdv(), Double::add(1.0, Double::sqr(rval()))));

   return val().isFinite();
}

void DagTan::linearizeImpl(LPModel& lm)
{
   if (val().isInf()) return;

   size_t iy = indexLinVar(),
          ix = child()->indexLinVar();

   double a = child()->val().left(),
          b = child()->val().right();

   auto f  = [](const Interval& x) { return tan(x); };
   auto df = [](const Interval& x) { return 1.0/sqr(cos(x)); };

   if (val().isNegative())
   {
      // convex function
      underConvex(lm, iy, ix, a, b, a, f, df);
      underConvex(lm, iy, ix, a, b, b, f, df);
      underConvex(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      overConvex(lm, iy, ix, a, b, f);
   }
   else if (val().isPositive())
   {
      // concave function
      overConcave(lm, iy, ix, a, b, a, f, df);
      overConcave(lm, iy, ix, a, b, b, f, df);
      overConcave(lm, iy, ix, a, b, Interval(a, b).midpoint(), f, df);

      underConcave(lm, iy, ix, a, b, f);         
   }
   else
   {
      // concavo-convex function
      LinVar x = lm.getLinVar(ix),
             y = lm.getLinVar(iy);

      // underestimation: under the line passing through
      // (b, tan(b)) with slope 1, i.e. y <= x + p
      Interval p1(val().right() - Interval(b));
      LinExpr e1( {1.0, -1.0}, {y, x} );
      lm.addCtr(e1, p1.right());

      // overestimation: over the line passing through
      // (a, tan(a)) with slope 1, i.e. y >= x + p
      Interval p2(val().left() - Interval(a));
      LinExpr e2( {1.0, -1.0}, {y, x} );
      lm.addCtr(p2.left(), e2);
   }
}

///////////////////////////////////////////////////////////////////////////////

DagFun::DagFun(Dag* dag, size_t root, const Interval& image)
      : dag_(dag),
        node_(),
        vnode_(),
        scope_(),
        image_(image),
        idx_(0),
        inode_()
{
   DagFunCreator vis(this);
   DagNode* node = dag->node(root);
   node->acceptVisitor(vis);
}

Interval DagFun::getImage() const
{
   return image_;
}

void DagFun::setImage(const Interval& x)
{
   image_ = x;
}

Dag* DagFun::dag() const
{
   return dag_;
}

size_t DagFun::nbNodes() const
{
   return node_.size();
}

DagNode* DagFun::node(size_t i) const
{
   return node_[i];
}

DagNode* DagFun::rootNode() const
{
   return node_.back();
}

size_t DagFun::nbVars() const
{
   return vnode_.size();
}

DagVar* DagFun::varNode(size_t i) const
{
   return vnode_[i];
}

const Bitset& DagFun::bitset() const
{
   return rootNode()->bitset();
}

bool DagFun::dependsOn(const Bitset& bs) const
{
   return rootNode()->dependsOn(bs);
}

bool DagFun::dependsOn(const Variable& v) const
{
   return rootNode()->dependsOn(v);
}

Interval DagFun::intervalDeriv(size_t i) const
{
   return varNode(i)->dv();
}

void DagFun::setIndex(size_t i)
{
   idx_ = i;
}

size_t DagFun::index() const
{
   return idx_;
}

Scope DagFun::scope() const
{
   return scope_;
}

void DagFun::setScope(Scope s)
{
   scope_ = s;
}

double DagFun::realDeriv(size_t i) const
{
   return varNode(i)->rdv();
}

double DagFun::realValue() const
{
   return rootNode()->rval();
}

Interval DagFun::intervalValue() const
{
   return rootNode()->val();
}

bool DagFun::hasNode(DagNode* node) const
{
   auto it = inode_.find(node->index());
   return (it != inode_.end());
}

void DagFun::insertConstNode(DagConst* node)
{
   if (!hasNode(node))
   {
      node_.push_back(node);
      inode_.insert(node->index());
   }
}

void DagFun::insertVarNode(DagVar* node)
{
   size_t id = node->getVar().id();

   if (!hasNode(node))
   {
      node_.push_back(node);
      inode_.insert(node->index());

      // insertion in the vector of variables sorted by
      // an ascending ordering of the variable identifiers
      auto it = vnode_.begin();
      while ((it!=vnode_.end() && (id > (*it)->getVar().id())))
         ++it;

      vnode_.insert(it, node);
   }
}

void DagFun::insertOpNode(DagOp* node)
{
   if (!hasNode(node))
   {
      node_.push_back(node);
      inode_.insert(node->index());
   }
}

Interval DagFun::intervalEval(const IntervalRegion& reg)
{
   for (size_t i=0; i<nbNodes(); ++i)
      node_[i]->eval(reg);

   return rootNode()->val();
}

Interval DagFun::intervalEval(const RealPoint& pt)
{
   for (size_t i=0; i<nbNodes(); ++i)
      node_[i]->eval(pt);

   return rootNode()->val();
}

Interval DagFun::intervalEvalOnly(const Variable& v, const Interval& x)
{
   ASSERT(dependsOn(v),
          "Node " << rootNode()->index()
                  << " does not depend on variable " << v.getName());

   rootNode()->evalOnly(v, x);
   return rootNode()->val();
}

Proof DagFun::hc4Revise(IntervalRegion& reg)
{
   // assigns the projections to the universe
   for (size_t i=0; i<nbNodes(); ++i)
         node_[i]->setDom(Interval::universe());

   return sharedHc4Revise(reg);
}

Proof DagFun::hc4ReviseNeg(IntervalRegion& reg)
{
   Interval e = intervalEval(reg);

   if (e.isEmpty())
      return Proof::Empty;

   else if (image_.contains(e))
      return Proof::Empty;

   else if (!image_.overlaps(e))
      return Proof::Inner;

   else
   {
      if (image_.isSingleton() || image_.isUniverse())
         return Proof::Maybe;

      else if (image_.isInfLeft())
      {
         // assigns the projections to the universe
         for (size_t i=0; i<nbNodes()-1; ++i)
            node_[i]->setDom(Interval::universe());

         // projection over the root node
         rootNode()->setDom(e & Interval::moreThan(image_.right()));

         return hc4ReviseBack(reg);
      }

      else if (image_.isInfRight())
      {
         // assigns the projections to the universe
         for (size_t i=0; i<nbNodes()-1; ++i)
            node_[i]->setDom(Interval::universe());

         // projection over the root node
         rootNode()->setDom(e & Interval::lessThan(image_.left()));
         return hc4ReviseBack(reg);                  
      }

      else
      {
         // given the image [a, b] calculates the projections for each
         // part [-oo, a] and [b, +oo] and makes the interval disjunction

         // assigns the projections to the universe
         for (size_t i=0; i<nbNodes()-1; ++i)
            node_[i]->setDom(Interval::universe());

         IntervalRegion Xl(reg);
         rootNode()->setDom(e & Interval::lessThan(image_.left()));
         Proof pl = hc4ReviseBack(Xl);

         // assigns the projections to the universe
         for (size_t i=0; i<nbNodes(); ++i)
            node_[i]->setDom(Interval::universe());

         IntervalRegion Xr(reg);
         rootNode()->setDom(e & Interval::moreThan(image_.right()));
         Proof pr = hc4ReviseBack(Xr);

         Proof proof;

         if (pl == Proof::Empty)
            proof = pr;

         else if (pr == Proof::Empty)
            proof = pl;

         else
            proof = std::min(pl, pr);

         if (proof != Proof::Empty)
         {
            for (size_t i=0; i<nbVars(); ++i)
            {
               Variable v = varNode(i)->getVar();

               if (pl == Proof::Empty)
                  reg.set(v, Xr.get(v));

               else if (pr == Proof::Empty)
                  reg.set(v, Xl.get(v));

               else
                  reg.set(v, Xl.get(v) | Xr.get(v));
            }
         }

         return proof;
      }
   }
}

Proof DagFun::sharedHc4Revise(IntervalRegion& reg)
{
   Interval e = intervalEval(reg);

   if (e.isEmpty())
      return Proof::Empty;

   else if (image_.contains(e))
      return Proof::Inner;

   else if (!image_.overlaps(e))
      return Proof::Empty;

   else
   {
      rootNode()->setDom(e & image_);
      return hc4ReviseBack(reg);
   }
}

Proof DagFun::hc4ReviseBack(IntervalRegion& reg)
{
   for (int i=nbNodes()-1; i>=0; --i)
   {
      size_t j = (size_t)i;
      node_[j]->proj(reg);
   }

   for (size_t i=0; i<nbVars(); ++i)
   {
      Variable v = varNode(i)->getVar();

      if (reg.get(v).isEmpty())
         return Proof::Empty;
   }

   return Proof::Maybe;
}

void DagFun::intervalDiff()
{
   // initializes the derivatives
   rootNode()->setDv(Interval::one());

   for (size_t i=0; i<nbNodes()-1; ++i)
      node_[i]->setDv(Interval::zero());

   // differentiation
   for (int i=nbNodes()-1; i>=0; --i)
   {
      size_t j = (size_t)i;
      node_[j]->diff();
   }
}

void DagFun::intervalDiff(IntervalVector& grad)
{
   intervalDiff();
   for (size_t i=0; i<nbVars(); ++i)
      grad.set(i, intervalDeriv(i));
}

Interval DagFun::intervalDeriv(const Variable& v) const
{
   return dag_->findVarNode(v.id())->dv();
}

bool DagFun::intervalDiffOnly(const Variable& v)
{
   ASSERT(dependsOn(v), "Node " << rootNode()->index()
                           << " does not depend on variable " << v.getName());

   Interval e = rootNode()->val();

   if (e.isEmpty() || e.isInf()) return false;

   // initializes the derivatives
   rootNode()->setDv(Interval::one());

   for (size_t i=0; i<nbNodes()-1; ++i)
      node_[i]->setDv(Interval::zero());
   
   // differentiation
   return rootNode()->diffOnly(v);
}

bool DagFun::intervalDiffOnly(const Variable& v, const Interval& x)
{
   intervalEvalOnly(v, x);
   return intervalDiffOnly(v);
}

double DagFun::realEval(const RealPoint& pt)
{
   Double::rndNear();

   for (size_t i=0; i<nbNodes(); ++i)
      node_[i]->reval(pt);

   return rootNode()->rval();
}

void DagFun::realDiff()
{
   // initializes the derivatives
   rootNode()->setRdv(1.0);

   for (size_t i=0; i<nbNodes()-1; ++i)
      node_[i]->setRdv(0.0);

   // differentiation
   for (int i=nbNodes()-1; i>=0; --i)
   {
      size_t j = (size_t)i;
      node_[j]->rdiff();
   }
}

void DagFun::realDiff(RealVector& grad)
{
   realDiff();
   for (size_t i=0; i<nbVars(); ++i)
      grad.set(i, realDeriv(i));
}

double DagFun::realDeriv(const Variable& v) const
{
   return dag_->findVarNode(v.id())->rdv();
}

void DagFun::linearize(LPModel& lm)
{
   for (size_t i=0; i<nbNodes(); ++i)
   {
      DagNode* node = node_[i];
      if (node->indexLinVar() < 0) node->linearize(lm);
   }
}

///////////////////////////////////////////////////////////////////////////////

DagContext::DagContext() : dom()
{}

DagContext::DagContext(size_t n) : dom(n)
{}

void DagContext::extend(size_t n)
{
   for (size_t i=0; i<n; ++i)
      dom.push(Interval::universe());
}

DagContext* DagContext::clone() const
{
   return new DagContext(*this);
}

///////////////////////////////////////////////////////////////////////////////

Dag::Dag()
      : node_(),
        cnode_(),
        vnode_(),
        onode_(),
        fun_(),
        vmap_(),
        omap_(),
        scope_()
{
   defaultContext_ = new DagContext();
   context_ = defaultContext_;
}

Dag::~Dag()
{
   for (DagNode* node: node_)
      delete node;

   for (DagFun* fun: fun_)
      delete fun;

   delete defaultContext_;
}

bool Dag::isEmpty() const
{
   return node_.empty();
}

size_t Dag::nbNodes() const
{
   return node_.size();
}

DagNode* Dag::node(size_t i) const
{
   return node_[i];
}

size_t Dag::nbFuns() const
{
   return fun_.size();
}

void Dag::pushNode(DagNode* node)
{
   node_.push_back(node);
   defaultContext_->extend(1);
}

Interval Dag::dom(size_t i) const
{
   return context_->dom[i];
}

void Dag::setDom(size_t i, const Interval& x)
{
   context_->dom.set(i, x);
}

void Dag::reduceDom(size_t i, const Interval& x)
{
   Interval aux( x & context_->dom[i]);
   context_->dom.set(i, aux);
}

DagContext* Dag::cloneDefaultContext() const
{
   return defaultContext_->clone();
}

size_t Dag::nbVars() const
{
   return vnode_.size();
}

DagVar* Dag::varNode(size_t i) const
{
   return vnode_[i];
}

DagFun* Dag::fun(size_t i) const
{
   ASSERT(i < nbFuns(), "Access out of range to a DAG function");

   return fun_[i];
}

size_t Dag::insert(Constraint c)
{
   DagCreator creator(this, c);
   c.acceptVisitor(creator);
   return creator.index();
}

size_t Dag::insert(Term t, const Interval& img)
{
   Constraint c( t == 0.0 );
   size_t idx = insert(c);
   fun_[idx]->setImage(img);
   return idx;
}

size_t Dag::insertFun(DagFun* f)
{
   size_t idx = fun_.size();
   f->setIndex(idx);
   fun_.push_back(f);
   return idx;
}

size_t Dag::insertConstNode(const Interval& x)
{
   DagConst* node = findConstNode(x);

   if (node == nullptr)
   {
      size_t index = node_.size();
      node = new DagConst(this, index, x);

      pushNode(node);
      cnode_.push_back(node);
   }

   return node->index();
}

size_t Dag::insertVarNode(const Variable& v)
{
   auto it = vmap_.find(v.id());
   size_t index;

   if (it == vmap_.end())
   {
      index = node_.size();
      DagVar* node = new DagVar(this, index, v);

      // insertion in the vector of nodes
      pushNode(node);

      // insertion in the map
      vmap_.insert(std::make_pair(v.id(), index));

      // insertion in the vector of variables
      auto it = vnode_.begin();
      while ((it!=vnode_.end() && (v.id() > (*it)->getVar().id())))
         ++it;

      vnode_.insert(it, node);
   }
   else
      index = it->second;

   // insertion in the scope
   scope_.insert(v);

   return index;
}

size_t Dag::insertOpNode(size_t hcode, DagOp* node)
{
   DagOp* in = findOpNode(hcode, node);

   if (in != nullptr)
   {
      delete node;
      return in->index();
   }
   else
   {
      size_t index = node_.size();
      node->setIndex(index);
      pushNode(node);
      onode_.push_back(node);

      auto it = omap_.find(hcode);
      if (it == omap_.end())
      {
         std::vector<size_t> v;
         v.push_back(index);
         omap_.insert(std::make_pair(hcode, v));
      }
      else
      {
         std::vector<size_t>& v = it->second;
         v.push_back(index);
      }
      return index;
   }
}

DagConst* Dag::findConstNode(const Interval& x) const
{
   for (size_t i=0; i<cnode_.size(); ++i)
   {
      DagConst* node = cnode_[i];
      if (node->getConst().isSetEq(x))
         return node;
   }
   return nullptr;
}

DagOp* Dag::findOpNode(size_t hcode, DagOp* node) const
{
   auto it = omap_.find(hcode);

   if (it == omap_.end())
      return nullptr;

   else
   {
      for (int i : it->second)
      {
         DagOp *in = dynamic_cast<DagOp*>(node_[i]);

         if ((in != nullptr) && in->eq(node))
            return in;
      }

      return nullptr;
   }
}

DagVar* Dag::findVarNode(size_t id) const
{
   auto it = vmap_.find(id);

   if (it == vmap_.end())
      return nullptr;

   else
   {
      DagNode* node = node_[it->second];
      return static_cast<DagVar*>(node);
   }
}

Scope Dag::scope() const
{
   return scope_;
}

bool Dag::intervalPointEval(const RealPoint& pt, IntervalVector& val)
{
   ASSERT(val.size() == nbFuns(), "Size of interval vector different from " <<
                                  "the number of functions in a dag");

   bool res = true;

   for (size_t i=0; i<nbNodes(); ++i)
      node_[i]->eval(pt);

   for (size_t i=0; i<nbFuns(); ++i)
   {
      DagFun* f = fun_[i];
      val.set(i, f->intervalValue());
      if (f->intervalValue().isEmpty()) res = false;
   }
   return res;
}

bool Dag::intervalEval(const IntervalRegion& reg)
{
   bool res = true;

   for (size_t i=0; i<nbNodes(); ++i)
      node_[i]->eval(reg);

   for (size_t i=0; i<nbFuns(); ++i)
      if (fun_[i]->intervalValue().isEmpty()) res = false;

   return res;
}

bool Dag::intervalEval(const IntervalRegion& reg, IntervalVector& val)
{
   ASSERT(val.size() == nbFuns(), "Size of interval vector different from " <<
                                  "the number of functions in a dag");

   bool res = true;

   for (size_t i=0; i<nbNodes(); ++i)
      node_[i]->eval(reg);

   for (size_t i=0; i<nbFuns(); ++i)
   {
      DagFun* f = fun_[i];
      val.set(i, f->intervalValue());
      if (f->intervalValue().isEmpty()) res = false;
   }
   return res;
}

void Dag::intervalDiff(IntervalMatrix& jac)
{
   ASSERT(nbVars() == jac.ncols() && nbFuns() == jac.nrows(),
          "Bad dimensions of a Jacobian matrix used in a DAG");

   for (size_t i=0; i<nbFuns(); ++i)
   {
      // differentiates the i-th function
      DagFun* f = fun_[i];
      f->intervalDiff();

      // fills the i-th row of the matrix
      size_t j = 0;
      for (auto v : scope())
      {
         if (f->dependsOn(v))
            jac.set(i, j, f->intervalDeriv(v));
         else
            jac.set(i, j, Interval::zero());

         ++j;
      }
   }
}

bool Dag::realEval(const RealPoint& pt)
{
   Double::rndNear();

   for (size_t i=0; i<nbNodes(); ++i)
      node_[i]->reval(pt);

   for (size_t i=0; i<nbFuns(); ++i)
      if (Double::isNan(fun_[i]->realValue()))
         return false;

   return true;
}

bool Dag::realEval(const RealPoint& pt, RealVector& v)
{
   ASSERT(v.size() == nbFuns(),
          "Bad dimensionEvaluation of dag, bad vector size");

   Double::rndNear();
   bool res = true;

   for (size_t i=0; i<nbNodes(); ++i)
      node_[i]->reval(pt);

   for (size_t i=0; i<nbFuns(); ++i)
   {
      DagFun* f = fun_[i];
      double x = f->realValue();

      if (Double::isNan(x)) res = false;
      v.set(i, x);
   }

   return res;
}

void Dag::realDiff(RealMatrix& jac)
{
   ASSERT(nbVars() == jac.ncols() && nbFuns() == jac.nrows(),
          "Bad dimensions of a Jacobian matrix used in a DAG");

   for (size_t i=0; i<nbFuns(); ++i)
   {
      // differentiates the i-th function
      DagFun* f = fun_[i];
      f->realDiff();

      // fills the i-th row of the matrix
      size_t j = 0;
      for (auto v : scope())
      {
         if (f->dependsOn(v))
            jac.set(i, j, f->realDeriv(v));
         else
            jac.set(i, j, 0.0);

         ++j;
      }
   }
}

void Dag::linearize(LPModel& lm)
{
   // linearizes all the nodes
   for (size_t i=0; i<nbNodes(); ++i) node_[i]->linearize(lm);

   for (size_t j=0; j<nbFuns(); ++j)
   {
      // assigns the domain of the linear variable associated with the root
      // node of a function of this Dag
      DagFun* f = fun(j);
      LinVar lv = lm.getLinVar(f->rootNode()->indexLinVar());
      lv.setDomain(f->getImage());
   }
}

void Dag::linearize(LPModel& lm, const Bitset& bs)
{
   ASSERT(nbFuns() == bs.size(), "Bad bitset used to linearize a DAG");

   if (bs.areAllOnes())
   {
      linearize(lm);
   }
   else
   {
      // resets the indexes of all the linear variables
      for (size_t i=0; i<nbNodes(); ++i) node_[i]->setIndexLinVar(-1);

      for (size_t j=0; j<bs.size(); ++j)
      {
         if (bs.get(j))
         {
            // linearizes the function
            DagFun* f = fun(j);
            f->linearize(lm);

            // assigns the domain of the linear variable associated with
            // its root node
            LinVar lv = lm.getLinVar(f->rootNode()->indexLinVar());
            lv.setDomain(f->getImage());
         }
      }
   }
}

void Dag::print(std::ostream& os) const
{
   for (size_t i=0; i<nbFuns(); ++i)
   {
      DagFun* f = fun(i);
      os << "fun " << i << ": ";

      os << "[";
      for (size_t j=0; j<f->nbNodes(); ++j)
      {
         DagNode* node = f->node(j);
         if (j != 0) os << " ";
         os << node->index();
      }
      os << "] in " << f->getImage();
      os << " bitset: " << f->bitset() << std::endl;
   }

   for (DagNode* node : node_)
   {
      os << "node " << node->index() << ": ";
      node->print(os);

      os << " sub[";
      for (size_t i=0; i<node->subArity(); ++i)
      {
         if (i != 0) os << " ";
         os << node->subIndex(i);
      }
      os << "]";

      os << " par[";
      for (size_t i=0; i<node->parArity(); ++i)
      {
         if (i != 0) os << " ";
         os << node->parNode(i);
      }
      os << "]";
      os << " depends on " << node->bitset();
      
      os << std::endl;
   }
}

std::ostream& operator<<(std::ostream& os, const Dag& dag)
{
   dag.print(os);
   return os;
}

void Dag::bindContext(DagContext* context)
{
   context_ = context;
}

DagContext* Dag::unbindContext()
{
   ASSERT(context_ != defaultContext_, "");

   DagContext* aux = context_;
   context_ = defaultContext_;
   return aux;
}

///////////////////////////////////////////////////////////////////////////////

DagVisitor::~DagVisitor()
{}

void DagVisitor::apply(const DagConst* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagVar* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagAdd* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagSub* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagMul* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagDiv* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagMin* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagMax* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagUsb* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagAbs* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagSgn* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagSqr* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagSqrt* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagPow* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagExp* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagLog* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagCos* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagSin* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagTan* d)
{
   THROW("visit method not implemented");
}

///////////////////////////////////////////////////////////////////////////////

DagFunCreator::DagFunCreator(DagFun* f): f_(f)
{}

void DagFunCreator::visitSubNodes(const DagOp* d)
{
   for (size_t i=0; i<d->subArity(); ++i)
   {
      DagFunCreator vis(f_);
      d->subNode(i)->acceptVisitor(vis);
   }
}

void DagFunCreator::apply(const DagConst* d)
{
   DagConst* aux = const_cast<DagConst*>(d);
   f_->insertConstNode(aux);
}

void DagFunCreator::apply(const DagVar* d)
{
   DagVar* aux = const_cast<DagVar*>(d);
   f_->insertVarNode(aux);
}

void DagFunCreator::apply(const DagAdd* d)
{
   visitSubNodes(d);
   DagAdd* aux = const_cast<DagAdd*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagSub* d)
{
   visitSubNodes(d);
   DagSub* aux = const_cast<DagSub*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagMul* d)
{
   visitSubNodes(d);
   DagMul* aux = const_cast<DagMul*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagDiv* d)
{
   visitSubNodes(d);
   DagDiv* aux = const_cast<DagDiv*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagMin* d)
{
   visitSubNodes(d);
   DagMin* aux = const_cast<DagMin*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagMax* d)
{
   visitSubNodes(d);
   DagMax* aux = const_cast<DagMax*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagUsb* d)
{
   visitSubNodes(d);
   DagUsb* aux = const_cast<DagUsb*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagAbs* d)
{
   visitSubNodes(d);
   DagAbs* aux = const_cast<DagAbs*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagSgn* d)
{
   visitSubNodes(d);
   DagSgn* aux = const_cast<DagSgn*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagSqr* d)
{
   visitSubNodes(d);
   DagSqr* aux = const_cast<DagSqr*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagSqrt* d)
{
   visitSubNodes(d);
   DagSqrt* aux = const_cast<DagSqrt*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagPow* d)
{
   visitSubNodes(d);
   DagPow* aux = const_cast<DagPow*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagExp* d)
{
   visitSubNodes(d);
   DagExp* aux = const_cast<DagExp*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagLog* d)
{
   visitSubNodes(d);
   DagLog* aux = const_cast<DagLog*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagCos* d)
{
   visitSubNodes(d);
   DagCos* aux = const_cast<DagCos*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagSin* d)
{
   visitSubNodes(d);
   DagSin* aux = const_cast<DagSin*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagTan* d)
{
   visitSubNodes(d);
   DagTan* aux = const_cast<DagTan*>(d);
   f_->insertOpNode(aux);
}

///////////////////////////////////////////////////////////////////////////////

DagCreator::DagCreator(Dag* dag, Constraint c)
      : dag_(dag),
        c_(c),
        index_(0)
{}

size_t DagCreator::index()
{
   return index_;
}

void DagCreator::apply(const ConstraintEq* c)
{
   // visits the constraint
   DagTermCreator vis(dag_);
   Term t( c->left() - c->right());
   t.acceptVisitor(vis);

   // creates the function
   DagFun* f = new DagFun(dag_, vis.index(), Interval::zero());
   f->setScope(c->scope());
   index_ = dag_->insertFun(f);
}
   
void DagCreator::apply(const ConstraintLe* c)
{
   // visits the constraint
   DagTermCreator vis(dag_);
   Term t( c->left() - c->right());
   t.acceptVisitor(vis);

   // creates the function
   DagFun* f = new DagFun(dag_, vis.index(), Interval::negative());
   f->setScope(c->scope());
   index_ = dag_->insertFun(f);
}

void DagCreator::apply(const ConstraintLt* c)
{
   // visits the constraint
   DagTermCreator vis(dag_);
   Term t( c->left() - c->right());
   t.acceptVisitor(vis);

   // creates the function
   DagFun* f = new DagFun(dag_, vis.index(), Interval::negative());
   f->setScope(c->scope());
   index_ = dag_->insertFun(f);
}

void DagCreator::apply(const ConstraintGe* c)
{
   // visits the constraint
   DagTermCreator vis(dag_);
   Term t( c->left() - c->right());
   t.acceptVisitor(vis);

   // creation of the function
   DagFun* f = new DagFun(dag_, vis.index(), Interval::positive());
   f->setScope(c->scope());
   index_ = dag_->insertFun(f);
}

void DagCreator::apply(const ConstraintGt* c)
{
   // visits the constraint
   DagTermCreator vis(dag_);
   Term t( c->left() - c->right());
   t.acceptVisitor(vis);

   // creation of the function
   DagFun* f = new DagFun(dag_, vis.index(), Interval::positive());
   f->setScope(c->scope());
   index_ = dag_->insertFun(f);
}

void DagCreator::apply(const ConstraintIn* c)
{
   // visits the constraint
   DagTermCreator vis(dag_);
   c->term().acceptVisitor(vis);

   // creates the function
   DagFun* f = new DagFun(dag_, vis.index(), c->image());
   f->setScope(c->scope());
   index_ = dag_->insertFun(f);
}

///////////////////////////////////////////////////////////////////////////////

DagTermCreator::DagTermCreator(Dag* dag)
      : dag_(dag),
        lsub_(),
        index_(0)
{}

size_t DagTermCreator::index()
{
   return index_;
}

void DagTermCreator::visitSubnodes(const TermOp* t)
{
   for (size_t i=0; i<t->arity(); ++i)
   {
      DagTermCreator vis(dag_);
      t->subTerm(i)->acceptVisitor(vis);
      lsub_.push_back(vis.index());
   }
}

void DagTermCreator::apply(const TermConst* t)
{
   index_ = dag_->insertConstNode(Interval(t->getVal()));
}

void DagTermCreator::apply(const TermVar* t)
{
   index_ = dag_->insertVarNode(t->var());
}

void DagTermCreator::apply(const TermAdd* t)
{
   visitSubnodes(t);
   DagAdd* node = new DagAdd(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermSub* t)
{
   visitSubnodes(t);
   DagSub* node = new DagSub(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermMul* t)
{
   visitSubnodes(t);
   DagMul* node = new DagMul(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermDiv* t)
{
   visitSubnodes(t);
   DagDiv* node = new DagDiv(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermMin* t)
{
   visitSubnodes(t);
   DagMin* node = new DagMin(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermMax* t)
{
   visitSubnodes(t);
   DagMax* node = new DagMax(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermUsb* t)
{
   visitSubnodes(t);
   DagUsb* node = new DagUsb(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermAbs* t)
{
   visitSubnodes(t);
   DagAbs* node = new DagAbs(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermSgn* t)
{
   visitSubnodes(t);
   DagSgn* node = new DagSgn(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermSqr* t)
{
   visitSubnodes(t);
   DagSqr* node = new DagSqr(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermSqrt* t)
{
   visitSubnodes(t);
   DagSqrt* node = new DagSqrt(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermPow* t)
{
   visitSubnodes(t);
   DagPow* node = new DagPow(dag_,t->exponent(), lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermExp* t)
{
   visitSubnodes(t);
   DagExp* node = new DagExp(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermLog* t)
{
   visitSubnodes(t);
   DagLog* node = new DagLog(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermCos* t)
{
   visitSubnodes(t);
   DagCos* node = new DagCos(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermSin* t)
{
   visitSubnodes(t);
   DagSin* node = new DagSin(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermTan* t)
{
   visitSubnodes(t);
   DagTan* node = new DagTan(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

} // namespace
