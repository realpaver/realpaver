// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_LP_MODEL_HPP
#define REALPAVER_LP_MODEL_HPP

#include <iostream>
#include <memory>
#include <vector>

namespace realpaver {

/*****************************************************************************
 * Class of shared variable representations.
 *****************************************************************************/
class LinVarRep {
   friend class LinVar;

public:
   LinVarRep(int index, double lb, double ub, bool continuous, const std::string& name);
   ~LinVarRep();

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

/*****************************************************************************
 * Class of variables occurring in linear programming models.
 *****************************************************************************/
class LinVar {
   friend class LinExpr;
   friend class LPModel;

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
   std::shared_ptr<LinVarRep> rep_;

   LinVar(int index, double lb, double ub, bool continuous, const std::string& name);
};

/*****************************************************************************
 * Class of shared linear expression representations.
 *****************************************************************************/
class LinExprRep {
   friend class LinExpr;

public:
   LinExprRep();

   void addTerm(double a, LinVar v);

   int getNbTerms() const;
   double getCoef(int i) const;
   int getIndexVar(int i) const;

   double* getCoefs() const;
   int* getIndexVars() const;

private:
   std::vector<double> coefs_;
   std::vector<int> vars_;
};

/*****************************************************************************
 * Class of linear expressions.
 *****************************************************************************/
class LinExpr {
   friend class LPModel;

public:
   LinExpr();
   LinExpr(const std::initializer_list<double>& lc,
           const std::initializer_list<LinVar>& lv);

   void addTerm(double a, LinVar v);

   int getNbTerms() const;
   double getCoef(int i) const;
   int getIndexVar(int i) const;

   double* getCoefs() const;
   int* getIndexVars() const;

private:
   std::shared_ptr<LinExprRep> rep_;
};

/*****************************************************************************
 * Class of linear constraint representations.
 *****************************************************************************/
class LinCtrRep {
public:
   LinCtrRep(double lb, LinExpr e, double ub);
   LinCtrRep(double lb, LinExpr e);
   LinCtrRep(LinExpr e, double ub);

   void setLB(double lb);
   void setUB(double ub);

   LinExpr getExpr() const;
   double getLB() const;
   double getUB() const;

   bool isLessEqual() const;
   bool isGreaterEqual() const;
   bool isEqual() const;
   bool isRange() const;
  
private:
   LinExpr expr_;
   double lb_, ub_;
};

/*****************************************************************************
 * Class of linear constraints.
 *****************************************************************************/
class LinCtr {
public:
   LinCtr(double lb, LinExpr e, double ub);
   LinCtr(double lb, LinExpr e);
   LinCtr(LinExpr e, double ub);

   void setLB(double lb);
   void setUB(double ub);

   LinExpr getExpr() const;
   double getLB() const;
   double getUB() const;
  
   bool isLessEqual() const;
   bool isGreaterEqual() const;
   bool isEqual() const;
   bool isRange() const;

private:
   std::shared_ptr<LinCtrRep> rep_;
};

/*****************************************************************************
 * Class of linear programming models.
 *****************************************************************************/
class LPModel {
   friend class LinVarRep;

public:
   LPModel();
   virtual ~LPModel();

   LinVar makeVar(double lb, double ub, const std::string& name="");

   void addCtr(double lb, LinExpr e, double ub);
   void addCtr(double lb, LinExpr e);
   void addCtr(LinExpr e, double ub);

   void setObj(LinExpr obj, bool minimize = true);
   void setMinimization();
   void setMaximization();

   virtual bool optimize();

   double getObjVal(LinVar v) const;
   double getObjVal() const;

   void setObjVal(double val);

   friend std::ostream& operator<<(std::ostream& os, const LPModel& model);

   int getNbLinVars() const;
   LinVar getLinVar(int i) const;  

   int getNbLinCtrs() const;
   LinCtr getLinCtr(int i) const;

   LinExpr getObj() const;
   bool isMinimization() const;
   bool isMaximization() const;

   // timeout in seconds
   void setMaxSeconds(double s);
   double getMaxSeconds() const;

   // maximum number of iterations
   void setMaxIterations(int n);
   int getMaxIterations() const;
  
private:
   std::vector<LinVar> vars_;
   std::vector<LinCtr> ctrs_;
   LinExpr obj_;
   bool minimization_;
   double objval_;
   double maxseconds_;
   int maxiter_;
  
   void printLinExpr(std::ostream& os, LinExpr e) const;
   void printVars(std::ostream& os) const;
   void printCtrs(std::ostream& os) const;
   void printObj(std::ostream& os) const;
};

} // namespace

#endif
