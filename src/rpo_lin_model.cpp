#include <iostream>
#include "rpo_lin_model.hpp"

////////////////////////////////////////////////////////////////////////

RpoLinVarRep::RpoLinVarRep(int index,
			   double lb, double ub,
			   bool continuous, const std::string& name) :
  index_(index),
  lb_(lb),
  ub_(ub),
  objval_(0.0),
  name_(name),
  continuous_(continuous)
{}

RpoLinVarRep::~RpoLinVarRep()
{}

void RpoLinVarRep::setContinuous()
{
  continuous_ = true;
}

void RpoLinVarRep::setInteger()
{
  continuous_ = false;
}

void RpoLinVarRep::setLB(double lb)
{
  lb_ = lb;
}

void RpoLinVarRep::setUB(double ub)
{
  ub_ = ub;
}

void RpoLinVarRep::setName(const std::string& name)
{
  name_ = name;
}

void RpoLinVarRep::setObjVal(double val)
{
  objval_ = val;
}

bool RpoLinVarRep::isContinuous() const
{
  return continuous_ == true;
}

bool RpoLinVarRep::isInteger() const
{
  return continuous_ == false;
}

int RpoLinVarRep::getIndex() const
{
  return index_;
}

double RpoLinVarRep::getLB() const
{
  return lb_;
}

double RpoLinVarRep::getUB() const
{
  return ub_;
}

std::string RpoLinVarRep::getName() const
{
  return name_;
}

double RpoLinVarRep::getObjVal() const
{
  return objval_;
}

////////////////////////////////////////////////////////////////////////

RpoLinVar::RpoLinVar(int index, double lb, double ub,
               bool continuous, const std::string& name):
  rep_(nullptr)
{
  rep_ = std::make_shared<RpoLinVarRep>(index, lb, ub, continuous, name);
}

void RpoLinVar::setContinuous()
{
  rep_->setContinuous();
}

void RpoLinVar::setInteger()
{
  rep_->setInteger();
}

void RpoLinVar::setLB(double lb)
{
  rep_->setLB(lb);
}

void RpoLinVar::setUB(double ub)
{
  rep_->setUB(ub);
}

void RpoLinVar::setName(const std::string& name)
{
  rep_->setName(name);
}

void RpoLinVar::setObjVal(double val) const
{
  rep_->setObjVal(val);
}

bool RpoLinVar::isContinuous() const
{
  return rep_->isContinuous();
}

bool RpoLinVar::isInteger() const
{
  return rep_->isInteger();
}

int RpoLinVar::getIndex() const
{
  return rep_->getIndex();
}

double RpoLinVar::getLB() const
{
  return rep_->getLB();
}

double RpoLinVar::getUB() const
{
  return rep_->getUB();
}

std::string RpoLinVar::getName() const
{
  return rep_->getName();
}

double RpoLinVar::getObjVal() const
{
  return rep_->getObjVal();
}

////////////////////////////////////////////////////////////////////////

RpoLinExprRep::RpoLinExprRep() :
  coefs_(),
  vars_()
{}

void RpoLinExprRep::addTerm(double a, RpoLinVar v)
{
  coefs_.push_back(a);
  vars_.push_back(v.getIndex());
}

int RpoLinExprRep::getNbTerms() const
{
  return coefs_.size();
}

double RpoLinExprRep::getCoef(int i) const
{
  return coefs_[i];
}

int RpoLinExprRep::getIndexVar(int i) const
{
  return vars_[i];
}

double* RpoLinExprRep::getCoefs() const
{
  return const_cast<double*>(coefs_.data());
}

int* RpoLinExprRep::getIndexVars() const
{
  return const_cast<int*>(vars_.data());
}

////////////////////////////////////////////////////////////////////////

RpoLinExpr::RpoLinExpr() :
  rep_(nullptr)
{
  rep_ = std::make_shared<RpoLinExprRep>();
}

RpoLinExpr::RpoLinExpr(const std::initializer_list<double>& lc,
                 const std::initializer_list<RpoLinVar>& lv)
{
  rep_ = std::make_shared<RpoLinExprRep>();

  auto ic = lc.begin();
  auto iv = lv.begin();

  for ( ; ic!=lc.end(); ++ic, ++iv)
    addTerm(*ic, *iv);
}

void RpoLinExpr::addTerm(double a, RpoLinVar v)
{
  rep_->addTerm(a, v);
}

int RpoLinExpr::getNbTerms() const
{
  return rep_->getNbTerms();
}

double RpoLinExpr::getCoef(int i) const
{
  return rep_->getCoef(i);
}

int RpoLinExpr::getIndexVar(int i) const
{
  return rep_->getIndexVar(i);
}

double* RpoLinExpr::getCoefs() const
{
  return rep_->getCoefs();
}

int* RpoLinExpr::getIndexVars() const
{
  return rep_->getIndexVars();
}

////////////////////////////////////////////////////////////////////////

RpoLinCtrRep::RpoLinCtrRep(double lb, RpoLinExpr e, double ub) :
  expr_(e),
  lb_(lb),
  ub_(ub)
{}

RpoLinCtrRep::RpoLinCtrRep(double lb, RpoLinExpr e) :
  expr_(e),
  lb_(lb),
  ub_(RPO_DBL_INF)
{}

RpoLinCtrRep::RpoLinCtrRep(RpoLinExpr e, double ub) :
  expr_(e),
  lb_(RPO_DBL_FNI),
  ub_(ub)
{}

void RpoLinCtrRep::setLB(double lb)
{
  lb_ = lb;
}

void RpoLinCtrRep::setUB(double ub)
{
  ub_ = ub;
}

RpoLinExpr RpoLinCtrRep::getExpr() const
{
  return expr_;
}

double RpoLinCtrRep::getLB() const
{
  return lb_;
}

double RpoLinCtrRep::getUB() const
{
  return ub_;
}

bool RpoLinCtrRep::isLessEqual() const
{
  return lb_ == RPO_DBL_FNI;
}

bool RpoLinCtrRep::isGreaterEqual() const
{
  return ub_ == RPO_DBL_INF;
}

bool RpoLinCtrRep::isEqual() const
{
  return lb_ == ub_;
}

bool RpoLinCtrRep::isRange() const
{
return lb_ != RPO_DBL_FNI && ub_ != RPO_DBL_INF && lb_ < ub_;
}

////////////////////////////////////////////////////////////////////////

RpoLinCtr::RpoLinCtr(double lb, RpoLinExpr e, double ub) :
  rep_(nullptr)
{
  rep_ = std::make_shared<RpoLinCtrRep>(lb, e, ub);
}

RpoLinCtr::RpoLinCtr(double lb, RpoLinExpr e) :
  rep_(nullptr)
{
  rep_ = std::make_shared<RpoLinCtrRep>(lb, e, RPO_DBL_INF);
}

RpoLinCtr::RpoLinCtr(RpoLinExpr e, double ub) :
  rep_(nullptr)
{
  rep_ = std::make_shared<RpoLinCtrRep>(RPO_DBL_FNI, e, ub);
}

void RpoLinCtr::setLB(double lb)
{
  rep_->setLB(lb);
}

void RpoLinCtr::setUB(double ub)
{
  rep_->setUB(ub);
}

RpoLinExpr RpoLinCtr::getExpr() const
{
  return rep_->getExpr();
}

double RpoLinCtr::getLB() const
{
  return rep_->getLB();
}

double RpoLinCtr::getUB() const
{
  return rep_->getUB();
}

bool RpoLinCtr::isLessEqual() const
{
  return rep_->isLessEqual();
}

bool RpoLinCtr::isGreaterEqual() const
{
  return rep_->isGreaterEqual();
}

bool RpoLinCtr::isEqual() const
{
  return rep_->isEqual();
}

bool RpoLinCtr::isRange() const
{
  return rep_->isRange();
}

////////////////////////////////////////////////////////////////////////

RpoLinModel::RpoLinModel() :
  vars_(),
  ctrs_(),
  obj_(),
  minimization_(true),
  objval_(0.0),
  maxseconds_(RPO_DBL_INF),
  maxiter_(RPO_MAX_INT)
{}

RpoLinModel::~RpoLinModel()
{}

RpoLinVar RpoLinModel::makeVar(double lb, double ub, const std::string& name)
{
  int index = vars_.size();
  bool continuous = true;
  vars_.push_back(RpoLinVar(index, lb, ub, continuous, name));
  return vars_.back();
}

void RpoLinModel::addCtr(double lb, RpoLinExpr e, double ub)
{
  ctrs_.push_back(RpoLinCtr(lb, e, ub));
}

void RpoLinModel::addCtr(double lb, RpoLinExpr e)
{

  ctrs_.push_back(RpoLinCtr(lb, e));
}

void RpoLinModel::addCtr(RpoLinExpr e, double ub)
{
  ctrs_.push_back(RpoLinCtr(e, ub));
}

void RpoLinModel::setObj(RpoLinExpr obj, bool minimization)
{
  obj_ = obj;
  minimization_ = minimization;
}

void RpoLinModel::setMinimize()
{
  minimization_ = true;
}

void RpoLinModel::setMaximize()
{
  minimization_ = false;
}

double RpoLinModel::getObjVal(RpoLinVar v) const
{
  return v.getObjVal();
}

double RpoLinModel::getObjVal() const
{
  return objval_;
}

void RpoLinModel::setObjVal(double val)
{
  objval_ = val;
}

void RpoLinModel::printVars(std::ostream& os) const
{
  for (RpoLinVar v : vars_)
    os << v.getLB()   << " <= "
       << v.getName() << " <= "
       << v.getUB()   << std::endl;
}

void RpoLinModel::printLinExpr(std::ostream& os, RpoLinExpr e) const
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

void RpoLinModel::printCtrs(std::ostream& os) const
{
  for (RpoLinCtr c : ctrs_)
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

void RpoLinModel::printObj(std::ostream& os) const
{
  if (minimization_)
    os << "minimize ";
  else
    os << "maximize ";

  printLinExpr(os, obj_);
  os << std::endl;
}

std::ostream& operator<<(std::ostream& os, const RpoLinModel& model)
{
  model.printObj(os);
  model.printCtrs(os);
  model.printVars(os);
  return os;
}

bool RpoLinModel::optimize()
{
  return false;
}

int RpoLinModel::getNbLinVars() const
{
  return vars_.size();
}

RpoLinVar RpoLinModel::getLinVar(int i) const
{
  return vars_[i];
}

int RpoLinModel::getNbLinCtrs() const
{
  return ctrs_.size();
}

RpoLinCtr RpoLinModel::getLinCtr(int i) const
{
  return ctrs_[i];
}

RpoLinExpr RpoLinModel::getObj() const
{
  return obj_;
}

bool RpoLinModel::isMinimization() const
{
  return minimization_;
}

bool RpoLinModel::isMaximization() const
{
  return !minimization_;
}

void RpoLinModel::setMaxSeconds(double s)
{
  maxseconds_ = s;
}

double RpoLinModel::getMaxSeconds() const
{
  return maxseconds_;
}

void RpoLinModel::setMaxIterations(int n)
{
  maxiter_ = n;
}

int RpoLinModel::getMaxIterations() const
{
  return maxiter_;
}
