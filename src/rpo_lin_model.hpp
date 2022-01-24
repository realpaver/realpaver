#ifndef RPO_LIN_MODEL_HPP
#define RPO_LIN_MODEL_HPP

#include <iostream>
#include <memory>
#include <vector>

// TODO
#define RPO_DBL_FNI -1000
#define RPO_DBL_INF 1000
#define RPO_MAX_INT 100000

////////////////////////////////////////////////////////////////////////

class RpoLinVarRep {
  friend class RpoLinVar;

public:
  RpoLinVarRep(int index, double lb, double ub, bool continuous, const std::string& name);
  ~RpoLinVarRep();

  void setContinuous();
  void setInteger();
  void setLB(double lb);
  void setUB(double ub);
  void setName(const std::string& name);
  void setObjVal(double val);

  bool isContinuous() const;
  bool isInteger() const;
  int getIndex() const;
  double getLB() const;
  double getUB() const;
  std::string getName() const;
  double getObjVal() const;
  
private:
  int index_;
  double lb_, ub_, objval_;
  std::string name_;
  bool continuous_;
};

////////////////////////////////////////////////////////////////////////

class RpoLinVar {
  friend class RpoLinExpr;
  friend class RpoLinModel;

public:
  void setContinuous();
  void setInteger();
  void setLB(double lb);
  void setUB(double ub);
  void setName(const std::string& name);
  void setObjVal(double val) const;

  bool isContinuous() const;
  bool isInteger() const;
  int getIndex() const;
  double getLB() const;
  double getUB() const;
  std::string getName() const;  
  double getObjVal() const;

private:
  std::shared_ptr<RpoLinVarRep> rep_;

  RpoLinVar(int index, double lb, double ub, bool continuous, const std::string& name);
};

////////////////////////////////////////////////////////////////////////

class RpoLinExprRep {
  friend class RpoLinExpr;

public:
  RpoLinExprRep();

  void addTerm(double a, RpoLinVar v);

  int getNbTerms() const;
  double getCoef(int i) const;
  int getIndexVar(int i) const;

  double* getCoefs() const;
  int* getIndexVars() const;

private:
  std::vector<double> coefs_;
  std::vector<int> vars_;
};

////////////////////////////////////////////////////////////////////////

class RpoLinExpr {
  friend class RpoLinModel;

public:
  RpoLinExpr();
  RpoLinExpr(const std::initializer_list<double>& lc,
          const std::initializer_list<RpoLinVar>& lv);

  void addTerm(double a, RpoLinVar v);

  int getNbTerms() const;
  double getCoef(int i) const;
  int getIndexVar(int i) const;

  double* getCoefs() const;
  int* getIndexVars() const;

private:
  std::shared_ptr<RpoLinExprRep> rep_;
};

////////////////////////////////////////////////////////////////////////

class RpoLinCtrRep {
public:
  RpoLinCtrRep(double lb, RpoLinExpr e, double ub);
  RpoLinCtrRep(double lb, RpoLinExpr e);
  RpoLinCtrRep(RpoLinExpr e, double ub);

  void setLB(double lb);
  void setUB(double ub);

  RpoLinExpr getExpr() const;
  double getLB() const;
  double getUB() const;

  bool isLessEqual() const;
  bool isGreaterEqual() const;
  bool isEqual() const;
  bool isRange() const;
  
private:
  RpoLinExpr expr_;
  double lb_, ub_;
};

////////////////////////////////////////////////////////////////////////

class RpoLinCtr {
public:
  RpoLinCtr(double lb, RpoLinExpr e, double ub);
  RpoLinCtr(double lb, RpoLinExpr e);
  RpoLinCtr(RpoLinExpr e, double ub);

  void setLB(double lb);
  void setUB(double ub);

  RpoLinExpr getExpr() const;
  double getLB() const;
  double getUB() const;
  
  bool isLessEqual() const;
  bool isGreaterEqual() const;
  bool isEqual() const;
  bool isRange() const;

private:
  std::shared_ptr<RpoLinCtrRep> rep_;
};

////////////////////////////////////////////////////////////////////////

class RpoLinModel {
  friend class RpoLinVarRep;

public:
  RpoLinModel();
  virtual ~RpoLinModel();

  RpoLinVar makeVar(double lb, double ub, const std::string& name="");

  void addCtr(double lb, RpoLinExpr e, double ub);
  void addCtr(double lb, RpoLinExpr e);
  void addCtr(RpoLinExpr e, double ub);

  void setObj(RpoLinExpr obj, bool minimize = true);
  void setMinimize();
  void setMaximize();

  virtual bool optimize();

  double getObjVal(RpoLinVar v) const;
  double getObjVal() const;

  void setObjVal(double val);

  friend std::ostream& operator<<(std::ostream& os, const RpoLinModel& model);

  int getNbLinVars() const;
  RpoLinVar getLinVar(int i) const;  

  int getNbLinCtrs() const;
  RpoLinCtr getLinCtr(int i) const;

  RpoLinExpr getObj() const;
  bool isMinimization() const;
  bool isMaximization() const;

  // timeout in seconds
  void setMaxSeconds(double s);
  double getMaxSeconds() const;

  // maximum number of iterations
  void setMaxIterations(int n);
  int getMaxIterations() const;
  
private:
  std::vector<RpoLinVar> vars_;
  std::vector<RpoLinCtr> ctrs_;
  RpoLinExpr obj_;
  bool minimization_;
  double objval_;
  double maxseconds_;
  int maxiter_;
  
  void printLinExpr(std::ostream& os, RpoLinExpr e) const;
  void printVars(std::ostream& os) const;
  void printCtrs(std::ostream& os) const;
  void printObj(std::ostream& os) const;
};

#endif
