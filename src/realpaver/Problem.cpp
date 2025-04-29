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
 * @file   Problem.cpp
 * @brief  Constraint satisfaction and optimization problem
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/ScopeBank.hpp"
#include <sstream>

namespace realpaver {

int Problem::NP = 0;

Problem::Problem(const std::string &name)
    : name_()
    , vars_()
    , ctrs_()
    , obj_(MIN(Term(0)))
    , scop_()
    , als_()
    , vname_()
    , erv_(std::make_shared<EntityReportedVector>())
    , id_(NP++)
{
}

Variable Problem::addVar(const std::string &name)
{
   size_t id = nextVarId();

   std::ostringstream os;

   if (name == "")
      os << "_v" << id;
   else
      os << name;

   checkSymbol(os.str());

   Variable v(os.str());
   v.setId(id).setTolerance(Tolerance(0.0, 0.0));

   pushVar(v);
   return v;
}

Variable Problem::addBinaryVar(const std::string &name)
{
   size_t id = nextVarId();

   std::ostringstream os;

   if (name == "")
      os << "_b" << id;
   else
      os << name;

   checkSymbol(os.str());

   std::unique_ptr<Domain> dom(new BinaryDomain());

   Variable v(os.str());
   v.setId(id).setDomain(std::move(dom)).setTolerance(Tolerance(0.0, 0.0));

   pushVar(v);
   return v;
}

VariableVector Problem::addBinaryVarVector(const std::string &name, int first, int last)
{
   VariableVector vec(name, first, last);

   for (int i = first; i <= last; ++i)
   {
      Variable v = vec[i];

      size_t id = nextVarId();
      std::unique_ptr<Domain> dom(new BinaryDomain());

      v.setId(id).setDomain(std::move(dom)).setTolerance(Tolerance(0.0, 0.0));

      pushVar(v);
   }

   return vec;
}

Variable Problem::addIntVar(int lo, int up, const std::string &name)
{
   return addIntVar(Range(lo, up), name);
}

Variable Problem::addIntVar(const Range &r, const std::string &name)
{
   THROW_IF(r.isEmpty(), "Integer variable with an empty domain");

   size_t id = nextVarId();

   std::ostringstream os;

   if (name == "")
      os << "_i" << id;
   else
      os << name;

   checkSymbol(os.str());

   std::unique_ptr<Domain> dom(new RangeDomain(r));

   Variable v(os.str());
   v.setId(id).setDomain(std::move(dom)).setTolerance(Tolerance(0.0, 0.0));

   pushVar(v);
   return v;
}

Variable Problem::addIntVar(const RangeUnion &u, const std::string &name)
{
   THROW_IF(u.isEmpty(), "Integer variable with an empty domain");

   size_t id = nextVarId();

   std::ostringstream os;

   if (name == "")
      os << "_i" << id;
   else
      os << name;

   checkSymbol(os.str());

   std::unique_ptr<Domain> dom(new RangeUnionDomain(u));

   Variable v(os.str());
   v.setId(id).setDomain(std::move(dom)).setTolerance(Tolerance(0.0, 0.0));

   pushVar(v);
   return v;
}

VariableVector Problem::addIntVarVector(const std::string &name, int first, int last,
                                        const Range &r)
{
   VariableVector vec(name, first, last);

   for (int i = first; i <= last; ++i)
   {
      Variable v = vec[i];
      size_t id = nextVarId();

      std::unique_ptr<Domain> dom(new RangeDomain(r));

      v.setId(id).setDomain(std::move(dom)).setTolerance(Tolerance(0.0, 0.0));

      pushVar(v);
   }

   return vec;
}

Variable Problem::addRealVar(double lo, double up, const std::string &name)
{
   return addRealVar(Interval(lo, up), name);
}

Variable Problem::addRealVar(const Interval &x, const std::string &name)
{
   THROW_IF(x.isEmpty(), "Real variable with an empty domain");

   size_t id = nextVarId();

   std::ostringstream os;

   if (name == "")
      os << "_x" << id;
   else
      os << name;

   checkSymbol(os.str());

   std::unique_ptr<Domain> dom(new IntervalDomain(x));

   double rtol = Params::GetDblParam("VAR_REL_TOL"),
          atol = Params::GetDblParam("VAR_ABS_TOL");

   Variable v(os.str());
   v.setId(id).setDomain(std::move(dom)).setTolerance(Tolerance(rtol, atol));

   pushVar(v);
   return v;
}

Variable Problem::addRealVar(const IntervalUnion &u, const std::string &name)
{
   THROW_IF(u.isEmpty(), "Real variable with an empty domain");

   size_t id = nextVarId();

   std::ostringstream os;

   if (name == "")
      os << "_x" << id;
   else
      os << name;

   checkSymbol(os.str());

   std::unique_ptr<Domain> dom(new IntervalUnionDomain(u));

   double rtol = Params::GetDblParam("VAR_REL_TOL"),
          atol = Params::GetDblParam("VAR_ABS_TOL");

   Variable v(os.str());
   v.setId(id).setDomain(std::move(dom)).setTolerance(Tolerance(rtol, atol));

   pushVar(v);
   return v;
}

VariableVector Problem::addRealVarVector(const std::string &name, int first, int last,
                                         const Interval &x)
{
   VariableVector vec(name, first, last);

   double rtol = Params::GetDblParam("VAR_REL_TOL"),
          atol = Params::GetDblParam("VAR_ABS_TOL");

   for (int i = first; i <= last; ++i)
   {
      Variable v = vec[i];
      size_t id = nextVarId();

      std::unique_ptr<Domain> dom(new IntervalDomain(x));

      v.setId(id).setDomain(std::move(dom)).setTolerance(Tolerance(rtol, atol));

      pushVar(v);
   }

   return vec;
}

Variable Problem::addClonedVar(Variable v)
{
   Variable res = v.clone();

   size_t id = nextVarId();
   res.setId(id);

   pushVar(res);
   return res;
}

void Problem::pushVar(Variable v)
{
   vars_.push_back(v);
   scop_.insert(v);
   erv_->addVariable(v);
}

void Problem::addCtr(Constraint c)
{
   ctrs_.push_back(c);
}

void Problem::addCtr(const std::initializer_list<Constraint> &l)
{
   for (const auto &c : l)
      addCtr(c);
}

void Problem::addObjective(Objective obj)
{
   obj_ = obj;
}

std::ostream &operator<<(std::ostream &os, const Problem &p)
{
   if (p.isEmpty())
      return os << "Nothing in this problem";

   std::string indent = "", s_var = "Variables", s_ctr = "Constraints", s_int = "Integer",
               s_obj = "Objectives", s_als = "Aliases";

   // variables
   bool first = true;
   os << s_var << std::endl;
   for (size_t i = 0; i < p.nbVars(); ++i)
   {
      if (!first)
         os << "," << std::endl;
      else
         first = false;

      os << p.varAt(i);
   }
   os << std::endl << ";";

   // constraints
   if (p.nbCtrs() > 0)
   {
      os << std::endl << s_ctr << std::endl;
      first = true;
      for (size_t i = 0; i < p.nbCtrs(); ++i)
      {
         if (!first)
            os << "," << std::endl;
         else
            first = false;

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

   // aliases
   if (p.nbAliases() > 0)
   {
      os << std::endl << s_als << std::endl;
      first = true;
      for (size_t i = 0; i < p.nbAliases(); ++i)
      {
         if (!first)
            os << "," << std::endl;
         else
            first = false;

         os << indent << p.aliasAt(i);
      }
      os << std::endl << ";";
   }

   return os;
}

bool Problem::isFakeVar(Variable v) const
{
   if (obj_.getTerm().dependsOn(v))
      return false;

   for (size_t i = 0; i < nbCtrs(); ++i)
      if (ctrAt(i).dependsOn(v))
         return false;

   return true;
}

bool Problem::isReal() const
{
   if (nbVars() == 0)
      return false;

   for (size_t i = 0; i < nbVars(); ++i)
      if (!varAt(i).isReal())
         return false;

   return true;
}

bool Problem::isInteger() const
{
   if (nbVars() == 0)
      return false;

   for (size_t i = 0; i < nbVars(); ++i)
      if (varAt(i).isReal())
         return false;

   return true;
}

bool Problem::isMixed() const
{
   if (nbVars() < 2)
      return false;

   bool cont = false, dis = false;

   for (size_t i = 0; i < nbVars(); ++i)
   {
      if (varAt(i).isInteger())
         dis = true;
      if (varAt(i).isReal())
         cont = true;

      if (dis && cont)
         return true;
   }
   return false;
}

bool Problem::isEqConstrained() const
{
   if (nbCtrs() == 0)
      return false;

   for (size_t i = 0; i < nbCtrs(); ++i)
      if (!ctrAt(i).isEquation())
         return false;

   return true;
}

bool Problem::isIneqConstrained() const
{
   if (nbCtrs() == 0)
      return false;

   for (size_t i = 0; i < nbCtrs(); ++i)
      if (!ctrAt(i).isInequality())
         return false;

   return true;
}

bool Problem::isMixedConstrained() const
{
   if (nbCtrs() < 2)
      return false;

   bool eq = false, ineq = false;

   for (size_t i = 0; i < nbCtrs(); ++i)
   {
      if (ctrAt(i).isEquation())
         eq = true;
      if (ctrAt(i).isInequality())
         ineq = true;

      if (eq && ineq)
         return true;
   }
   return false;
}

bool Problem::isLinConstrained() const
{
   if (nbCtrs() == 0)
      return false;

   for (size_t i = 0; i < nbCtrs(); ++i)
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
   return varAt(nbVars() - 1);
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
   if (scop_.size() != vars_.size())
   {
      Scope scop;

      for (const auto &v : vars_)
         scop.insert(v);

      Problem *prob = const_cast<Problem *>(this);
      prob->scop_ = ScopeBank::getInstance()->insertScope(scop);
   }
   return scop_;
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

void Problem::checkSymbol(const std::string &name)
{
   auto it = vname_.find(name);
   if (it != vname_.end())
      THROW("Symbol [" << name << "] already defined");
   else
      vname_.insert(name);
}

size_t Problem::nextVarId() const
{
   return vars_.size();
}

void Problem::addAlias(const Alias &a)
{
   checkSymbol(a.name());

   THROW_IF(!scop_.contains(a.scope()),
            "Alias " << a.name() << " not on the problem's scope");

   als_.push_back(a);
   erv_->addAlias(a);
}

size_t Problem::nbAliases() const
{
   return als_.size();
}

Alias Problem::aliasAt(size_t i) const
{
   ASSERT(i < als_.size(), "Bad access to aa alias in a problem");
   return als_[i];
}

void Problem::reportVariable(Variable v, bool b)
{
   if (erv_->contains(v.getName()))
   {
      if (!b)
         erv_->remove(v.getName());
   }
   else
   {
      if (b)
         erv_->addVariable(v);
   }
}

void Problem::reportAlias(Alias a, bool b)
{
   if (erv_->contains(a.name()))
   {
      if (!b)
         erv_->remove(a.name());
   }
   else
   {
      if (b)
         erv_->addAlias(a);
   }
}

bool Problem::isVarReported(const Variable &v) const
{
   return erv_->contains(v.getName());
}

bool Problem::isAliasReported(const Alias &a) const
{
   return erv_->contains(a.name());
}

} // namespace realpaver
