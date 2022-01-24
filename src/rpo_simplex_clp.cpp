#include "rpo_simplex_clp.hpp"

RpoSimplex::RpoSimplex() :
  simplex_(nullptr)
{
  simplex_ = new ClpSimplex();
}

RpoSimplex::~RpoSimplex()
{
  if (simplex_!= nullptr)
  delete simplex_;
}

void RpoSimplex::makeVars()
{
  int n = getNbLinVars();
  simplex_->resize(0, n);

  for (int i=0; i<n; ++i)
  {
    RpoLinVar v = getLinVar(i);
    int j = v.getIndex();
    simplex_->setColumnLower(j, v.getLB());
    simplex_->setColumnUpper(j, v.getUB());

    if (v.isContinuous())
      simplex_->setContinuous(j);
    else
      simplex_->setInteger(j);
  }
}

void RpoSimplex::makeCtrs()
{
  int m = getNbLinCtrs();
  for (int i=0; i<m; ++i)
  {
    RpoLinCtr c = getLinCtr(i);
    RpoLinExpr e = c.getExpr();

    simplex_->addRow(e.getNbTerms(), e.getIndexVars(), e.getCoefs(),
		     c.getLB(), c.getUB());
  }
}

void RpoSimplex::makeObj()
{
  RpoLinExpr obj = getObj();
  int n = obj.getNbTerms();

  for (int i=0; i<n; ++i)
    simplex_->setObjectiveCoefficient(obj.getIndexVar(i), obj.getCoef(i));

  int sense = isMinimization() ? 1.0: -1.0;
  simplex_->setOptimizationDirection(sense);
}

void RpoSimplex::makeClpSimplex()
{
  simplex_->setMaximumSeconds(getMaxSeconds());
  simplex_->setMaximumIterations(getMaxIterations());
  
  makeVars();
  makeCtrs();
  makeObj();
}

bool RpoSimplex::optimize()
{
  makeClpSimplex();

  simplex_->initialSolve();
  if (simplex_->isProvenOptimal())
  {
    setObjVal(simplex_->getObjValue());

    int n = getNbLinVars();
    double* sol = simplex_->primalColumnSolution();

    for (int i=0; i<n; ++i)
    {
      RpoLinVar v = getLinVar(i);
      v.setObjVal(sol[i]);
    }

    return true;
  }
  else
    return false;
}
