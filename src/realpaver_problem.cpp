// This file is part of Realpaver. License: see COPYING file.

#include <sstream>
#include "realpaver_constraint_fixer.hpp"
#include "realpaver_param.hpp"
#include "realpaver_problem.hpp"

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
    .setPrecision(Precision::absolute(0));

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
    .setPrecision(Precision::absolute(0));

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
    .setPrecision(Param::DefRealVarPrecision());

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

Box Problem::getBox() const
{
   Box B(vars_.size());
   
   for(size_t i=0; i<vars_.size(); ++i)
      B.set(i,vars_[i].domain());

   return B;
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
      os << v.name() << " in " << v.domain();
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

bool Problem::preprocess(const Box& B, Problem& other)
{
   LOG("   > simplifies the problem");

   // detects the fixed variables and creates the maps
   ConstraintFixer::VVMap vvm;
   ConstraintFixer::VIMap vim;

   for (size_t i=0; i<B.size(); ++i)
   {
      Variable v = varAt(i);

      if (B[i].isEmpty())
      {
         LOG("     - empty variable domain: " << v.name());
         return false;
      }
      else if (B[i].isCanonical())
      {
         LOG("     - replaces " << v.name() << " by " << B[i]);
         vim.insert(std::make_pair(v, B[i]));
      }
      else
      {
         // creates a clone of the variable in the other problem
         Variable w = v.clone();
         w.setId(other.nbVars());
         w.setDomain(B[i]);
         other.vars_.push_back(w);

         // new map entry
         vvm.insert(std::make_pair(v, w));
      }
   }

   Box oB = other.getBox();

   // rewrites the constraints
   for (size_t i=0; i<nbCtrs(); ++i)
   {
      ConstraintFixer fixer(&vvm, &vim);
      ctrAt(i).acceptVisitor(fixer);
      Constraint c = fixer.getConstraint();

      Proof proof = c.isSat(oB);
      if (proof == Proof::Empty)
      {
         LOG("     - violated constraint: " << ctrAt(i));
         return false;
      }
      else if (proof == Proof::Inner)
      {
         LOG("     - inactive constraint: " << ctrAt(i));
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
      LOG("     - fixed objective: " << fixer.getTerm());
   }
   
   other.addObj(Obj(obj_.getDir(), fixer.getTerm()));

   // detects fake variables
   for (size_t i=0; i<other.nbVars(); ++i)
   {
      Variable v = other.varAt(i);
      if (other.isFakeVar(v))
         LOG("     - unconstrained variable: " << v.name());
   }

   return true;
}

bool Problem::preprocess(Problem& other)
{
   Box V = getBox();
   return preprocess(V, other);
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

} // namespace
