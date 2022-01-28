// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/bco_model.hpp"
#include "realpaver/timer.hpp"

namespace realpaver {

BcoResult::BcoResult() :
   proof_(Proof::Maybe),
   xstar_(nullptr),
   fstar_(Interval::universe()),
   ctime_(0),
   ptime_(0),
   stime_(0)
{}

BcoResult::BcoResult(const BcoResult& res)
{
   proof_ = res.proof_;
   fstar_ = res.fstar_;
   ctime_ = res.ctime_;
   ptime_ = res.ptime_;
   stime_ = res.stime_;

   if (res.xstar_ == nullptr)
      xstar_ = res.xstar_;
   else
      xstar_ = new Box(*res.xstar_);
}

BcoResult& BcoResult::operator=(const BcoResult& res)
{
   proof_ = res.proof_;
   fstar_ = res.fstar_;
   ctime_ = res.ctime_;   
   ptime_ = res.ptime_;
   stime_ = res.stime_;

   if (res.xstar_ == nullptr)
      xstar_ = res.xstar_;
   else
      xstar_ = new Box(*res.xstar_);

   return *this;
}

BcoResult::~BcoResult()
{
   if (xstar_ != nullptr)
      delete xstar_;
}

Proof BcoResult::getProof() const
{
   return proof_;
}

void BcoResult::setProof(Proof proof)
{
   proof_ = proof;
}

Interval BcoResult::getOptimum() const
{
   return fstar_;
}

void BcoResult::setOptimum(const Interval& x)
{
   fstar_ = x;
}

size_t BcoResult::getCTime() const
{
   return ctime_;
}

void BcoResult::addCTime(size_t t)
{
   ctime_ += t;
}

size_t BcoResult::getPTime() const

{
   return ptime_;
}

void BcoResult::addPTime(size_t t)
{
   ptime_ += t;
}

size_t BcoResult::getSTime() const
{
   return stime_;
}

void BcoResult::addSTime(size_t t)
{
   stime_ += t;
}

Box* BcoResult::getBox() const
{
   return xstar_;
}

void BcoResult::setBox(const Box& B)
{
   if (xstar_ != nullptr)
      delete xstar_;

   xstar_ = new Box(B);
}

///////////////////////////////////////////////////////////////////////////////

BcoModel::BcoModel(Problem& P) :
   prob_(&P),
   bdag_(nullptr),
   L_(0),
   U_(0),
   ctimer_(),
   ptimer_(),
   stimer_()
{
   ASSERT(!P.isConstrained(),
          "BCO model created from a constrained problem.");


   ASSERT(P.hasObjective(),
          "BCO model created from a problem with no objective.");

   LOG("-- Creation of a BCO model -- ");

   ctimer_.start();

   bdag_ = new BcoDag(P);

   ctimer_.stop();
}

BcoModel::~BcoModel()
{
   delete bdag_;
}

BcoResult BcoModel::preprocess(const Param& param)
{
   BcoResult res;
   ptimer_.start();

   // checks the problem consistency
   if (!bdag_->checkProblem())
   {
      ptimer_.stop();
      res.addPTime(ptimer_.elapsedTime());
      res.setProof(Proof::Empty);
      return res;
   }

   // creates a propagator if any
   bdag_->makeDefaultPropagator();

   LOG("   > tries to fix some variables...");
   Box B(*initialBox());
   Proof proof = bdag_->propagator()->contract(B);

   if (proof == Proof::Empty)
   {
      LOG("   > first propagation: false");
      ptimer_.stop();
      res.addPTime(ptimer_.elapsedTime());
      res.setProof(Proof::Empty);
      return res;
   }
   else
   {
      nbFixed_ = 0;
      for (size_t i=0; i<prob_->nbVars(); ++i)
      {
         Variable v = prob_->varAt(i);
         if (B[v].isCanonical())
         {
            ++nbFixed_;
            LOG("     - fix " << v.name() << " to " << B[v]);
         }
         initialBox()->set(v, B[v]);
      }

      LOG("   > first propagation: true");      

      res.setOptimum(B[objVar()]);
      res.setBox(B);
      res.setProof(Proof::Maybe);

      if (nbFixed_ == prob_->nbVars())
      {
         LOG("   > problem solved by preprocessing");
         res.setProof(Proof::Optimal);

      }

      LOG("   > box after preprocessing: " << B);

      ptimer_.stop();
      res.addPTime(ptimer_.elapsedTime());
      return res;
   }
}

BcoResult BcoModel::solve(const Param& param)
{
   LOG("\n-- Solving of a Bound-Constrained Optimization problem -- ");

   BcoResult res;
   stimer_.start();

   L_ = initialBox()->at(objVar()).left();
   U_ = initialBox()->at(objVar()).right();

   DEBUG("lu : " << Interval(L_, U_));

   // creates the search space
   BcoSpace space;
   SharedBcoNode inode = std::make_shared<BcoNode>(*initialBox());
   inode->setLower(L_);
   inode->setUpper(U_);
   inode->setDepth(0);
   space.insertNode(inode);

DEBUG("inode : " << *inode);

/*

   // creates the search space
   BcoSpace space;
   SharedBcoNode inode = std::make_shared<BcoNode>(*init_);
   space.insertNode(inode);

   bool iter = true;
   
   while(iter)
   {
      inode = space.extractNode();

      // TODO
  

      // objective precise enough?
      if (param.objPrecision().test(Interval(L_, U_)))
      {
         iter = false;
      }

      // no more regions
      if (space.isEmpty())
         iter = false;

      // timeout reached
      if (res.getCTime() + res.getPTime()
                         + stimer_.elapsedTime() >= param.timeout())
         iter = false;
   }

   */

   stimer_.stop();
   res.addSTime(stimer_.elapsedTime());

   return res;
}

} // namespace
