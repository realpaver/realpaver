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
 * @file   Dag.cpp
 * @brief  DAG of constraints
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include <sstream>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Double.hpp"
#include "realpaver/FlatFunction.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, DagSymbol op)
{
   switch(op)
   {
      case DagSymbol::Cst:  return os << "cst";
      case DagSymbol::Var:  return os << "var";
      case DagSymbol::Add:  return os << "+";
      case DagSymbol::Sub:  return os << "-";
      case DagSymbol::Mul:  return os << "*";
      case DagSymbol::Div:  return os << "/";
      case DagSymbol::Min:  return os << "min";
      case DagSymbol::Max:  return os << "max";
      case DagSymbol::Usb:  return os << "-";
      case DagSymbol::Abs:  return os << "abs";
      case DagSymbol::Sgn:  return os << "sgn";
      case DagSymbol::Sqr:  return os << "sqr";
      case DagSymbol::Sqrt: return os << "sqrt";
      case DagSymbol::Pow:  return os << "pow";
      case DagSymbol::Exp:  return os << "exp";
      case DagSymbol::Log:  return os << "log";
      case DagSymbol::Cos:  return os << "cos";
      case DagSymbol::Sin:  return os << "sin";
      case DagSymbol::Tan:  return os << "tan";
      case DagSymbol::Cosh: return os << "cosh";
      case DagSymbol::Sinh: return os << "sinh";
      case DagSymbol::Tanh: return os << "tanh";
      case DagSymbol::Lin: return os << "lin";

      default: os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

DagNode::DagNode(Dag* dag, DagSymbol symb, size_t index)
      : bitset_(),
        dag_(dag),
        symb_(symb),
        index_(index),
        vpar_(),
        vsub_(),
        ival_()
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

DagSymbol DagNode::symbol() const
{
   return symb_;
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

bool DagNode::isShared() const
{
   return parArity() > 1;
}

Interval DagNode::ival() const
{
   return ival_;
}

void DagNode::setIval(const Interval& x)
{
   ival_ = x;
}

size_t DagNode::nbOccurrences(const Variable& v) const
{
   return 0;
}

/*----------------------------------------------------------------------------*/

DagConst::DagConst(Dag* dag, size_t index, const Interval& x)
      : DagNode(dag, DagSymbol::Cst, index),
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

void DagConst::iEvalNode(const IntervalBox& B)
{
   setIval(x_);
}

void DagConst::iEvalTree(const IntervalBox& B)
{
   setIval(x_);
}

Interval DagConst::getConst() const
{
   return x_;
}

/*----------------------------------------------------------------------------*/

DagVar::DagVar(Dag* dag, size_t index, Variable v)
      : DagNode(dag, DagSymbol::Var, index),
        v_(v)
{
   bitset_ = Bitset(v.id(), v.id());
   bitset_.setOne(v.id());
}

size_t DagVar::nbOccurrences(const Variable& v) const
{
   return (v_.id() == v.id()) ? 1 : 0;
}

void DagVar::print(std::ostream& os) const
{
   os << v_.getName();
}

void DagVar::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagVar::iEvalNode(const IntervalBox& B)
{
   setIval(B.get(v_));
}

void DagVar::iEvalTree(const IntervalBox& B)
{
   setIval(B.get(v_));
}

Variable DagVar::getVar() const
{
   return v_;
}

/*----------------------------------------------------------------------------*/

DagOp::DagOp(Dag* dag, DagSymbol symb, const IndexList& lsub)
      : DagNode(dag, symb)
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
   return symbol() == other->symbol();
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

size_t DagOp::nbOccurrences(const Variable& v) const
{
   size_t n = 0;

   for (size_t i=0; i<subArity(); ++i)
      n += subNode(i)->nbOccurrences(v);

   return n;
}

void DagOp::print(std::ostream& os) const
{
   os << symbol();
}

void DagOp::iEvalNode(const IntervalBox& B)
{
   iEvalNode();
}

void DagOp::iEvalTree(const IntervalBox& B)
{
   for (size_t i=0; i<subArity(); ++i)
      subNode(i)->iEvalTree(B);

   iEvalNode();
}

/*----------------------------------------------------------------------------*/

DagAdd::DagAdd(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Add, lsub)
{}

void DagAdd::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagAdd::iEvalNode()
{
   setIval(left()->ival()+right()->ival());
}

/*----------------------------------------------------------------------------*/

DagSub::DagSub(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Sub, lsub)
{}

void DagSub::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSub::iEvalNode()
{
   setIval(left()->ival()-right()->ival());
}

/*----------------------------------------------------------------------------*/

DagMul::DagMul(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Mul, lsub)
{}

void DagMul::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagMul::iEvalNode()
{
   setIval(left()->ival()*right()->ival());
}

/*----------------------------------------------------------------------------*/

DagDiv::DagDiv(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Div, lsub)
{}

void DagDiv::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagDiv::iEvalNode()
{
   setIval(left()->ival()/right()->ival());
}

/*----------------------------------------------------------------------------*/

DagMin::DagMin(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Min, lsub)
{}

void DagMin::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagMin::iEvalNode()
{
   setIval(min(left()->ival(), right()->ival()));
}

/*----------------------------------------------------------------------------*/

DagMax::DagMax(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Max, lsub)
{}

void DagMax::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagMax::iEvalNode()
{
   setIval(max(left()->ival(), right()->ival()));
}

/*----------------------------------------------------------------------------*/

DagUsb::DagUsb(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Usb, lsub)
{}

void DagUsb::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagUsb::iEvalNode()
{
   setIval(-child()->ival());
}

/*----------------------------------------------------------------------------*/

DagAbs::DagAbs(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Abs, lsub)
{}

void DagAbs::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagAbs::iEvalNode()
{
   setIval(abs(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagSgn::DagSgn(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Sgn, lsub)
{}

void DagSgn::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSgn::iEvalNode()
{
   setIval(sgn(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagSqr::DagSqr(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Sqr, lsub)
{}

void DagSqr::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSqr::iEvalNode()
{
   setIval(sqr(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagSqrt::DagSqrt(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Sqrt, lsub)
{}

void DagSqrt::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSqrt::iEvalNode()
{
   setIval(sqrt(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagPow::DagPow(Dag* dag, int n, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Pow, lsub),
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

void DagPow::iEvalNode()
{
   setIval(pow(child()->ival(), n_));
}

/*----------------------------------------------------------------------------*/

DagExp::DagExp(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Exp, lsub)
{}

void DagExp::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagExp::iEvalNode()
{
   setIval(exp(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagLog::DagLog(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Log, lsub)
{}

void DagLog::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagLog::iEvalNode()
{
   setIval(log(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagCos::DagCos(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Cos, lsub)
{}

void DagCos::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagCos::iEvalNode()
{
   setIval(cos(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagSin::DagSin(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Sin, lsub)
{}

void DagSin::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSin::iEvalNode()
{
   setIval(sin(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagTan::DagTan(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Tan, lsub)
{}

void DagTan::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagTan::iEvalNode()
{
   setIval(tan(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagCosh::DagCosh(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Cosh, lsub)
{}

void DagCosh::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagCosh::iEvalNode()
{
   setIval(cosh(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagSinh::DagSinh(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Sinh, lsub)
{}

void DagSinh::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagSinh::iEvalNode()
{
   setIval(sinh(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagTanh::DagTanh(Dag* dag, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Tanh, lsub)
{}

void DagTanh::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagTanh::iEvalNode()
{
   setIval(tanh(child()->ival()));
}

/*----------------------------------------------------------------------------*/

DagLin::DagLin(Dag* dag, const TermLin& tl, const IndexList& lsub)
      : DagOp(dag, DagSymbol::Lin, lsub),
        cst_(),
        terms_()
{
   cst_ = tl.getCst();

   for (size_t i=0; i<tl.nbTerms(); ++i)
   {
      DagVar* node = dag->findVarNode(tl.var(i).id());
      Item it = { tl.coef(i), node, Interval::universe() };
      terms_.insert(it);
   }
}

bool DagLin::eqSymbol(const DagOp* other) const
{
   if (symbol() != other->symbol())
      return false;

   if (subArity() != other->subArity())
      return false;

   const DagLin* dl = static_cast<const DagLin*>(other);

   // compares the constant terms
   if (!cst_.isSetEq(dl->cst_))
      return false;

   // compares the factors of the linear terms
   auto it = terms_.begin();
   auto jt = dl->terms_.begin();

   while (it != terms_.end())
   {
      if (!(*it).coef.isSetEq((*jt).coef))
         return false;

      ++it;
      ++jt;
   }

   // no need to compare the variables / sub-nodes since this is done
   // in the eq() method

   return true;
}

size_t DagLin::nbOccurrences(const Variable& v) const
{
   return dependsOn(v) ? 1 : 0;
}

void DagLin::print(std::ostream& os) const
{
   os << symbol() << "(" << cst_ << ")";
   for (const auto& t : terms_)
   {
      os << "(" << t.coef << ", " << t.node->index() << ")";
   }
}

void DagLin::acceptVisitor(DagVisitor& vis) const
{
   vis.apply(this);
}

void DagLin::iEvalNode()
{
   setIval(cst_);

   for (const Item& citm : terms_)
   {
      Item& itm = const_cast<Item&>(citm);
      itm.ival = (itm.coef.isOne()) ? itm.node->ival() :
                                      itm.coef * itm.node->ival();
      setIval(ival() + itm.ival);
   }
}

DagLin::const_iterator DagLin::begin() const
{
   return terms_.begin();
}

DagLin::const_iterator DagLin::end() const
{
   return terms_.end();
}

Interval DagLin::getCoefSub(const_iterator it) const
{
   return (*it).coef;
}

DagVar* DagLin::getNodeSub(const_iterator it) const
{
   return (*it).node;
}

Interval DagLin::getCst() const
{
   return cst_;
}

size_t DagLin::nbTerms() const
{
   return terms_.size();
}

DagVar* DagLin::varNode(size_t i) const
{
   ASSERT(i<nbTerms(), "Bad access in a DagLin node @ " << i);

   auto it = terms_.begin();
   std::advance(it, i);
   return it->node;
}

Interval DagLin::coef(size_t i) const
{
   ASSERT(i<nbTerms(), "Bad access in a DagLin node @ " << i);

   auto it = terms_.begin();
   std::advance(it, i);
   return it->coef;
}

/*----------------------------------------------------------------------------*/

DagFun::DagFun(Dag* dag, size_t root, Scope scop, const Interval& image)
      : dag_(dag),
        node_(),
        vnode_(),
        scop_(),
        image_(image),
        idx_(0),
        inode_()
{
   scop_ = ScopeBank::getInstance()->insertScope(scop);
   DagFunCreator vis(this);
   DagNode* node = dag->node(root);
   node->acceptVisitor(vis);
}

DagFun::~DagFun()
{}

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

bool DagFun::dependsOn(const Variable& v) const
{
   return rootNode()->dependsOn(v);
}

size_t DagFun::nbOccurrences(const Variable& v) const
{
   return rootNode()->nbOccurrences(v);
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
   return scop_;
}

std::shared_ptr<FlatFunction> DagFun::flatFunction() const
{
   return flat_;
}

void DagFun::setFlatFunction(std::shared_ptr<FlatFunction> fun)
{
   flat_ = fun;
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

Proof DagFun::hc4ReviseNeg(IntervalBox& B)
{
   return flat_->hc4ReviseNeg(B);
}

Proof DagFun::hc4Revise(IntervalBox& B)
{
   return flat_->hc4Revise(B);
}

Interval DagFun::iEval(const IntervalBox& B)
{
   return flat_->iEval(B);
}

void DagFun::iDiff(const IntervalBox& B, IntervalVector& G)
{
   flat_->iDiff(B, G);
}

void DagFun::iDiffHansen(const IntervalBox& B, IntervalVector& G)
{
   IntervalBox X = B.midpoint();
   IntervalVector V(G.size());

   size_t i = 0;
   for (const auto& v : scop_)
   {
      X.set(v, B.get(v));
      flat_->iDiff(B, V);
      G[i] = V[i];
      ++i;
   }
}

double DagFun::rEval(const RealPoint& pt)
{
   return flat_->rEval(pt);
}

void DagFun::rDiff(const RealPoint& pt, RealVector& G)
{
   flat_->rDiff(pt, G);
}

bool DagFun::isLinear() const
{
   return rootNode()->symbol() == DagSymbol::Lin;
}

DagLin* DagFun::getLinearExpr() const
{
   return isLinear() ? static_cast<DagLin*>(rootNode()) : nullptr;
}

/*----------------------------------------------------------------------------*/

Dag::Dag()
      : node_(),
        cnode_(),
        vnode_(),
        onode_(),
        fun_(),
        vmap_(),
        omap_(),
        scop_()
{}

Dag::~Dag()
{
   for (DagNode* node: node_)
      delete node;

   for (DagFun* fun: fun_)
      delete fun;
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
   scop_.insert(v);

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
   return scop_;
}

bool Dag::iEvalNodes(const IntervalBox& B)
{
   bool res = true;

   for (size_t i=0; i<nbNodes(); ++i)
      node_[i]->iEvalNode(B);

   for (size_t i=0; i<nbFuns(); ++i)
      if (fun_[i]->rootNode()->ival().isEmpty())
         res = false;

   return res;
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
      os << " bitset: " << f->rootNode()->bitset() << std::endl;
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

void Dag::iEval(const IntervalBox& B, IntervalVector& V)
{
   for (size_t i=0; i<nbFuns(); ++i)
   {
      V.set(i, fun(i)->iEval(B));
   }
}

void Dag::iDiff(const IntervalBox& B, IntervalMatrix& J)
{
   ASSERT(nbVars() == J.ncols() && nbFuns() == J.nrows(),
          "Bad dimensions of a Jacobian matrix used in a DAG");

   for (size_t i=0; i<nbFuns(); ++i)
   {
      // differentiates the i-th function
      DagFun* f = fun_[i];
      IntervalVector G(f->nbVars());
      f->iDiff(B, G);

      // fills the i-th row of the matrix
      size_t j = 0;
      for (const auto& v : scope())
      {
         if (f->dependsOn(v))
            J.set(i, j, G[f->scope().index(v)]);
         else
            J.set(i, j, Interval::zero());

         ++j;
      }
   }
}

void Dag::iDiffHansen(const IntervalBox& B, IntervalMatrix& H)
{
   ASSERT(nbVars() == H.ncols() && nbFuns() == H.nrows(),
          "Bad dimensions of a Hansen matrix used in a DAG");

   ASSERT(nbVars() == nbFuns(),
          "Hansen's derivatives can be computed only for square systems");

   IntervalBox X =  B.midpoint();

   size_t j = 0;
   for (const auto& v : scope())
   {
      // assigns the domain of v
      X.set(v, B.get(v));

      // calculates every dfi/dv and assigns it in the Jacobian matrix
      for (size_t i=0; i<nbFuns(); ++i)
      {
         DagFun* f = fun_[i];

         if (f->dependsOn(v))
         {
            IntervalVector G(f->nbVars());
            f->iDiff(X, G);
            H.set(i, j, G[f->scope().index(v)]);
         }
         else
         {
            H.set(i, j, Interval::zero());
         }
      }

      ++j;
   }
}

void Dag::rEval(const RealPoint& pt, RealVector& V)
{
   for (size_t i=0; i<nbFuns(); ++i)
   {
      V.set(i, fun(i)->rEval(pt));
   }
}

void Dag::rDiff(const RealPoint& pt, RealMatrix& J)
{
   ASSERT(nbVars() == J.ncols() && nbFuns() == J.nrows(),
          "Bad dimensions of a Jacobian matrix used in a DAG");

   for (size_t i=0; i<nbFuns(); ++i)
   {
      // differentiates the i-th function
      DagFun* f = fun_[i];
      RealVector G(f->nbVars());
      f->rDiff(pt, G);

      // fills the i-th row of the matrix
      size_t j = 0;
      for (const auto& v : scope())
      {
         if (f->dependsOn(v))
            J.set(i, j, G[f->scope().index(v)]);
         else
            J.set(i, j, 0.0);

         ++j;
      }
   }
}

/*----------------------------------------------------------------------------*/

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

void DagVisitor::apply(const DagCosh* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagSinh* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagTanh* d)
{
   THROW("visit method not implemented");
}

void DagVisitor::apply(const DagLin* d)
{
   THROW("visit method not implemented");
}

/*----------------------------------------------------------------------------*/

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

void DagFunCreator::apply(const DagCosh* d)
{
   visitSubNodes(d);
   DagCosh* aux = const_cast<DagCosh*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagSinh* d)
{
   visitSubNodes(d);
   DagSinh* aux = const_cast<DagSinh*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagTanh* d)
{
   visitSubNodes(d);
   DagTanh* aux = const_cast<DagTanh*>(d);
   f_->insertOpNode(aux);
}

void DagFunCreator::apply(const DagLin* d)
{
   visitSubNodes(d);
   DagLin* aux = const_cast<DagLin*>(d);
   f_->insertOpNode(aux);
}

/*----------------------------------------------------------------------------*/

DagCreator::DagCreator(Dag* dag, Constraint c)
      : dag_(dag),
        c_(c),
        index_(0)
{}

size_t DagCreator::index()
{
   return index_;
}

void DagCreator::apply(const ArithCtrEq* c)
{
   Term t(c->left() - c->right());
   make(t, c->scope(), Interval::zero(), c->flatFunction());
}

void DagCreator::apply(const ArithCtrLe* c)
{
   Term t( c->left() - c->right());
   make(t, c->scope(), Interval::negative(), c->flatFunction());
}

void DagCreator::apply(const ArithCtrLt* c)
{
   Term t( c->left() - c->right());
   make(t, c->scope(), Interval::negative(), c->flatFunction());
}

void DagCreator::apply(const ArithCtrGe* c)
{
   Term t( c->left() - c->right());
   make(t, c->scope(), Interval::positive(), c->flatFunction());
}

void DagCreator::apply(const ArithCtrGt* c)
{
   Term t( c->left() - c->right());
   make(t, c->scope(), Interval::positive(), c->flatFunction());
}

void DagCreator::apply(const ArithCtrIn* c)
{
   make(c->term(), c->scope(), c->image(), c->flatFunction());
}

void DagCreator::make(const Term& t, Scope scop, const Interval& img,
                      std::shared_ptr<FlatFunction> fun)
{
   size_t root = 0;  // index of root node

   try
   {
      // tries to make a linear term
      TermLin tl(t);

      // lsub is the list of indexes of variable nodes
      IndexList lsub;
      for (size_t i=0; i<tl.nbTerms(); ++i)
      {
         size_t index = dag_->insertVarNode(tl.var(i));
         lsub.push_back(index);
      }

      // creates and inserts the node
      DagLin* node = new DagLin(dag_, tl, lsub);
      root = dag_->insertOpNode(tl.hashCode(), node);
      scop = tl.makeScope();
   }
   catch(Exception e)
   {
      // handles a non-linear term
      DagTermCreator vis(dag_);
      t.acceptVisitor(vis);
      root = vis.index();
   }

   // creates the function
   DagFun* f = new DagFun(dag_, root, scop, img);
   f->setFlatFunction(fun);
   index_ = dag_->insertFun(f);
}

/*----------------------------------------------------------------------------*/

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

void DagTermCreator::apply(const TermCst* t)
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

void DagTermCreator::apply(const TermCosh* t)
{
   visitSubnodes(t);
   DagCosh* node = new DagCosh(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermSinh* t)
{
   visitSubnodes(t);
   DagSinh* node = new DagSinh(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

void DagTermCreator::apply(const TermTanh* t)
{
   visitSubnodes(t);
   DagTanh* node = new DagTanh(dag_, lsub_);
   index_ = dag_->insertOpNode(t->hashCode(), node);
}

} // namespace
