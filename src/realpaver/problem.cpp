// This file is part of Realpaver. License: see COPYING file.

#include <sstream>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/constraint_fixer.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/param.hpp"
#include "realpaver/problem.hpp"

namespace realpaver {

Problem::Problem(const std::string& name) :
   name_(), vars_(), ctrs_(), obj_(Obj::Dir::Max, Term(0)), scope_()
{}

Variable Problem::addBoolVar(const std::string& name)
{
   size_t id = vars_.size();

   std::ostringstream os;

   if (name == "")
      os << "_b" << id;
   else
      os << name;
   
   Variable v(os.str());
   v.setId(id)
    .setDomain(Interval(0,1))
    .setDiscrete()
    .setTolerance(Tolerance::makeAbs(0.0));

   vars_.push_back(v);
   scope_.insert(v);

   return v;
}

Variable Problem::addIntVar(const int& a, const int& b,
                            const std::string& name)
{
   size_t id = vars_.size();

   std::ostringstream os;

   if (name == "")
      os << "_i" << id;
   else
      os << name;

   Variable v(os.str());
   v.setId(id)
    .setDomain(Interval(a,b))
    .setDiscrete()
    .setTolerance(Tolerance::makeAbs(0.0));

   vars_.push_back(v);
   scope_.insert(v);

   return v;
}

Variable Problem::addRealVar(const double& a, const double& b,
                             const std::string& name)
{
   return addRealVar(Interval(a,b), name);
}

Variable Problem::addRealVar(const Interval& x, const std::string& name)
{
   size_t id = vars_.size();

   std::ostringstream os;

   if (name == "")
      os << "_x" << id;
   else
      os << name;
   
   Variable v(os.str());
   v.setId(id)
    .setDomain(x)
    .setContinuous()
    .setTolerance(Param::DefRealVarPrecision());

   vars_.push_back(v);
   scope_.insert(v);

   return v;
}

void Problem::addCtr(const Constraint& c)
{
   ctrs_.push_back(c);
}

void Problem::addObj(const Obj& obj)
{
   obj_ = obj;
}

IntervalVector Problem::getDomains() const
{
   IntervalVector X(vars_.size());
   
   for(size_t i=0; i<vars_.size(); ++i)
      X.set(i, vars_[i].getDomain());

   return X;
}

std::ostream& operator<<(std::ostream& os, const Problem& p)
{
   std::string indent = "   ",
               s_int  = "int  ",
               s_real = "real ",
               s_var  = "Variables",
               s_ctr  = "Constraints",
               s_obj  = "Objective";

   // variables
   bool first = true;
   os << s_var << std::endl;
   for (size_t i=0; i<p.nbVars(); ++i)
   {
      if (!first) os << "," << std::endl;
      else first = false;

      Variable v = p.varAt(i);
      os << indent;
      if (v.isDiscrete())
         os << s_int;
      else
         os << s_real;
      os << v.getName() << " in " << v.getDomain();
   }
   os << ";" << std::endl << std::endl;

   // constraints
   os << s_ctr << std::endl;
   first = true;
   for (size_t i=0; i<p.nbCtrs(); ++i)
   {
      if (!first) os << "," << std::endl;
      else first = false;

      os << indent << p.ctrAt(i);
   }
   os << ";" << std::endl;

   // objective function
   Obj o = p.obj();
   os << std::endl << s_obj << std::endl;
   os << indent << o << ";" << std::endl;

   return os;
}

bool Problem::isFakeVar(const Variable& v) const
{
   if (obj_.getTerm().dependsOn(v))
      return false;
   
   for (size_t i=0; i<nbCtrs(); ++i)
      if (ctrAt(i).dependsOn(v))
         return false;

   return true;
}

bool Problem::preprocess(const IntervalVector& X, Problem& other)
{
   //LOG_INFO("   > simplifies the problem");

   // detects the fixed variables and creates the maps
   ConstraintFixer::VVMap vvm;
   ConstraintFixer::VIMap vim;

   for (size_t i=0; i<X.size(); ++i)
   {
      Variable v = varAt(i);

      if (X[i].isEmpty())
      {
         //LOG_INFO("     - empty variable domain: " << v.name());
         return false;
      }
      else if (X[i].isCanonical())
      {
         //LOG_INFO("     - replaces " << v.name() << " by " << B[i]);
         vim.insert(std::make_pair(v, X[i]));
      }
      else
      {
         // creates a clone of the variable in the other problem
         Variable w = v.clone();
         w.setId(other.nbVars());
         w.setDomain(X[i]);
         other.vars_.push_back(w);

         // new map entry
         vvm.insert(std::make_pair(v, w));
      }
   }

   IntervalVector oX = other.getDomains();

   // rewrites the constraints
   for (size_t i=0; i<nbCtrs(); ++i)
   {
      ConstraintFixer fixer(&vvm, &vim);
      ctrAt(i).acceptVisitor(fixer);
      Constraint c = fixer.getConstraint();

      Proof proof = c.isSat(oX);
      if (proof == Proof::Empty)
      {
         //LOG_INFO("     - violated constraint: " << ctrAt(i));
         return false;
      }
      else if (proof == Proof::Inner)
      {
         //LOG_INFO("     - inactive constraint: " << ctrAt(i));
      }
      else
      {
         other.ctrs_.push_back(c);
      }
   }

   // simplifies the objective function
   TermFixer fixer(&vvm, &vim);
   obj_.getTerm().acceptVisitor(fixer);
   
   if (!obj_.isConstant() && fixer.getTerm().isConstant())
   {
      //LOG_INFO("     - fixed objective: " << fixer.getTerm());
   }
   
   other.addObj(Obj(obj_.getDir(), fixer.getTerm()));

   // detects fake variables
   for (size_t i=0; i<other.nbVars(); ++i)
   {
      Variable v = other.varAt(i);
      //if (other.isFakeVar(v))
         //LOG_INFO("     - unconstrained variable: " << v.name());
   }

   return true;
}

bool Problem::preprocess(Problem& other)
{
   IntervalVector X = getDomains();
   return preprocess(X, other);
}

bool Problem::isContinuous() const
{
   for (size_t i=0; i<nbVars(); ++i)
      if (!varAt(i).isContinuous())
         return false;

   return true;
}

bool Problem::isDiscrete() const
{
   for (size_t i=0; i<nbVars(); ++i)
      if (!varAt(i).isDiscrete())
         return false;

   return true;
}

bool Problem::isMixed() const
{
   bool cont = false, dis = false;

   for (size_t i=0; i<nbVars(); ++i)
      if (varAt(i).isDiscrete())
         dis = true;
      else if (varAt(i).isContinuous())
         cont = true;

   return cont && dis;
}

bool Problem::isEqConstrained() const
{
   for (size_t i=0; i<nbCtrs(); ++i)
      if (!ctrAt(i).isEquation())
         return false;

   return true;
}

bool Problem::isIneqConstrained() const
{
   for (size_t i=0; i<nbCtrs(); ++i)
      if (!ctrAt(i).isInequality())
         return false;

   return true;
}

bool Problem::isMixedConstrained() const
{
   bool eq = false, ineq = false;

   for (size_t i=0; i<nbCtrs(); ++i)
   {
      if (ctrAt(i).isEquation()) eq = true;
      if (ctrAt(i).isInequality()) ineq = true;
   }

   return eq && ineq;
}

bool Problem::isLinConstrained() const
{
   for (size_t i=0; i<nbCtrs(); ++i)
      if (!ctrAt(i).isLinear())
         return false;

   return true;
}

size_t Problem::nbVars() const
{
   return vars_.size();
}

Variable Problem::varAt(size_t i) const
{
   ASSERT(i < vars_.size(), "bad access to the variables in a problem");

   return vars_[i];
}

size_t Problem::nbCtrs() const
{
   return ctrs_.size();
}

Constraint Problem::ctrAt(size_t i) const
{
   ASSERT(i < ctrs_.size(), "bad access to the constraints in a problem");

   return ctrs_[i];
}

Obj Problem::obj() const
{
   return obj_;
}

Scope Problem::scope() const
{
   return scope_;
}

bool Problem::isConstrained() const
{
   return nbCtrs() > 0;
}

bool Problem::hasObjective() const
{
   return !obj_.isConstant();
}

bool Problem::isLinObjective() const
{
   return hasObjective() && obj_.isLinear();
}

} // namespace
