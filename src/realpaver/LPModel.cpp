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
 * @file   LPModel.cpp
 * @brief  Classes for representing linear programs
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include <iostream>
#include <sstream>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Double.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/LPModel.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

LinVarRep::LinVarRep(int index, double lb, double ub,
                     bool continuous, const std::string& name) :
   index_(index),
   lb_(lb),
   ub_(ub),
   objval_(0.0),
   mult_(0.0),
   name_(name),
   continuous_(continuous)
{}

LinVarRep::~LinVarRep()
{}

void LinVarRep::setContinuous()
{
   continuous_ = true;
}

void LinVarRep::setInteger()
{
   continuous_ = false;
}

void LinVarRep::setLB(double lb)
{
   lb_ = lb;
}

void LinVarRep::setUB(double ub)
{
   ub_ = ub;
}

void LinVarRep::setDomain(const Interval& x)
{
   lb_ = x.left();
   ub_ = x.right();
}

void LinVarRep::setName(const std::string& name)
{
   name_ = name;
}

void LinVarRep::setObjVal(double val)
{
   objval_ = val;
}

void LinVarRep::setMultiplier(double val)
{
   mult_ = val;
}

bool LinVarRep::isContinuous() const
{
   return continuous_ == true;
}

bool LinVarRep::isInteger() const
{
   return continuous_ == false;
}

int LinVarRep::getIndex() const
{
   return index_;
}

double LinVarRep::getLB() const
{
   return lb_;
}

double LinVarRep::getUB() const
{
   return ub_;
}

Interval LinVarRep::getDomain() const
{
   return Interval(lb_, ub_);
}

std::string LinVarRep::getName() const
{
   if (name_ != "")
      return name_;

   else
   {
      std::ostringstream os;
      os << "_v" << index_;
      return os.str();
   }
}

double LinVarRep::getObjVal() const
{
   return objval_;
}

double LinVarRep::getMultiplier() const
{
   return mult_;
}

bool LinVarRep::isPrimalSolutionFeasible() const
{
   return lb_ <= objval_ && objval_ <= ub_;
}

/*----------------------------------------------------------------------------*/

LinVar::LinVar() : rep_(nullptr)
{}

LinVar::LinVar(int index, double lb, double ub,
               bool continuous, const std::string& name):
   rep_(nullptr)
{
   rep_ = std::make_shared<LinVarRep>(index, lb, ub, continuous, name);
}

void LinVar::setContinuous()
{
   rep_->setContinuous();
}

void LinVar::setInteger()
{
   rep_->setInteger();
}

void LinVar::setLB(double lb)
{
   rep_->setLB(lb);
}

void LinVar::setUB(double ub)
{
   rep_->setUB(ub);
}

void LinVar::setDomain(const Interval& x)
{
   rep_->setDomain(x);
}

void LinVar::setName(const std::string& name)
{
   rep_->setName(name);
}

void LinVar::setObjVal(double val) const
{
   rep_->setObjVal(val);
}

void LinVar::setMultiplier(double val)
{
   rep_->setMultiplier(val);
}

bool LinVar::isContinuous() const
{
   return rep_->isContinuous();
}

bool LinVar::isInteger() const
{
   return rep_->isInteger();
}

int LinVar::getIndex() const
{
   return rep_->getIndex();
}

double LinVar::getLB() const
{
   return rep_->getLB();
}

double LinVar::getUB() const
{
   return rep_->getUB();
}

Interval LinVar::getDomain() const
{
   return rep_->getDomain();
}

std::string LinVar::getName() const
{
   return rep_->getName();
}

double LinVar::getObjVal() const
{
   return rep_->getObjVal();
}

double LinVar::getMultiplier() const
{
   return rep_->getMultiplier();
}

bool LinVar::isPrimalSolutionFeasible() const
{
   return rep_->isPrimalSolutionFeasible();
}

/*----------------------------------------------------------------------------*/

void LinExprRep::addTerm(double a, LinVar v)
{
   coefs_.push_back(a);
   vars_.push_back(v.getIndex());
}

int LinExprRep::getNbTerms() const
{
   return coefs_.size();
}

double LinExprRep::getCoef(int i) const
{
   return coefs_[i];
}

int LinExprRep::getIndexVar(int i) const
{
   return vars_[i];
}

double* LinExprRep::getCoefs() const
{
   return const_cast<double*>(coefs_.data());
}

int* LinExprRep::getIndexVars() const
{
   return const_cast<int*>(vars_.data());
}

void LinExprRep::print(std::ostream& os) const
{
   for (size_t i=0; i<coefs_.size(); ++i)
   {
      double c = coefs_[i];
      int iv = vars_[i];

      if (c > 0) os << "+" << c << "*v" << iv;
      if (c < 0) os << "-" << -c << "*v" << iv;
   }
}

/*----------------------------------------------------------------------------*/

LinExpr::LinExpr() :
   rep_(nullptr)
{
   rep_ = std::make_shared<LinExprRep>();
}

LinExpr::LinExpr(const std::initializer_list<double>& lc,
                 const std::initializer_list<LinVar>& lv)
{
   rep_ = std::make_shared<LinExprRep>();

   auto ic = lc.begin();
   auto iv = lv.begin();

   for ( ; ic!=lc.end(); ++ic, ++iv)
      addTerm(*ic, *iv);
}

LinExpr& LinExpr::addTerm(double a, LinVar v)
{
   rep_->addTerm(a, v);
   return *this;
}

int LinExpr::getNbTerms() const
{
   return rep_->getNbTerms();
}

double LinExpr::getCoef(int i) const
{
   return rep_->getCoef(i);
}

int LinExpr::getIndexVar(int i) const
{
   return rep_->getIndexVar(i);
}

double* LinExpr::getCoefs() const
{
   return rep_->getCoefs();
}

int* LinExpr::getIndexVars() const
{
   return rep_->getIndexVars();
}

std::ostream& operator<<(std::ostream& os, const LinExpr& e)
{
   e.rep_->print(os);
   return os;
}

/*----------------------------------------------------------------------------*/

LinCtrRep::LinCtrRep(double lb, LinExpr e, double ub) :
   expr_(e),
   lb_(lb),
   ub_(ub),
   mult_(0.0)
{}

LinCtrRep::LinCtrRep(double lb, LinExpr e) :
   expr_(e),
   lb_(lb),
   ub_(Double::inf())
{}

LinCtrRep::LinCtrRep(LinExpr e, double ub) :
   expr_(e),
   lb_(Double::neginf()),
   ub_(ub)
{}

void LinCtrRep::setLB(double lb)
{
   lb_ = lb;
}

void LinCtrRep::setUB(double ub)
{
   ub_ = ub;
}

void LinCtrRep::setMultiplier(double val)
{
   mult_ = val;
}

LinExpr LinCtrRep::getExpr() const
{
   return expr_;
}

double LinCtrRep::getLB() const
{
   return lb_;
}

double LinCtrRep::getUB() const
{
   return ub_;
}

double LinCtrRep::getMultiplier() const
{
   return mult_;
}

bool LinCtrRep::isLessEqual() const
{
   return lb_ == Double::neginf();
}

bool LinCtrRep::isGreaterEqual() const
{
   return ub_ == Double::inf();
}

bool LinCtrRep::isEqual() const
{
   return lb_ == ub_;
}

bool LinCtrRep::isRange() const
{
   return lb_ != Double::neginf() && ub_ != Double::inf() && lb_ < ub_;
}

/*----------------------------------------------------------------------------*/

LinCtr::LinCtr(double lb, LinExpr e, double ub) :
   rep_(nullptr)
{
   rep_ = std::make_shared<LinCtrRep>(lb, e, ub);
}

LinCtr::LinCtr(double lb, LinExpr e) :
   rep_(nullptr)
{
   rep_ = std::make_shared<LinCtrRep>(lb, e, Double::inf());
}

LinCtr::LinCtr(LinExpr e, double ub) :
   rep_(nullptr)
{
   rep_ = std::make_shared<LinCtrRep>(Double::neginf(), e, ub);
}

void LinCtr::setLB(double lb)
{
   rep_->setLB(lb);
}

void LinCtr::setUB(double ub)
{
   rep_->setUB(ub);
}

void LinCtr::setMultiplier(double val)
{
   rep_->setMultiplier(val);
}

LinExpr LinCtr::getExpr() const
{
   return rep_->getExpr();
}

double LinCtr::getLB() const
{
   return rep_->getLB();
}

double LinCtr::getUB() const
{
   return rep_->getUB();
}

double LinCtr::getMultiplier() const
{
   return rep_->getMultiplier();
}

bool LinCtr::isLessEqual() const
{
   return rep_->isLessEqual();
}

bool LinCtr::isGreaterEqual() const
{
   return rep_->isGreaterEqual();
}

bool LinCtr::isEqual() const
{
   return rep_->isEqual();
}

bool LinCtr::isRange() const
{
   return rep_->isRange();
}

/*----------------------------------------------------------------------------*/

LPModel::LPModel() :
   vars_(),
   ctrs_(),
   obj_(),
   minimization_(true),
   objval_(0.0),
   maxseconds_(Param::GetDblParam("LP_TIME_LIMIT")),
   maxiter_(Param::GetIntParam("LP_ITER_LIMIT")),
   status_(OptimizationStatus::Other)
{}

LPModel::~LPModel()
{}

LinVar LPModel::makeVar(double lb, double ub, const std::string& name)
{
   int index = vars_.size();
   bool continuous = true;
   vars_.push_back(LinVar(index, lb, ub, continuous, name));
   return vars_.back();
}

void LPModel::addCtr(LinCtr c)
{
   ctrs_.push_back(c);
}

void LPModel::addCtr(double lb, LinExpr e, double ub)
{
   ctrs_.push_back(LinCtr(lb, e, ub));
}

void LPModel::addCtr(double lb, LinExpr e)
{
   ctrs_.push_back(LinCtr(lb, e));
}

void LPModel::addCtr(LinExpr e, double ub)
{
   ctrs_.push_back(LinCtr(e, ub));
}

void LPModel::setObj(LinExpr obj, bool minimization)
{
   obj_ = obj;
   minimization_ = minimization;
}

void LPModel::setMinimization()
{
   minimization_ = true;
}

void LPModel::setMaximization()
{
   minimization_ = false;
}

double LPModel::getObjVal(LinVar v) const
{
   return v.getObjVal();
}

double LPModel::getObjVal() const
{
   return objval_;
}

void LPModel::setObjVal(double val)
{
   objval_ = val;
}

void LPModel::printVars(std::ostream& os) const
{
   for (LinVar v : vars_)
      os << v.getLB()   << " <= "
         << v.getName() << " <= "
         << v.getUB()   << std::endl;
}

void LPModel::printLinExpr(std::ostream& os, LinExpr e) const
{
   for (int i=0; i<e.getNbTerms(); ++i)
   {
      double a = e.getCoef(i);

      if (a < 0.0)
      {
         if (i != 0) os << " - ";
         else os << "-";
         if (a != -1) os << -a;
      }
      else
      {
         if (i != 0) os << " + ";
         if (a != 1.0) os << a;
      }
 
      int j = e.getIndexVar(i);
      os << vars_[j].getName();
   }
}

void LPModel::printCtrs(std::ostream& os) const
{
   for (LinCtr c : ctrs_)
   {
      if (c.isLessEqual())
      {
         printLinExpr(os, c.getExpr());
         os << " <= " << c.getUB();
      }
      else if (c.isGreaterEqual())
      {
         os << c.getLB() << " <= ";
         printLinExpr(os, c.getExpr());
      }
      else if (c.isEqual())
      {
         printLinExpr(os, c.getExpr());
         os << " = " << c.getLB();
      }
      else
      {
         os << c.getLB() << " <= ";
         printLinExpr(os, c.getExpr());
         os << " <= " << c.getUB();
      }
      os << std::endl;
   }
}

void LPModel::printObj(std::ostream& os) const
{
   if (minimization_)
      os << "MIN ";
   else
      os << "MAX ";

   printLinExpr(os, obj_);
   os << std::endl;
}

std::ostream& operator<<(std::ostream& os, const LPModel& model)
{
   model.printObj(os);
   model.printCtrs(os);
   model.printVars(os);
   return os;
}

bool LPModel::optimize()
{
   return false;
}

bool LPModel::reoptimize()
{
   return false;
}

int LPModel::getNbLinVars() const
{
   return vars_.size();
}

LinVar LPModel::getLinVar(int i) const
{
   return vars_[i];
}

int LPModel::getNbLinCtrs() const
{
   return ctrs_.size();
}

LinCtr LPModel::getLinCtr(int i) const
{
   return ctrs_[i];
}

LinExpr LPModel::getObjExpr() const
{
   return obj_;
}

bool LPModel::isMinimization() const
{
   return minimization_;
}

bool LPModel::isMaximization() const
{
   return !minimization_;
}

void LPModel::setMaxSeconds(double s)
{
   maxseconds_ = s;
}

double LPModel::getMaxSeconds() const
{
   return maxseconds_;
}

void LPModel::setMaxIter(size_t n)
{
   maxiter_ = n;
}

size_t LPModel::getMaxIter() const
{
   return maxiter_;
}

OptimizationStatus LPModel::getStatus() const
{
   return status_;
}

void LPModel::setStatus(OptimizationStatus status)
{
   status_ = status;
}

double LPModel::getSafeObjVal() const
{
/*
std::cout << "safeL : " << safeL() << std::endl << std::endl;
std::cout << "safeB : " << safeB() << std::endl << std::endl;
std::cout << "safeC : " << safeC() << std::endl << std::endl;
std::cout << "safeX : " << safeX() << std::endl << std::endl;
std::cout << "L.B   : " << safeL().scalarProduct(safeB()) << std::endl << std::endl;
std::cout << "R     : " << safeR() << std::endl << std::endl;
std::cout << "R.X   : " << safeR().scalarProduct(safeX()) << std::endl << std::endl;
std::cout << "AT*L  : " << safeAT()*safeL() << std::endl << std::endl;

std::cout << "Bound : " << getObjVal() << std::endl;
std::cout << "safeBound : " << safeBound() << std::endl;
*/
   return safeBound();
}

bool LPModel::isPrimalSolutionFeasible() const
{
   // tests the variables
   for (auto& v : vars_)
      if (!v.isPrimalSolutionFeasible())
         return false;

   // tests the constraints
   for (auto& ctr : ctrs_)
   {
      Interval val = Interval::zero();
      LinExpr expr = ctr.getExpr();

      for (int i=0; i<expr.getNbTerms(); ++i)
      {
         LinVar v = getLinVar(expr.getIndexVar(i));
         val += Interval(expr.getCoef(i)) * v.getObjVal();
      }

      Interval img(ctr.getLB(), ctr.getUB());
      if (!img.contains(val)) return false;
   }

   return true;
}

IntervalVector LPModel::safeX() const
{
   int n = getNbLinVars();

   IntervalVector X(n);
   for (int i=0; i<n; ++i)
   {
      X.set(i, getLinVar(i).getDomain());
   }
   return X;
}

IntervalVector LPModel::safeB() const
{
   int n = getNbLinVars();
   int m = getNbLinCtrs();

   IntervalVector B(m+n);
   for (int i=0; i<m; ++i)
   {
      LinCtr c = getLinCtr(i);
      B.set(i, Interval(c.getLB(), c.getUB()));
   }
   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      B.set(m+i, v.getDomain());
   }
   return B;
}

IntervalVector LPModel::safeC() const
{
   int n = getNbLinVars();
   LinExpr obj = getObjExpr();

   IntervalVector C(n, Interval::zero());
   for (int i=0; i<obj.getNbTerms(); ++i)
   {
      int j = obj.getIndexVar(i);
      double a = obj.getCoef(i);
      C.set(j, Interval(a));
   }

   return C;
}

IntervalVector LPModel::safeR() const
{
   return safeAT()*safeL() - safeC();
}

IntervalVector LPModel::safeL() const
{
   int n = getNbLinVars();
   int m = getNbLinCtrs();
   IntervalVector L(m+n);
   for (int i=0; i<m; ++i)
   {
      LinCtr c = getLinCtr(i);
      L.set(i, Interval(c.getMultiplier()));
   }
   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      L.set(m+i, Interval(v.getMultiplier()));
   }
   return L;
}

IntervalMatrix LPModel::safeAT() const
{
   int n = getNbLinVars();
   int m = getNbLinCtrs();
   IntervalMatrix AT(n, m+n, Interval::zero());

   // primal constraints
   for (int j=0; j<m; ++j)
   {
      LinExpr e = getLinCtr(j).getExpr();
      int p = e.getNbTerms();
      for (int k=0; k<p; ++k)
      {
         double a = e.getCoef(k);
         int ind = e.getIndexVar(k);
         AT.set(ind, j, Interval(a));
      }
   }

   // bound constraints
   for (int i=0; i<n; ++i)
   {
      AT.set(i, m+i, Interval::one());
   }

   return AT;
}

double LPModel::safeBound() const
{
   IntervalVector L = safeL();
   IntervalVector R = safeAT()*L - safeC();
   Interval z = L.scalarProduct(safeB()) - R.scalarProduct(safeX());
   return isMinimization() ? z.left() : z.right();
}

bool LPModel::isSafeInfeasible() const
{
   IntervalVector L = safeL();
   IntervalVector R = safeAT()*L;
   Interval d = R.scalarProduct(safeX()) - L.scalarProduct(safeB());
   return !d.containsZero();
}

} // namespace
