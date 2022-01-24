#include "rpo_simplex_gurobi.hpp"

RpoSimplex::RpoSimplex() :
  env_(nullptr),
  simplex_(nullptr),
  vars_()
{
  env_ = new GRBEnv();
  simplex_ = new GRBModel(env_);
}

RpoSimplex::~RpoSimplex()
{
  if (simplex_ != nullptr)
    delete simplex_;

  if (env_ != nullptr)
    delete env_;
}

void RpoSimplex::makeVars()
{
  int n = getNbLinVars();

  for (int i=0; i<n; ++i)
  {
    RpoLinVar v = getLinVar(i);
    char type = v.isContinuous() ? GRB_CONTINUOUS : GRB_INTEGER;

    GRBVar gv = simplex_->addVar(v.getLB(), v.getUB(), 0.0, type, v.getName());
    vars_.push_back(gv);
  }
}

GRBLinExpr RpoSimplex::makeGrbLinExpr(RpoLinExpr e)
{
  GRBLinExpr ge;

  int n = e.getNbTerms();
  for (int i=0; i<n; ++i)
  {
    int j = e.getIndexVar(i);
    double coef = e.getCoef(i);

    GRBVar gv = vars_[j];
    ge += coef*gv;
  }

  return ge;
}

void RpoSimplex::makeCtrs()
{
  int m = getNbLinCtrs();
  for (int i=0; i<m; ++i)
  {
    RpoLinCtr c = getLinCtr(i);
    RpoLinExpr e = c.getExpr();

    GRBLinExpr ge = makeGrbLinExpr(e);

    if (c.isLessEqual())
      simplex_->addConstr(ge, GRB_LESS_EQUAL, c.getUB());

    else if (c.isGreaterEqual())
      simplex_->addConstr(ge, GRB_GREATER_EQUAL, c.getLB());

    else if (c.isEqual())
      simplex_->addConstr(ge, GRB_EQUAL, c.getLB());

    else
    {
      simplex_->addConstr(ge, GRB_LESS_EQUAL, c.getUB());
      simplex_->addConstr(ge, GRB_GREATER_EQUAL, c.getLB());
    }
  }
}

void RpoSimplex::makeObj()
{
  RpoLinExpr e = getObj();
  GRBLinExpr ge = makeGrbLinExpr(e);

  int sense = isMinimization() ? GRB_MINIMIZE : GRB_MAXIMIZE;
  simplex_->setObjective(ge, sense);
}

void RpoSimplex::makeGurobiSimplex()
{
  env_->set(GRB_DoubleParam_TimeLimit, getMaxSeconds());  
  env_->set(GRB_DoubleParam_IterationLimit, getMaxIterations());  

  makeVars();
  makeCtrs();
  makeObj();
}

bool RpoSimplex::optimize()
{
  makeGurobiSimplex();

  simplex_->optimize();
  bool optimal = simplex_->get(GRB_IntAttr_Status) == GRB_OPTIMAL;

  if (optimal)
  {
    setObjVal(simplex_->get(GRB_DoubleAttr_ObjVal));
    
    int n = getNbLinVars();
    for (int i=0; i<n; ++i)
    {
      double val = vars_[i].get(GRB_DoubleAttr_X);
      RpoLinVar v = getLinVar(i);
      v.setObjVal(val);
    }
  }

  return optimal;
}
