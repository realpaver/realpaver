///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "realpaver/Double.hpp"
#include "realpaver/LPModel.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

LinVarRep::LinVarRep(int index, double lb, double ub,
                     bool continuous, const std::string& name) :
   index_(index),
   lb_(lb),
   ub_(ub),
   objval_(0.0),
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

void LinVarRep::setName(const std::string& name)
{
   name_ = name;
}

void LinVarRep::setObjVal(double val)
{
   objval_ = val;
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

std::string LinVarRep::getName() const
{
   return name_;
}

double LinVarRep::getObjVal() const
{
   return objval_;
}

///////////////////////////////////////////////////////////////////////////////

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

void LinVar::setName(const std::string& name)
{
   rep_->setName(name);
}

void LinVar::setObjVal(double val) const
{
   rep_->setObjVal(val);
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

std::string LinVar::getName() const
{
   return rep_->getName();
}

double LinVar::getObjVal() const
{
   return rep_->getObjVal();
}

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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

void LinExpr::addTerm(double a, LinVar v)
{
   rep_->addTerm(a, v);
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

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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
      os << "minimize ";
   else
      os << "maximize ";

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

} // namespace
