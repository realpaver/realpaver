///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintFixer.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Problem.hpp"

namespace realpaver {

Problem::Problem(const std::string& name)
      : name_(),
        vars_(),
        ctrs_(),
        obj_(MIN(Term(0))),
        scope_(),
        vname_(),
        dom_()
{}

Variable Problem::addBinaryVar(const std::string& name)
{
   size_t id = vars_.size();

   std::ostringstream os;

   if (name == "") os << "_b" << id;
   else            os << name;

   checkSymbol(os.str());

   Variable v(os.str());
   v.setId(id)
    .setDomain(Interval::zeroPlusOne())
    .setInteger()
    .setTolerance(Tolerance::makeAbs(0.0));

   vars_.push_back(v);
   scope_.insert(v);
   dom_.insert(std::make_pair(v, Interval::zeroPlusOne()));

   return v;
}

Variable Problem::addIntVar(int lo, int up, const std::string& name)
{
   return addIntVar(Interval(lo, up), name);
}

Variable Problem::addIntVar(const Interval& x, const std::string& name)
{
   Interval y = round(x);
   THROW_IF(y.isEmpty(), "Integer variable with an empty domain");
   
   size_t id = vars_.size();

   std::ostringstream os;

   if (name == "") os << "_i" << id;
   else            os << name;

   checkSymbol(os.str());

   Variable v(os.str());
   v.setId(id)
    .setDomain(y)
    .setInteger()
    .setTolerance(Tolerance::makeAbs(0.0));

   vars_.push_back(v);
   scope_.insert(v);
   dom_.insert(std::make_pair(v, y));

   return v;
}

Variable Problem::addRealVar(double lo, double up, const std::string& name)
{
   return addRealVar(Interval(lo, up), name);
}

Variable Problem::addRealVar(const Interval& x, const std::string& name)
{
   THROW_IF(x.isEmpty(), "Real variable with an empty domain");

   size_t id = vars_.size();

   std::ostringstream os;

   if (name == "") os << "_x" << id;
   else            os << name;
   
   checkSymbol(os.str());

   Variable v(os.str());
   v.setId(id)
    .setDomain(x)
    .setContinuous()
    .setTolerance(Param::GetTolParam("XTOL"));

   vars_.push_back(v);
   scope_.insert(v);
   dom_.insert(std::make_pair(v, x));

   return v;
}

void Problem::addCtr(Constraint c)
{
   ctrs_.push_back(c);
}

void Problem::addCtr(const std::initializer_list<Constraint>& l)
{
   for (const auto& c : l) addCtr(c);
}

void Problem::addObjective(Objective obj)
{
   obj_ = obj;
}

IntervalRegion Problem::getDomains() const
{
   IntervalRegion r(scope_);
   for (auto v : scope_) r.set(v, getDomain(v));
   return r;
}

Interval Problem::getDomain(Variable v) const
{
   ASSERT(scope_.contains(v),
          "Variable " << v.getName() << "does not belong to the problem");

   auto it = dom_.find(v);
   return it->second;
}

void Problem::setDomains(const IntervalRegion& reg)
{
   for (auto v : reg.scope()) setDomain(v, reg.get(v));
}

void Problem::setDomain(Variable v, const Interval& x)
{
   ASSERT(scope_.contains(v),
          "Variable " << v.getName() << "does not belong to the problem");

   auto it = dom_.find(v);
   it->second = x;
}

void Problem::setDomain(Variable v, double lo, double up)
{
   setDomain(v, Interval(lo, up));
}

std::ostream& operator<<(std::ostream& os, const Problem& p)
{
   if (p.isEmpty()) return os << "Nothing in this problem";
   
   std::string indent = "",
               s_var  = "Variables",
               s_ctr  = "Constraints",
               s_int  = "Integer",
               s_obj  = "Objectives";

   // variables
   bool first = true;
   bool intvar = false;
   os << s_var << std::endl;
   for (size_t i=0; i<p.nbVars(); ++i)
   {
      if (!first) os << "," << std::endl;
      else first = false;

      os << p.varAt(i);
   }
   os << std::endl << ";";

   // constraints
   if (p.nbCtrs() > 0)
   {
      os << std::endl << s_ctr << std::endl;
      first = true;
      for (size_t i=0; i<p.nbCtrs(); ++i)
      {
         if (!first) os << "," << std::endl;
         else first = false;

         os << indent << p.ctrAt(i);
      }
      os << std::endl << ";";
   }

   // objective function
   if (p.hasObjective())
   {
      os << std::endl << s_obj << std::endl;
      os << indent << p.getObjective() << std::endl << ";";
   }

   return os;
}

bool Problem::isFakeVar(Variable v) const
{
   if (obj_.getTerm().dependsOn(v))
      return false;
   
   for (size_t i=0; i<nbCtrs(); ++i)
      if (ctrAt(i).dependsOn(v))
         return false;

   return true;
}

bool Problem::isContinuous() const
{
   if (nbVars() == 0) return false;

   for (size_t i=0; i<nbVars(); ++i)
      if (!varAt(i).isContinuous())
         return false;

   return true;
}

bool Problem::isInteger() const
{
   if (nbVars() == 0) return false;

   for (size_t i=0; i<nbVars(); ++i)
      if (!varAt(i).isInteger())
         return false;

   return true;
}

bool Problem::isMixed() const
{
   if (nbVars() < 2) return false;

   bool cont = false, dis = false;

   for (size_t i=0; i<nbVars(); ++i)
   {
      if (varAt(i).isInteger()) dis = true;
      if (varAt(i).isContinuous()) cont = true;

      if (dis && cont) return true;
   }
   return false;
}

bool Problem::isEqConstrained() const
{
   if (nbCtrs() == 0) return false;

   for (size_t i=0; i<nbCtrs(); ++i)
      if (!ctrAt(i).isEquation())
         return false;

   return true;
}

bool Problem::isIneqConstrained() const
{
   if (nbCtrs() == 0) return false;

   for (size_t i=0; i<nbCtrs(); ++i)
      if (!ctrAt(i).isInequality())
         return false;

   return true;
}

bool Problem::isMixedConstrained() const
{
   if (nbCtrs() < 2) return false;

   bool eq = false, ineq = false;

   for (size_t i=0; i<nbCtrs(); ++i)
   {
      if (ctrAt(i).isEquation()) eq = true;
      if (ctrAt(i).isInequality()) ineq = true;

      if (eq && ineq) return true;
   }
   return false;
}

bool Problem::isLinConstrained() const
{
   if (nbCtrs() == 0) return false;

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
   ASSERT(vars_.size() > 0, "Bad access to a variable in an empty problem");
   ASSERT(i < vars_.size(), "Bad access to a variable in a problem");

   return vars_[i];
}

Variable Problem::lastVar() const
{
   return varAt(nbVars()-1);
}

size_t Problem::nbCtrs() const
{
   return ctrs_.size();
}

Constraint Problem::ctrAt(size_t i) const
{
   ASSERT(ctrs_.size() > 0, "Bad access to a constraint in an empty problem");
   ASSERT(i < ctrs_.size(), "Bad access to a constraint in a problem");

   return ctrs_[i];
}

Objective Problem::getObjective() const
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

bool Problem::isCSP() const
{
   return (nbVars() > 0) && (nbCtrs() > 0) && (!hasObjective());
}

bool Problem::isBOP() const
{
   return (nbVars() > 0) && (nbCtrs() == 0) && hasObjective();   
}

bool Problem::isCOP() const
{
   return (nbVars() > 0) && (nbCtrs() > 0) && hasObjective();      
}

bool Problem::isEmpty() const
{
   return (nbVars() == 0) && (nbCtrs() == 0) && (!hasObjective()); 
}

void Problem::checkSymbol(const std::string& name)
{
   auto it = vname_.find(name);
   if (it != vname_.end())
      THROW("Symbol [" << name << "] already defined");
   else
      vname_.insert(name);
}

} // namespace
