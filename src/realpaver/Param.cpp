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
 * @file   Param.cpp
 * @brief  Parameters of solvers
 * @author Laurent Granvilliers
 * @date   2025-4-2
 */

#include "realpaver/Double.hpp"
#include "realpaver/Exception.hpp"
#include "realpaver/Interval.hpp"
#include "realpaver/Param.hpp"
#include <algorithm>
#include <fstream>
#include <limits>
#include <ostream>
#include <sstream>

namespace realpaver {

std::ostream &operator<<(std::ostream &os, const ParamCat &cat)
{
   switch (cat)
   {
   case ParamCat::General:
      return os << "General parameters";
   case ParamCat::Branch:
      return os << "Branch-and-contract algorithms";
   case ParamCat::Split:
      return os << "Splitting strategies";
   case ParamCat::Contract:
      return os << "Contractors and propagation";
   case ParamCat::Polytope:
      return os << "Polytope hull contractor";
   case ParamCat::Acid:
      return os << "ACID contractor";
   case ParamCat::LocalOptim:
      return os << "Local optimization";
   case ParamCat::LinearSolver:
      return os << "Linear solvers";
   case ParamCat::Interval:
      return os << "Interval methods";
   case ParamCat::NoCat:
      return os << "Not a category";
   default:
      os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

Param::Param()
    : name_("")
    , cat_(ParamCat::General)
    , what_("")
    , text_("")
{
}

Param::~Param()
{
}

const std::string &Param::name() const
{
   return name_;
}

Param &Param::setName(const std::string &name)
{
   name_ = name;
   return *this;
}

const ParamCat &Param::cat() const
{
   return cat_;
}

Param &Param::setCat(const ParamCat &cat)
{
   cat_ = cat;
   return *this;
}

const std::string &Param::what() const
{
   return what_;
}

Param &Param::setWhat(const std::string &what)
{
   what_ = what;
   return *this;
}

const std::string &Param::text() const
{
   return text_;
}

Param &Param::setText(const std::string &str)
{
   text_ = str;
   return *this;
}

std::vector<std::string> Param::splitStr(const std::string &str)
{
   std::vector<std::string> v;
   size_t pos = 0, l = str.length();
   while (pos < l)
   {
      size_t next = std::min(l, str.find_first_of('\n', pos));
      v.push_back(str.substr(pos, next - pos));
      pos = next + 1;
   }
   return v;
}

void Param::printStr(std::ostream &os, const std::string &str, size_t ncols)
{
   if (maxWordLength(str) > ncols - 2)
      THROW("Word too long or number of columns too small");

   std::vector<std::string> lines = splitStr(str);

   for (const auto &line : lines)
   {
      bool item = line[0] == '-';
      size_t nc = ncols - 2;
      size_t l = line.length(), first = 0, last = std::min(l - 1, first + nc - 1);
      while (first < l)
      {
         if (last < l - 1 && line[last + 1] != ' ')
         {
            while (line[last] != ' ')
               last--;
            last--;
         }

         os << "# ";
         if (first != 0 && item)
            os << "  ";
         for (int i = first; i <= last; ++i)
            os << line[i];
         os << std::endl;

         first = last + 2;
         last = std::min(l - 1, first + nc - 1);
      }
   }
}

size_t Param::maxWordLength(const std::string &str)
{
   size_t lmax = 0, l = 0;
   for (size_t i = 0; i < str.length(); ++i)
   {
      if (str[i] == ' ')
      {
         if (lmax < l)
            lmax = l;
         l = 0;
      }
      else
      {
         l++;
      }
   }
   return std::max(lmax, l);
}

/*----------------------------------------------------------------------------*/

ParamDbl::ParamDbl()
    : Param()
    , val_(0.0)
    , min_(-Double::inf())
    , max_(Double::inf())
{
}

ParamDbl &ParamDbl::setValue(const double &a)
{
   val_ = a;
   check();
   return *this;
}

ParamDbl &ParamDbl::setMinValue(const double &a)
{
   min_ = a;
   check();
   return *this;
}

ParamDbl &ParamDbl::setMaxValue(const double &a)
{
   max_ = a;
   check();
   return *this;
}

const double &ParamDbl::value() const
{
   return val_;
}

const double &ParamDbl::minValue() const
{
   return min_;
}

const double &ParamDbl::maxValue() const
{
   return max_;
}

ParamDbl &ParamDbl::setName(const std::string &name)
{
   Param::setName(name);
   return *this;
}

ParamDbl &ParamDbl::setCat(const ParamCat &cat)
{
   Param::setCat(cat);
   return *this;
}

ParamDbl &ParamDbl::setWhat(const std::string &what)
{
   Param::setWhat(what);
   return *this;
}

ParamDbl &ParamDbl::setText(const std::string &str)
{
   Param::setText(str);
   return *this;
}

ParamDbl *ParamDbl::clone() const
{
   ParamDbl *prm = new ParamDbl();
   prm->setName(name_)
       .setCat(cat_)
       .setWhat(what_)
       .setText(text_)
       .setValue(val_)
       .setMinValue(min_)
       .setMaxValue(max_);
   return prm;
}

void ParamDbl::printValue(std::ostream &os, int spaces) const
{
   os << name_;
   for (int i = 0; i < spaces; ++i)
      os << ' ';
   os << " = " << val_;
}

void ParamDbl::print(std::ostream &os, size_t ncols) const
{
   printStr(os, what_, ncols);
   os << "# Range of possible real values: [" << min_ << ", " << max_ << "]" << std::endl;
   if (text_ != "")
      printStr(os, text_, ncols);
   printValue(os, 0);
   os << std::endl;
}

void ParamDbl::check() const
{
   if (val_ < min_ || val_ > max_)
      THROW("Value " << val_ << " of real parameter " << name_ << "out of range");
}

/*----------------------------------------------------------------------------*/

ParamInt::ParamInt()
    : Param()
    , val_(0.0)
    , min_(std::numeric_limits<int>::min())
    , max_(std::numeric_limits<int>::max())
{
}

ParamInt &ParamInt::setValue(const int &a)
{
   val_ = a;
   check();
   return *this;
}

ParamInt &ParamInt::setMinValue(const int &a)
{
   min_ = a;
   check();
   return *this;
}

ParamInt &ParamInt::setMaxValue(const int &a)
{
   max_ = a;
   check();
   return *this;
}

const int &ParamInt::value() const
{
   return val_;
}

const int &ParamInt::minValue() const
{
   return min_;
}

const int &ParamInt::maxValue() const
{
   return max_;
}

ParamInt &ParamInt::setName(const std::string &name)
{
   Param::setName(name);
   return *this;
}

ParamInt &ParamInt::setCat(const ParamCat &cat)
{
   Param::setCat(cat);
   return *this;
}

ParamInt &ParamInt::setWhat(const std::string &what)
{
   Param::setWhat(what);
   return *this;
}

ParamInt &ParamInt::setText(const std::string &str)
{
   Param::setText(str);
   return *this;
}

ParamInt *ParamInt::clone() const
{
   ParamInt *prm = new ParamInt();
   prm->setName(name_)
       .setCat(cat_)
       .setWhat(what_)
       .setText(text_)
       .setValue(val_)
       .setMinValue(min_)
       .setMaxValue(max_);
   return prm;
}

void ParamInt::printValue(std::ostream &os, int spaces) const
{
   os << name_;
   for (int i = 0; i < spaces; ++i)
      os << ' ';
   os << " = " << val_;
}

void ParamInt::print(std::ostream &os, size_t ncols) const
{
   printStr(os, what_, ncols);
   os << "# Range of possible int values: [" << min_ << ".." << max_ << "]" << std::endl;
   if (text_ != "")
      printStr(os, text_, ncols);
   printValue(os, 0);
   os << std::endl;
}

void ParamInt::check() const
{

   if (val_ < min_ || val_ > max_)
      THROW("Value " << val_ << " of int parameter " << name_ << " out of range");
}

/*----------------------------------------------------------------------------*/

ParamStr::ParamStr()
    : Param()
    , val_("")
    , cho_()
{
}

ParamStr &ParamStr::setValue(const std::string &str)
{
   val_ = str;
   check();
   return *this;
}

const std::string &ParamStr::value() const
{
   return val_;
}

ParamStr &ParamStr::setName(const std::string &name)
{
   Param::setName(name);
   return *this;
}

ParamStr &ParamStr::setCat(const ParamCat &cat)
{
   Param::setCat(cat);
   return *this;
}

ParamStr &ParamStr::setWhat(const std::string &what)
{
   Param::setWhat(what);
   return *this;
}

ParamStr &ParamStr::setText(const std::string &str)
{
   Param::setText(str);
   return *this;
}

ParamStr &ParamStr::addChoice(const std::string &val, const std::string &what)
{
   Choice c = {val, what};
   cho_.push_back(c);
   return *this;
}

ParamStr *ParamStr::clone() const
{
   ParamStr *prm = new ParamStr();
   for (const auto &x : cho_)
      prm->addChoice(x.val, x.what);

   prm->setName(name_).setCat(cat_).setWhat(what_).setText(text_).setValue(val_);

   return prm;
}

void ParamStr::printValue(std::ostream &os, int spaces) const
{
   os << name_;
   for (int i = 0; i < spaces; ++i)
      os << ' ';
   os << " = " << val_;
}

void ParamStr::print(std::ostream &os, size_t ncols) const
{
   printStr(os, what_, ncols);
   os << "# List of possible values: " << std::endl;
   for (const auto &x : cho_)
   {
      std::ostringstream ostr;
      ostr << "- " << x.val << ": " << x.what;
      printStr(os, ostr.str(), ncols);
   }
   if (text_ != "")
      printStr(os, text_, ncols);
   printValue(os, 0);
   os << std::endl;
}

void ParamStr::check() const
{
   for (const auto &x : cho_)
      if (val_ == x.val)
         return;

   THROW("Value " << val_ << " of string parameter " << name_ << " not a valid choice");
}

/*----------------------------------------------------------------------------*/

Params Params::instance_;

double Params::GetDblParam(const std::string &name)
{
   return instance_.getDblParam(name);
}

const std::string &Params::GetStrParam(const std::string &name)
{
   return instance_.getStrParam(name);
}

int Params::GetIntParam(const std::string &name)
{
   return instance_.getIntParam(name);
}

void Params::Print(std::ostream &os)
{
   instance_.print(os);
}

Params::Params()
    : map_()
    , ncols_(80)
    , path_("")
    , lineno_(0)
{
   makeGeneral();
   makeBranch();
   makeSplit();
   makeContract();
   makePolytope();
   makeAcid();
   makeLocalOptim();
   makeLinearSolver();
   makeInterval();
}

Params::~Params()
{
   for (auto &x : map_)
      delete x.second;
}

Params::Params(const Params &other)
{
   for (const auto &x : other.map_)
      map_.insert(std::make_pair(x.first, x.second->clone()));
}

Params &Params::operator=(const Params &other)
{
   for (auto &x : map_)
      delete x.second;
   map_.clear();

   for (const auto &x : other.map_)
      map_.insert(std::make_pair(x.first, x.second->clone()));

   return *this;
}

size_t Params::nbColumns() const
{
   return ncols_;
}

void Params::setNbColumns(size_t col)
{
   ncols_ = col;
}

double Params::getDblParam(const std::string &name) const
{
   auto it = map_.find(name);
   if (it == map_.end())
      THROW(name << " is not a parameter");

   const ParamDbl *prm = dynamic_cast<const ParamDbl *>(it->second);
   if (prm == nullptr)
      THROW(name << " is not a parameter of type double");

   return prm->value();
}

void Params::setDblParam(const std::string &name, const double &val)
{
   auto it = map_.find(name);
   if (it == map_.end())
      THROW(name << " is not a parameter");

   ParamDbl *prm = dynamic_cast<ParamDbl *>(it->second);
   if (prm == nullptr)
      THROW(name << " is not a parameter of type double");

   Interval x(val);
   if (x.isEmpty())
      THROW("The value " << val << " of " << name << " is not valid");

   prm->setValue(x.right());
}

const std::string &Params::getStrParam(const std::string &name) const
{
   auto it = map_.find(name);
   if (it == map_.end())
      THROW(name << " is not a parameter");

   const ParamStr *prm = dynamic_cast<const ParamStr *>(it->second);
   if (prm == nullptr)
      THROW(name << " is not a parameter of type string");

   return prm->value();
}

void Params::setStrParam(const std::string &name, const std::string &val)
{
   auto it = map_.find(name);
   if (it == map_.end())
      THROW(name << " is not a parameter");

   ParamStr *prm = dynamic_cast<ParamStr *>(it->second);
   if (prm == nullptr)
      THROW(name << " is not a parameter of type string");

   prm->setValue(val);
}

int Params::getIntParam(const std::string &name) const
{
   auto it = map_.find(name);
   if (it == map_.end())
      THROW(name << " is not a parameter");

   const ParamInt *prm = dynamic_cast<const ParamInt *>(it->second);
   if (prm == nullptr)
      THROW(name << " is not a parameter of type int");

   return prm->value();
}

void Params::setIntParam(const std::string &name, const int &val)
{
   auto it = map_.find(name);
   if (it == map_.end())
      THROW(name << " is not a parameter");

   ParamInt *prm = dynamic_cast<ParamInt *>(it->second);
   if (prm == nullptr)
      THROW(name << " is not a parameter of type int");

   prm->setValue(val);
}

void Params::add(const Param &prm)
{
   auto it = map_.find(prm.name());
   if (it != map_.end())
      map_.erase(it);
   map_.insert(std::make_pair(prm.name(), prm.clone()));
}

void Params::printValues(std::ostream &os) const
{
   size_t lmax = maxLength();
   std::vector<Param *> v = sort();

   for (const auto &x : v)
   {
      size_t l = x->name().length();
      x->printValue(os, lmax - l);
      os << std::endl;
   }
}

void Params::print(std::ostream &os) const
{
   ParamCat cat = ParamCat::NoCat;
   std::vector<Param *> v = sort();

   for (const auto &x : v)
   {
      if (x->cat() != cat)
      {
         if (cat != ParamCat::NoCat)
            os << std::endl;

         cat = x->cat();
         os << "# ----------------------------------------------------------------------"
            << std::endl
            << "# " << cat << std::endl
            << "# ----------------------------------------------------------------------"
            << std::endl;
      }

      os << std::endl;
      x->print(os, ncols_);
   }
}

size_t Params::maxLength() const
{
   size_t lmax = 0;
   for (const auto &x : map_)
   {
      size_t l = x.first.length();
      if (l > lmax)
         lmax = l;
   }
   return lmax;
}

std::vector<Param *> Params::sort() const
{
   std::vector<Param *> v;
   for (auto &x : map_)
      v.push_back(x.second);

   std::sort(v.begin(), v.end(), ParamComparator());

   return v;
}

size_t Params::eatSpaces(const std::string &line, size_t first)
{
   size_t i = first;

   while (i < line.size() && isSpace(line[i]))
      ++i;

   return i;
}

bool Params::isDigit(char c)
{
   return c >= '0' && c <= '9';
}

bool Params::isSpace(char c)
{
   return c == ' ' || c == '\t' || c == '\r';
}

bool Params::isLetter(char c)
{
   return c >= 'A' && c <= 'Z';
}

bool Params::isCharName(char c)
{
   return isLetter(c) || isDigit(c) || c == '_';
}

bool Params::isInt(const std::string &s)
{
   if (s.size() == 0)
      return false;

   size_t i = 0;
   if ((s[0] == '+') || (s[0] == '-'))
      ++i;

   if (i == s.size())
      return false;

   for (size_t j = i; j < s.size(); ++j)
      if (!isDigit(s[j]))
         return false;

   return true;
}

void Params::throwParsing()
{
   THROW("Error in parameter file '" << path_ << "' at line " << lineno_);
}

void Params::processParam(const std::string &name, const std::string &val)
{
   auto it = map_.find(name);
   if (it == map_.end())
      throwParsing(); // name not found

   ParamDbl *pd = dynamic_cast<ParamDbl *>(it->second);
   if (pd != nullptr)
   {
      Interval x(val);
      if (x.isEmpty())
         throwParsing();

      pd->setValue(x.right());
      return;
   }

   ParamInt *pi = dynamic_cast<ParamInt *>(it->second);
   if (pi != nullptr)
   {
      if (!isInt(val))
         throwParsing();

      try
      {
         pi->setValue(std::stoi(val));
         return;
      }
      catch (std::exception &e)
      {
         throwParsing();
      }
   }

   ParamStr *ps = dynamic_cast<ParamStr *>(it->second);
   if (ps != nullptr)
   {
      ps->setValue(val);
      return;
   }

   throwParsing();
}

void Params::readParam(const std::string &line, size_t first)
{
   size_t i = first;

   while (i < line.size() && isCharName(line[i]))
      ++i;

   if (i == line.size())
      throwParsing();

   std::string name = line.substr(first, i - first);

   size_t j = eatSpaces(line, i);

   if (j == line.size())
      throwParsing();

   if (line[j] != '=')
      throwParsing();

   // eats =
   ++j;

   size_t k = eatSpaces(line, j);

   if (k == line.size())
      throwParsing();

   size_t l = k;

   while (l < line.size() && (!isSpace(line[l])))
      ++l;

   std::string val = line.substr(k, l - k);

   size_t p = eatSpaces(line, l);
   if (p != line.size())
      throwParsing();

   processParam(name, val);
}

void Params::processLine(const std::string &line)
{
   size_t i = eatSpaces(line, 0);

   // line with spaces
   if (i == line.size())
      return;

   // comment line
   if (line[i] == '#')
      return;

   // line with a parameter
   if (isLetter(line[i]))
      readParam(line, i);

   else
      throwParsing();
}

void Params::loadParam(const std::string &filename)
{
   path_ = filename;
   std::ifstream f(filename);

   if (f)
   {
      std::string line;
      lineno_ = 0;

      while (getline(f, line))
      {
         ++lineno_;
         processLine(line);
      }
   }
   else
      THROW("file not found: " << filename);
}

std::ostream &operator<<(std::ostream &os, const Params &P)
{
   P.printValues(os);
   return os;
}

void Params::makeGeneral()
{
   ParamCat cat = ParamCat::General;
   std::string str;

   ////////////////////
   ParamStr dis_reg;
   dis_reg.setName("DISPLAY_REGION")
       .setCat(cat)
       .setWhat("Display of regions")
       .addChoice("STD", "standard display with one variable and its domain per line")
       .addChoice("VEC", "display as a vector of domains")
       .setValue("STD");
   add(dis_reg);

   ////////////////////
   ParamDbl time_lim;
   time_lim.setName("TIME_LIMIT")
       .setCat(cat)
       .setWhat("Time limit in seconds")
       .setValue(3600.0)
       .setMinValue(0.0);
   add(time_lim);

   ////////////////////
   ParamDbl var_rel_tol;
   str = std::string("Relative tolerance on the width of an interval domain ") +
         "of a constrained variable";
   var_rel_tol.setName("VAR_REL_TOL")
       .setCat(cat)
       .setWhat(str)
       .setValue(0.0)
       .setMinValue(0.0)
       .setMaxValue(1.0);
   add(var_rel_tol);

   ////////////////////
   ParamDbl var_abs_tol;
   str = std::string("Absolute tolerance on the width of an interval domain ") +
         "of a constrained variable";
   var_abs_tol.setName("VAR_ABS_TOL")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-8)
       .setMinValue(0.0);
   add(var_abs_tol);

   ////////////////////
   ParamInt float_prec;
   float_prec.setName("FLOAT_PRECISION")
       .setCat(cat)
       .setWhat("Number of digits used to print floating-point numbers and intervals")
       .setValue(12)
       .setMinValue(1)
       .setMaxValue(16);
   add(float_prec);

   ////////////////////
   ParamStr trace;
   trace.setName("TRACE")
       .setCat(cat)
       .setWhat("Log level")
       .addChoice("YES", "display of a trace during a solving process")
       .addChoice("NO", "no trace")
       .setValue("YES");
   add(trace);

   ////////////////////
   ParamInt trace_f;
   trace_f.setName("TRACE_FREQUENCY")
       .setCat(cat)
       .setWhat("Trace displayed every N nodes where N is the value of this parameter")
       .setValue(1000)
       .setMinValue(1.0);
   add(trace_f);

   ////////////////////
   ParamStr log_level;
   log_level.setName("LOG_LEVEL")
       .setCat(cat)
       .setWhat("Log level")
       .addChoice("NONE", "no log")
       .addChoice("MAIN", "main level e.g.display results of first-class algorithms")
       .addChoice("INTER", "intermediary level e.g.display nodes in a search algorithm")
       .addChoice("LOW", "low level e.g.display quantities in iterative methods")
       .addChoice("FULL", "verbose mode")
       .setValue("NONE");
   add(log_level);

   ////////////////////
   ParamStr prepro;
   prepro.setName("PREPROCESSING")
       .setCat(cat)
       .setWhat("Preprocessing phase of the solving process")
       .addChoice("YES", "simplification of problems before solving")
       .addChoice("NO", "no simplification")
       .setValue("YES");
   add(prepro);

   ////////////////////
   ParamStr postpro;
   postpro.setName("CERTIFICATION")
       .setCat(cat)
       .setWhat("Postprocessing of solution nodes")
       .addChoice("YES", "handles clusters and provides certificate of proofs")
       .addChoice("NO", "no certification")
       .setValue("YES");
   add(postpro);
}

void Params::makeBranch()
{
   ParamCat cat = ParamCat::Branch;
   std::string str;

   ////////////////////
   ParamStr search_stat;
   str = std::string("The search is complete when the whole search space is ") +
         "explorated until reaching the tolerances on the variables " +
         "(and the tolerance on the cost function for optimization " +
         "problems. On the contrary, the search can be stopped when " +
         "the following limits are reached: TIME_LIMIT, SOLUTION_LIMIT " +
         "NODE_LIMIT, and DEPTH_LIMIT.";
   search_stat.setName("SEARCH_STATUS")
       .setCat(cat)
       .setWhat(str)
       .addChoice("COMPLETE", "the whole search space is explorated")
       .addChoice("LIMITED", "the search is possibly stopped due to the limits")
       .setValue("LIMITED");
   add(search_stat);

   ////////////////////
   ParamInt node_lim;
   node_lim.setName("NODE_LIMIT")
       .setCat(cat)
       .setWhat("Limit on the number of nodes generated by search algorithms")
       .setValue(1000000)
       .setMinValue(1);
   add(node_lim);

   ////////////////////
   ParamInt depth_lim;
   depth_lim.setName("DEPTH_LIMIT")
       .setCat(cat)
       .setWhat("Limit on the depth of nodes of search trees")
       .setValue(1000)
       .setMinValue(1);
   add(depth_lim);

   ////////////////////
   ParamInt sol_lim;
   sol_lim.setName("SOLUTION_LIMIT")
       .setCat(cat)
       .setWhat("Limit on the number of solutions")
       .setValue(1000)
       .setMinValue(1);
   add(sol_lim);

   ////////////////////
   ParamDbl sol_clu_gap;
   str = std::string("Limit on the gap between two solutions such that they are ") +
         "aggregated if they are close enough.\nThe value is negative if " +
         "no clustering is done; equal to 0.0 if two solutions are " +
         "aggregated when they overlap; positive otherwise.\nA huge " +
         "value may be used to calculate the hull of all the solutions.";
   sol_clu_gap.setName("SOLUTION_CLUSTER_GAP")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-10)
       .setMinValue(-1.0);
   add(sol_clu_gap);

   ////////////////////
   ParamInt bb_freq;
   str = std::string("In a branch-and-bound algorithm, the next node can be either ") +
         "the node with the lowest lower bound or the node with the lowest upper " +
         "bound (diving). The frequency manages the selection of the next node:\n" +
         "- 1 means that the node with the lowest lower bound is always selected;\n" +
         "- a value > 1 means that the node with the lowest upper bound is selected " +
         "when the node counter modulo the frequency is equal to 0.";
   bb_freq.setName("BB_SPACE_FREQUENCY")
       .setCat(cat)
       .setWhat(str)
       .setValue(1)
       .setMinValue(1);
   add(bb_freq);

   ////////////////////
   ParamStr node_sel;
   std::string idfs = std::string("hybrid Best-First Depth-First strategy such ") +
                      "that the next node after a DFS stage is a node whose depth " +
                      "in the search tree is minimal";
   std::string pdfs = std::string("hybrid Best-First Depth-First strategy such ") +
                      "that the next node after a DFS stage is a node whose perimeter " +
                      "is maximal";
   std::string gpdfs = std::string("hybrid Best-First Depth-First strategy such ") +
                       "that the next node after a DFS stage is a node whose grid " +
                       "perimeter is maximal";
   std::string text = std::string("Assign SPLIT_INNER_BOX = true for DMDFS and the ") +
                      " hybrid DFS in order to calculate solutions at the given " +
                      "tolerance, and not to consider larger inner boxes as " +
                      " solutions.";
   node_sel.setName("BP_NODE_SELECTION")
       .setCat(cat)
       .setWhat("Exploration strategy for branch-and-prune algorithms")
       .addChoice("DFS", "Depth-First-Search")
       .addChoice("BFS", "Breadth-First-Search")
       .addChoice("DMDFS", "Distant-Most Depth-First-Search")
       .addChoice("IDFS", idfs)
       .addChoice("PDFS", pdfs)
       .addChoice("GPDFS", gpdfs)
       .setValue("DFS")
       .setText(text);
   add(node_sel);
}

void Params::makeSplit()
{
   ParamCat cat = ParamCat::Split;
   std::string str;

   ////////////////////
   ParamStr split_obj;
   str = std::string("The objective function is represented by a variable ") +
         "and its domain can be split or not";
   split_obj.setName("SPLIT_OBJECTIVE")
       .setCat(cat)
       .setWhat(str)
       .addChoice("YES", "it is considered as any other variable")
       .addChoice("NO", "its domain is not split")
       .setValue("NO");
   add(split_obj);

   ////////////////////
   ParamStr var_sel;
   str = std::string("Variable selection strategy that chooses the next variable ") +
         "to split";
   var_sel.setName("SPLIT_SELECTION")
       .setCat(cat)
       .setWhat(str)
       .addChoice("RR", "round-robin strategy")
       .addChoice("LF", "largest domain")
       .addChoice("SF", "smallest domain")
       .addChoice("SSR", "smear sum relative values (using derivatives)")
       .addChoice("SLF", "integer variable with smallest first and then real variable")
       .addChoice("ASR", "affine sum relative values")
       .setValue("SSR");
   add(var_sel);

   ////////////////////
   ParamStr split_sl;
   split_sl.setName("SPLIT_SLICING")
       .setCat(cat)
       .setWhat("Domain splitting strategy")
       .addChoice("BI", "divides a domain in two parts")
       .setValue("BI");
   add(split_sl);

   ////////////////////
   ParamDbl split_point;
   str = std::string("An interval [a, b] is split in two parts at a+sip*(b-a) ") +
         "where 0 < sip < 1 denotes this parameter. For example, 0.5 corresponds to " +
         "the midpoint.";
   split_point.setName("SPLIT_INTERVAL_POINT")
       .setCat(cat)
       .setWhat(str)
       .setValue(0.45)
       .setMinValue(0.0)
       .setMaxValue(1.0);
   add(split_point);

   ////////////////////
   ParamStr split_inner;
   split_inner.setName("SPLIT_INNER_BOX")
       .setCat(cat)
       .setWhat("An inner box can be considered as a solution or it can be split")
       .addChoice("YES", "it is split")
       .addChoice("NO", "it is considered as a solution")
       .setValue("NO");
   add(split_inner);
}

void Params::makeContract()
{
   ParamCat cat = ParamCat::Contract;
   std::string str;

   ////////////////////
   ParamDbl propag_tol;
   str = std::string("Tolerance that corresponds to a percentage of reduction ") +
         "of the width of a box driving propagation steps. Given two consecutive " +
         "domains prev and next of some variable and tol the tolerance, a propagation " +
         "on this variable occurs if we have (1 - width(next) / width(prev)) > tol.";
   propag_tol.setName("PROPAGATION_TOL")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-3)
       .setMinValue(0.0)
       .setMaxValue(1.0);
   add(propag_tol);

   ////////////////////
   ParamDbl bc3_peel;
   str = std::string("Peel factor for the BC3 contractor\nGiven an interval ") +
         "[a, b] and the peel factor f, let w = (b - a) * (p / 100) be a ratio " +
         "of the interval width. Then the consistency of the intervals [a, a+w] " +
         "and [b-w, w] is checked in order to stop the search.";
   bc3_peel.setName("BC3_PEEL_FACTOR")
       .setCat(cat)
       .setWhat(str)
       .setValue(2.0)
       .setMinValue(0.0)
       .setMaxValue(100.0);
   add(bc3_peel);

   ////////////////////
   ParamInt bc3_iter;
   str = std::string("Iteration limit for the iterative method of the BC3 contractor ") +
         "used to find the outermost zeros of a function in a given interval";
   bc3_iter.setName("BC3_ITER_LIMIT")
       .setCat(cat)
       .setWhat(str)
       .setValue(30)
       .setMinValue(1);
   add(bc3_iter);

   ////////////////////
   ParamStr propag_base;
   str = std::string("Propagation strategy: the default one is a classical ") +
         "propagation algorithm that uses one contractor per constraint";
   propag_base.setName("PROPAGATION_BASE")
       .setCat(cat)
       .setWhat(str)
       .addChoice("HC4", "HC4 contractor")
       .addChoice("BC4", "BC4 contractor")
       .addChoice("AFFINE", "contractor based on affine forms")
       .setValue("HC4");
   add(propag_base);

   ////////////////////
   ParamStr propag_acid;
   propag_acid.setName("PROPAGATION_WITH_ACID")
       .setCat(cat)
       .setWhat("Propagation with a ACID contractor")
       .addChoice("YES", "with ACID")
       .addChoice("NO", "no ACID")
       .setValue("YES");
   add(propag_acid);

   ////////////////////
   ParamStr propag_poly;
   propag_poly.setName("PROPAGATION_WITH_POLYTOPE_HULL")
       .setCat(cat)
       .setWhat("Propagation with a polytope hull contractor")
       .addChoice("YES", "used")
       .addChoice("NO", "not used")
       .setValue("YES");
   add(propag_poly);

   ////////////////////
   ParamStr propag_newton;
   propag_newton.setName("PROPAGATION_WITH_NEWTON")
       .setCat(cat)
       .setWhat("Propagation with the interval Newton method applied to a square system")
       .addChoice("YES", "used")
       .addChoice("NO", "not used")
       .setValue("YES");
   add(propag_newton);

   ////////////////////
   ParamDbl relax_tol;
   str = std::string("Positive real number eps used to relax an equation ") +
         "c(x)=0 as |l(x)| <= eps where l(x) is generated by a relaxation method.\n" +
         "The value 0.0 implies that a linear equation is generated.";
   relax_tol.setName("RELAXATION_EQ_TOL")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-10)
       .setMinValue(0.0);
   add(relax_tol);

   ////////////////////
   ParamDbl loop_tol;
   str = std::string("Tolerance that corresponds to a percentage of reduction ") +
         "of the width of a box driving the loop contractor.";
   loop_tol.setName("LOOP_CONTRACTOR_TOL")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-2)
       .setMinValue(0.0)
       .setMaxValue(1.0);
   add(loop_tol);
}

void Params::makePolytope()
{
   ParamCat cat = ParamCat::Polytope;
   std::string str;

   ////////////////////
   ParamStr relax;
   relax.setName("POLYTOPE_HULL_RELAXATION")
       .setCat(cat)
       .setWhat("Relaxation method for the polytope hull contractor")
       .addChoice("AFFINE", "use of affine forms")
       .addChoice("TAYLOR", "use of Taylor (mean value) forms")
       .addChoice("AFFINE_TAYLOR", "use of affine and Taylor (mean value) forms")
       .setValue("AFFINE");
   add(relax);

   ////////////////////
   ParamStr hansen;
   hansen.setName("POLYTOPE_HULL_TAYLOR_HANSEN")
       .setCat(cat)
       .setWhat("Use of Hansen's derivatives in the Taylor forms")
       .addChoice("YES", "Hansen's derivatives")
       .addChoice("NO", "classical derivatives")
       .setValue("YES");
   add(hansen);

   ////////////////////
   ParamStr tayl_corn;
   tayl_corn.setName("POLYTOPE_HULL_TAYLOR_CORNER")
       .setCat(cat)
       .setWhat("Choice of corners in the Taylor-based relaxation")
       .addChoice("RANDOM", "one corner selected randomly")
       .addChoice("RANDOM_OPPOSITE", "two opposite corners, one selected randomly")
       .addChoice("LEFT", "corner of left bounds")
       .addChoice("RIGHT", "corner of right bounds")
       .setValue("RANDOM_OPPOSITE");
   add(tayl_corn);

   ////////////////////
   ParamInt tayl_seed;
   str = std::string("Seed used to initialize the generator of pseudo-random numbers. ") +
         "The system is used if it is equal to 0.";
   tayl_seed.setName("POLYTOPE_HULL_TAYLOR_SEED")
       .setCat(cat)
       .setWhat(str)
       .setValue(0)
       .setMinValue(0);
   add(tayl_seed);

   ////////////////////
   ParamStr loop;
   str = "it is applied in a fixed-point loop while the box can be reduced enough";
   loop.setName("POLYTOPE_HULL_LOOP")
       .setCat(cat)
       .setWhat("Control of the application of the polytope hull contractor")
       .addChoice("YES", str)
       .addChoice("NO", "it is applied only once")
       .setValue("NO");
   add(loop);

   ////////////////////
   ParamDbl loop_tol;
   str = std::string("Tolerance that corresponds to a percentage of reduction ") +
         "of the width of a box driving the fixed-point loop";
   loop_tol.setName("POLYTOPE_HULL_LOOP_TOL")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-1)
       .setMinValue(0.0)
       .setMaxValue(1.0);
   add(loop_tol);

   ////////////////////
   ParamStr approx;
   str = "Approximation method that generates affine forms for the elementary functions";
   approx.setName("POLYTOPE_HULL_AFFINE_APPROX")
       .setCat(cat)
       .setWhat(str)
       .addChoice("MINRANGE", "MinRange approximation")
       .addChoice("CHEBYSHEV", "Chebyshev approximation")
       .setValue("CHEBYSHEV");
   add(approx);
}

void Params::makeAcid()
{
   ParamCat cat = ParamCat::Acid;
   std::string str;

   ////////////////////
   ParamInt nb_sl_3b;
   nb_sl_3b.setName("NB_SLICE_3B")
       .setCat(cat)
       .setWhat("Number of slices of a domain handled by var3B contractors")
       .setValue(7)
       .setMinValue(1);
   add(nb_sl_3b);

   ////////////////////
   ParamInt nb_sl_cid;
   nb_sl_cid.setName("NB_SLICE_CID")
       .setCat(cat)
       .setWhat("Number of slices of a domain handled by varCID contractors")
       .setValue(3)
       .setMinValue(1);
   add(nb_sl_cid);

   ////////////////////
   ParamInt learn_lg;
   learn_lg.setName("ACID_LEARN_LENGTH")
       .setCat(cat)
       .setWhat("Number of consecutive nodes handled in the learning phase of ACID")
       .setValue(50)
       .setMinValue(1);
   add(learn_lg);

   ////////////////////
   ParamInt cycle_lg;
   cycle_lg.setName("ACID_CYCLE_LENGTH")
       .setCat(cat)
       .setWhat("Number of nodes in a learning phase and an exploitation phase of ACID")
       .setValue(1000)
       .setMinValue(1);
   add(cycle_lg);

   ////////////////////
   ParamDbl ct_ratio;
   str = std::string("Gain ratio between two consecutive boxes handed by var3BCID ") +
         "contractors in ACID. It is used to adapt the number of var3BCID contractors " +
         "that are applied in the contraction procedure.";
   ct_ratio.setName("ACID_CT_RATIO")
       .setCat(cat)
       .setWhat(str)
       .setValue(0.002)
       .setMinValue(0.0)
       .setMaxValue(1.0);
   add(ct_ratio);

   ////////////////////
   ParamDbl min_w;
   str = std::string("Threshold on the width of a variable domain used by ") +
         "var3BCID contractors";
   min_w.setName("VAR3BCID_MIN_WIDTH")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-11)
       .setMinValue(0.0);
   add(min_w);

   ////////////////////
   ParamDbl hc4_tol;
   str = std::string("Tolerance in that corresponds to a percentage of reduction ") +
         "of the width of a box driving propagation steps of the HC4 algorithm used " +
         "in ACID. Given two consecutive domains prev and next of some variable " +
         "and tol the tolerance, a propagation on this variable occurs if we have " +
         "(1-width(next)/width(prev))>tol.";
   min_w.setName("ACID_HC4_TOL")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-1)
       .setMinValue(0.0)
       .setMaxValue(1.0);
   add(min_w);
}

void Params::makeLocalOptim()
{
   ParamCat cat = ParamCat::LocalOptim;
   std::string str;

   ////////////////////
   ParamStr solver;
   str = "";
   solver.setName("NLP_SOLVER_ALGORITHM")
       .setCat(cat)
       .setWhat(str)
       .addChoice("DEFAULT", "default algorithm")
       .addChoice("NLOPT_NELDERMEAD",
                  "derivative-free unconstrained local optimization using NlOpt")
       .addChoice("NLOPT_BOBYQA",
                  "derivative-free unconstrained local optimization using NlOpt")
       .addChoice("NLOPT_MMA",
                  "gradient-based constrained local optimization using NlOpt")
       .addChoice("NLOPT_SLSQP",
                  "gradient-based constrained local optimization using NlOpt")
       .setValue("DEFAULT");
   add(solver);

   ////////////////////
   ParamDbl time_lim;
   time_lim.setName("NLP_SOLVER_TIME_LIMIT")
       .setCat(cat)
       .setWhat("Time limit in seconds")
       .setValue(10.0)
       .setMinValue(0.0);
   add(time_lim);

   ////////////////////
   ParamInt iter_lim;
   iter_lim.setName("NLP_SOLVER_ITER_LIMIT")
       .setCat(cat)
       .setWhat("Iteration limit")
       .setValue(100)
       .setMinValue(1);
   add(iter_lim);

   ////////////////////
   ParamDbl obj_rel_tol;
   obj_rel_tol.setName("NLP_SOLVER_OBJ_REL_TOL")
       .setCat(cat)
       .setWhat("Relative tolerance on the objective function")
       .setValue(1.0e-4)
       .setMinValue(0.0)
       .setMaxValue(1.0);
   add(obj_rel_tol);

   ////////////////////
   ParamDbl obj_abs_tol;
   obj_abs_tol.setName("NLP_SOLVER_OBJ_ABS_TOL")
       .setCat(cat)
       .setWhat("Absolute tolerance on the objective function")
       .setValue(1.0e-8)
       .setMinValue(0.0);
   add(obj_abs_tol);
}

void Params::makeLinearSolver()
{
   ParamCat cat = ParamCat::LinearSolver;
   std::string str;

   ////////////////////
   ParamDbl time_lim;
   time_lim.setName("LP_TIME_LIMIT")
       .setCat(cat)
       .setWhat("Time limit in seconds of the LP solver")
       .setValue(100.0)
       .setMinValue(0.0);
   add(time_lim);

   ////////////////////
   ParamInt iter_lim;
   iter_lim.setName("LP_ITER_LIMIT")
       .setCat(cat)
       .setWhat("Iteration limit of the LP solver")
       .setValue(100)
       .setMinValue(1);
   add(iter_lim);

   ////////////////////
   ParamDbl feas_tol;
   feas_tol.setName("LP_FEAS_TOL")
       .setCat(cat)
       .setWhat("Absolute tolerance on the feasibility of constraints in the LP solver")
       .setValue(1.0e-8)
       .setMinValue(0.0);
   add(feas_tol);

   ////////////////////
   ParamDbl min_pivot;
   str = std::string("Threshold on the values of pivots in Gaussian elimination. ") +
         "A pivot must be strictly greater than this value.";
   min_pivot.setName("GAUSSIAN_MIN_PIVOT")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-8)
       .setMinValue(0.0);
   add(min_pivot);
}

void Params::makeInterval()
{
   ParamCat cat = ParamCat::Interval;
   std::string str;

   ////////////////////
   ParamInt uni_iter_lim;
   uni_iter_lim.setName("UNI_NEWTON_ITER_LIMIT")
       .setCat(cat)
       .setWhat("Iteration limit of the univariate interval Newton method")
       .setValue(20)
       .setMinValue(1);
   add(uni_iter_lim);

   ////////////////////
   ParamInt gs_iter_lim;
   str = std::string("Iteration limit on the number of steps in the interval ") +
         "Gauss-Seidel method";
   gs_iter_lim.setName("GAUSS_SEIDEL_ITER_LIMIT")
       .setCat(cat)
       .setWhat(str)
       .setValue(50)
       .setMinValue(1);
   add(gs_iter_lim);

   ////////////////////
   ParamDbl gs_tol;
   str = std::string("Tolerance that corresponds to a percentage of reduction ") +
         " of the width of an interval vector. Given two consecutive domains " +
         "prev and next of some variable and tol the tolerance, the method is " +
         "iterated if we have (1-width(next)/width(prev))>tol.";
   gs_tol.setName("GAUSS_SEIDEL_TOL")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-3)
       .setMinValue(0.0)
       .setMaxValue(1.0);
   add(gs_tol);

   ////////////////////
   ParamInt nwt_iter_lim;
   str = std::string("Iteration limit on the number of (outer) steps in the ") +
         "contraction technique of the interval Newton method";
   nwt_iter_lim.setName("NEWTON_ITER_LIMIT")
       .setCat(cat)
       .setWhat(str)
       .setValue(30)
       .setMinValue(1);
   add(nwt_iter_lim);

   ////////////////////
   ParamInt cert_iter_lim;
   str = std::string("Iteration limit on the number of (outer) steps in the ") +
         "certification technique of the interval Newton method";
   cert_iter_lim.setName("NEWTON_CERTIFY_ITER_LIMIT")
       .setCat(cat)
       .setWhat(str)
       .setValue(20)
       .setMinValue(1);
   add(cert_iter_lim);

   ////////////////////
   ParamDbl nwt_tol;
   str = std::string("Tolerance that corresponds to a percentage of reduction ") +
         "of the width of a box in the interval Newton method. Given two " +
         "consecutive domains prev and next of some variable and tol the tolerance, " +
         "the method is  iterated if we have (1-width(next)/width(prev))>tol.";
   nwt_tol.setName("NEWTON_TOL")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-3)
       .setMinValue(0.0)
       .setMaxValue(1.0);
   add(nwt_tol);

   ////////////////////
   ParamDbl nwt_wlim;
   str = std::string("Threshold on the width of an input box of the contraction ") +
         "method of the interval Newton method, which is not applied if the width " +
         "of the box is strictly greater than this value.";
   nwt_wlim.setName("NEWTON_WIDTH_LIMIT")
       .setCat(cat)
       .setWhat(str)
       .setValue(0.5)
       .setMinValue(0.0);
   add(nwt_wlim);

   ////////////////////
   ParamDbl inf_delta;
   str = std::string("Let x be an interval and let m(x) be its midpoint. An ") +
         "inflation of x returns m(x)+delta*(x-m(x))+chi*[-1,1]. The value of "
         "delta must be strictly greater than 1.0";
   inf_delta.setName("INFLATION_DELTA")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.125)
       .setMinValue(1.0);
   add(inf_delta);

   ////////////////////
   ParamDbl inf_chi;
   str = std::string("Let x be an interval and let m(x) be its midpoint. An ") +
         "inflation of x returns m(x)+delta*(x-m(x))+chi*[-1,1]. The value of " +
         "chi must be strictly greater than 0.0, which is necessary to inflate " +
         "degenerated intervals.";
   inf_chi.setName("INFLATION_CHI")
       .setCat(cat)
       .setWhat(str)
       .setValue(1.0e-12)
       .setMinValue(0.0);
   add(inf_chi);
}

} // namespace realpaver
