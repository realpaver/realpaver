///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Double.hpp"

namespace realpaver {

DagNode::DagNode(Dag* dag, size_t index)
      : bitset_(),
        dag_(dag),
        index_(index),
        vpar_(),
        vsub_(),
        val_()
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
   return bitset_.get(v.getId());
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

///////////////////////////////////////////////////////////////////////////////

DagVar::DagVar(Dag* dag, size_t index, Variable v)
      : DagNode(dag, index),
        v_(v)
{
   bitset_ = Bitset(1 + v.getId(), 0);
   bitset_.setOne(v.getId());
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
   if (v_.getId() == v.getId())
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

size_t DagFun::nbNode() const
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

size_t DagFun::nbVar() const
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

Interval DagFun::deriv(size_t i) const
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

double DagFun::rderiv(size_t i) const
{
   return varNode(i)->rdv();
}

double DagFun::rval() const
{
   return rootNode()->rval();
}

Interval DagFun::val() const
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
   size_t id = node->getVar().getId();

   if (!hasNode(node))
   {
      node_.push_back(node);
      inode_.insert(node->index());

      // insertion in the vector of variables sorted by
      // an ascending ordering of the variable identifiers
      auto it = vnode_.begin();
      while ((it!=vnode_.end() && (id > (*it)->getVar().getId())))
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

Interval DagFun::eval(const IntervalRegion& reg)
{
   for (size_t i=0; i<nbNode(); ++i)
      node_[i]->eval(reg);

   return rootNode()->val();
}

Interval DagFun::eval(const RealPoint& pt)
{
   for (size_t i=0; i<nbNode(); ++i)
      node_[i]->eval(pt);

   return rootNode()->val();
}

Interval DagFun::evalOnly(const Variable& v, const Interval& x)
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
   for (size_t i=0; i<nbNode(); ++i)
         node_[i]->setDom(Interval::universe());

   return sharedHc4Revise(reg);
}

Proof DagFun::hc4ReviseNeg(IntervalRegion& reg)
{
   Interval e = eval(reg);

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
         for (size_t i=0; i<nbNode()-1; ++i)
            node_[i]->setDom(Interval::universe());

         // projection over the root node
         rootNode()->setDom(e & Interval::moreThan(image_.right()));

         return hc4ReviseBack(reg);
      }

      else if (image_.isInfRight())
      {
         // assigns the projections to the universe
         for (size_t i=0; i<nbNode()-1; ++i)
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
         for (size_t i=0; i<nbNode()-1; ++i)
            node_[i]->setDom(Interval::universe());

         IntervalRegion Xl(reg);
         rootNode()->setDom(e & Interval::lessThan(image_.left()));
         Proof pl = hc4ReviseBack(Xl);

         // assigns the projections to the universe
         for (size_t i=0; i<nbNode(); ++i)
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
            for (size_t i=0; i<nbVar(); ++i)
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
   Interval e = eval(reg);

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
   for (int i=nbNode()-1; i>=0; --i)
   {
      size_t j = (size_t)i;
      node_[j]->proj(reg);
   }

   for (size_t i=0; i<nbVar(); ++i)
   {
      Variable v = varNode(i)->getVar();

      if (reg.get(v).isEmpty())
         return Proof::Empty;
   }

   return Proof::Maybe;
}

bool DagFun::diff(const IntervalRegion& reg)
{
   eval(reg);
   return diff();
}

bool DagFun::diff()
{
   Interval e = rootNode()->val();

   if (e.isEmpty() || e.isInf())
      return false;

   // initializes the derivatives
   rootNode()->setDv(Interval::one());

   for (size_t i=0; i<nbNode()-1; ++i)
      node_[i]->setDv(Interval::zero());

   // differentiation
   bool res = true;
   for (int i=nbNode()-1; i>=0; --i)
   {
      size_t j = (size_t)i;
      res = res & node_[j]->diff();
   }

   return res;
}

IntervalVector DagFun::grad() const
{
   IntervalVector G(nbVar());
   toGrad(G);
   return G;
}

void DagFun::toGrad(IntervalVector& G) const
{
   for (size_t i=0; i<nbVar(); ++i)
      G.set(i, deriv(i));
}

Interval DagFun::deriv(const Variable& v) const
{
   return dag_->findVarNode(v.getId())->dv();
}

bool DagFun::diffOnly(const Variable& v)
{
   ASSERT(dependsOn(v), "Node " << rootNode()->index()
                           << " does not depend on variable " << v.getName());

   Interval e = rootNode()->val();

   if (e.isEmpty() || e.isInf()) return false;

   // initializes the derivatives
   rootNode()->setDv(Interval::one());

   for (size_t i=0; i<nbNode()-1; ++i)
      node_[i]->setDv(Interval::zero());
   
   // differentiation
   return rootNode()->diffOnly(v);
}

bool DagFun::diffOnly(const Variable& v, const Interval& x)
{
   evalOnly(v, x);
   return diffOnly(v);
}

double DagFun::reval(const RealPoint& pt)
{
   Double::rndNear();

   for (size_t i=0; i<nbNode(); ++i)
      node_[i]->reval(pt);

   return rootNode()->rval();
}

bool DagFun::rdiff(const RealPoint& pt)
{
   reval(pt);
   return rdiff();
}

bool DagFun::rdiff()
{
   double e = rootNode()->rval();

   if (Double::isNan(e) || Double::isInf(e))
      return false;

   // initializes the derivatives
   rootNode()->setRdv(1.0);

   for (size_t i=0; i<nbNode()-1; ++i)
      node_[i]->setRdv(0.0);

   // differentiation
   bool res = true;
   for (int i=nbNode()-1; i>=0; --i)
   {
      size_t j = (size_t)i;
      res = res & node_[j]->rdiff();
   }

   return res;
}

RealVector DagFun::rgrad() const
{
   RealVector G(nbVar());
   toRgrad(G);
   return G;
}

void DagFun::toRgrad(RealVector& G) const
{
   for (size_t i=0; i<nbVar(); ++i)
      G.set(i, rderiv(i));
}

double DagFun::rderiv(const Variable& v) const
{
   return dag_->findVarNode(v.getId())->rdv();
}

Scope DagFun::rfunScope() const
{
   return scope();
}

size_t DagFun::rfunArity() const
{
   return scope().size();   
}

double DagFun::rfunEval(const RealPoint& pt)
{
   return reval(pt);
}

void DagFun::rfunDiff(const RealPoint& pt, RealVector& g)
{
   rdiff(pt);
   toRgrad(g);
}

void DagFun::rfunEvalDiff(const RealPoint& pt, RealVector& g, double& e)
{
   rdiff(pt);
   toRgrad(g);
   e = rval();
}

Scope DagFun::ifunScope() const
{
   return scope();
}

size_t DagFun::ifunArity() const
{
   return scope().size();   
}

Interval DagFun::ifunEval(const IntervalRegion& reg)
{
   return eval(reg);
}

Interval DagFun::ifunEvalPoint(const RealPoint& pt)
{
   return eval(pt);
}

void DagFun::ifunDiff(const IntervalRegion& reg, IntervalVector& g)
{
   diff(reg);
   toGrad(g);
}

void DagFun::ifunEvalDiff(const IntervalRegion& reg, IntervalVector& g,
                          Interval& e)
{
   diff(reg);
   toGrad(g);
   e = val();
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
        omap_()
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

size_t Dag::nbNode() const
{
   return node_.size();
}

DagNode* Dag::node(size_t i) const
{
   return node_[i];
}

size_t Dag::nbFun() const
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

size_t Dag::nbVar() const
{
   return vnode_.size();
}

DagVar* Dag::varNode(size_t i) const
{
   return vnode_[i];
}

DagFun* Dag::fun(size_t i) const
{
   ASSERT(i < nbFun(), "Access out of range to a DAG function");

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
   auto it = vmap_.find(v.getId());
   size_t index;

   if (it == vmap_.end())
   {
      index = node_.size();
      DagVar* node = new DagVar(this, index, v);

      // insertion in the vector of nodes
      pushNode(node);

      // insertion in the map
      vmap_.insert(std::make_pair(v.getId(), index));

      // insertion in the vector of variables
      auto it = vnode_.begin();
      while ((it!=vnode_.end() && (v.getId() > (*it)->getVar().getId())))
         ++it;

      vnode_.insert(it, node);
   }
   else
      index = it->second;

DEBUG("insert var node : " << v.getName() << " @ " << index);



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
   Scope res;
   for (DagVar* node : vnode_)
      res.insert(node->getVar());
   return res;
}

void Dag::reval(const RealPoint& pt)
{
   Double::rndNear();

   for (size_t i=0; i<nbNode(); ++i)
      node_[i]->reval(pt);
}

void Dag::eval(const IntervalRegion& reg)
{
   for (size_t i=0; i<nbNode(); ++i)
      node_[i]->eval(reg);
}

void Dag::print(std::ostream& os) const
{
   for (size_t i=0; i<nbFun(); ++i)
   {
      DagFun* f = fun(i);
      os << "fun " << i << ": ";

      os << "[";
      for (size_t j=0; j<f->nbNode(); ++j)
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
