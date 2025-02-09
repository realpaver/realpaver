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
#include "RealVector.hpp"
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
   ub_(ub)
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

std::ostream& operator<<(std::ostream& os, const LPStatus& status)
{
   switch(status)
   {
      case LPStatus::Optimal:                return os << "Optimal";
      case LPStatus::Infeasible:             return os << "Infeasible";
      case LPStatus::Unbounded:              return os << "Unbounded";
      case LPStatus::InfeasibleOrUnbounded:  return os << "InfeasibleOrUnbounded";
      case LPStatus::StopOnIterLimit:        return os << "StopOnIterLimit";
      case LPStatus::StopOnTimeLimit:        return os << "StopOnTimeLimit";
      case LPStatus::Other:                  return os << "Other";
      default: os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

std::ostream& operator<<(std::ostream& os, const LPSense& sense)
{
   switch(sense)
   {
      case LPSense::Min: return os << "MIN";
      case LPSense::Max: return os << "MAX";
      default: os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

LPModel::LPModel() :
   vars_(),
   ctrs_(),
   cost_(),
   sense_(LPSense::Min),
   maxseconds_(Param::GetDblParam("LP_TIME_LIMIT")),
   maxiter_(Param::GetIntParam("LP_ITER_LIMIT")),
   tol_(Param::GetDblParam("LP_FEAS_TOL"))
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

size_t LPModel::addCtr(LinCtr c)
{
   size_t i = ctrs_.size();
   ctrs_.push_back(c);
   return i;
}

size_t LPModel::addCtr(double lb, LinExpr e, double ub)
{
   return addCtr(LinCtr(lb, e, ub));
}

size_t LPModel::addCtr(double lb, LinExpr e)
{
   return addCtr(LinCtr(lb, e));
}

size_t LPModel::addCtr(LinExpr e, double ub)
{
   return addCtr(LinCtr(e, ub));
}

void LPModel::setCost(LinExpr cost)
{
   cost_ = cost;
}

void LPModel::setSense(LPSense sense)
{
   sense_ = sense;
}

LPSense LPModel::getSense() const
{
   return sense_;
}

void LPModel::printVars(std::ostream& os) const
{
   for (int i=0; i<getNbLinVars(); ++i)
   {
      LinVar v = getLinVar(i);
      os << v.getLB()   << " <= "
         << v.getName() << " <= "
         << v.getUB() << std::endl;
   }
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
   for (const LinCtr& c : ctrs_)
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

void LPModel::printCost(std::ostream& os) const
{
   os << sense_ << " ";
   printLinExpr(os, cost_);
   os << std::endl;
}

std::ostream& operator<<(std::ostream& os, const LPModel& model)
{
   model.printCost(os);
   model.printCtrs(os);
   model.printVars(os);
   return os;
}

LPStatus LPModel::optimize()
{
   return LPStatus::Other;
}

LPStatus LPModel::reoptimize()
{
   return optimize();
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

LinExpr LPModel::getCost() const
{
   return cost_;
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

double LPModel::getFeasilityTol() const
{
   return tol_;
}

void LPModel::setFeasilityTol(double tol)
{
   tol_ = tol;
}

double LPModel::certifiedCostSolution() const
{
   RealVector dual = dualSolution();
   IntervalMatrix M = matrix().transpose();
   IntervalVector B = bounds(),
                  C = cost(),
                  R = M*dual - C,
                  X = varBounds();

   ASSERT(X.isFinite(), "Certification impossible due to an infinite bound");

   Interval val = dual*B - R*X;
   return (getSense() == LPSense::Min) ? val.left() : val.right();
}

bool LPModel::isCertifiedInfeasible() const
{
   LOG_LOW("Certification of infeasible LP");

   RealVector ray(getNbLinVars()+getNbLinCtrs());
   bool hasRay = infeasibleRay(ray);

   if (!hasRay)
   {
      LOG_LOW("Dual ray not available");
      return false;
   }

  LOG_LOW("Dual ray: " << ray);

   IntervalMatrix M = matrix().transpose();
   IntervalVector R = M*ray,
                  B = bounds(),
                  X = varBounds();
   Interval val = R*X - ray*B;

   LOG_LOW("Interval value computed: " << val);

   if (val.containsZero())
   {
      LOG_LOW("Infeasibility not certified");
      return false;
   }
   else
   {
      LOG_LOW("Infeasibility certified");
      return true;
   }
}

IntervalMatrix LPModel::matrix() const
{
   int n = getNbLinCtrs(),
       m = getNbLinVars();
   IntervalMatrix A(m+n, m, Interval::zero());
   for (int i=0; i<m; ++i)
   {
      A.set(i, i, Interval::one());
   }
   for (int i=0; i<n; ++i)
   {
      const LinExpr& e = getLinCtr(i).getExpr();
      for (int j=0; j<e.getNbTerms(); ++j)
         A.set(i+m, e.getIndexVar(j), e.getCoef(j));
   }
   return A;
}

IntervalVector LPModel::cost() const
{
   int n = getNbLinVars();
   IntervalVector V(n, Interval::zero());
   for (int i=0; i<cost_.getNbTerms(); ++i)
   {
      int j = cost_.getIndexVar(i);
      V[j] = Interval(cost_.getCoef(i));
   }
   return V;
}

IntervalVector LPModel::varBounds() const
{
   int n = getNbLinVars();
   IntervalVector V(n);
   for (int i=0; i<n; ++i)
   {
      const LinVar& v = getLinVar(i);
      V[i] = Interval(v.getLB(), v.getUB());
   }
   return V;
}

IntervalVector LPModel::ctrBounds() const
{
   int m = getNbLinCtrs();
   IntervalVector V(m);
   for (int i=0; i<m; ++i)
   {
      const LinCtr& c = getLinCtr(i);
      V[i] = Interval(c.getLB(), c.getUB());
   }
   return V;
}

IntervalVector LPModel::bounds() const
{
   int n = getNbLinVars();
   int m = getNbLinCtrs();
   IntervalVector V(n+m);
   for (int i=0; i<n; ++i)
   {
      const LinVar& v = getLinVar(i);
      V[i] = Interval(v.getLB(), v.getUB());
   }
   for (int i=0; i<m; ++i)
   {
      const LinCtr& c = getLinCtr(i);
      V[i+n] = Interval(c.getLB(), c.getUB());
   }
   return V;
}

} // namespace
