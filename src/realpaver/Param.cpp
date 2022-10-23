///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <limits>
#include <map>
#include <sstream>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Double.hpp"
#include "realpaver/Exception.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

Param Param::instance_;

Param::Param()
      : path_(""),
        lineno_(0),
        tolmap_(),
        intmap_(),
        dblmap_(),
        strmap_()
{
   Tolerance tol3 = Tolerance::makeRel(1.0e-3),
             tol6 = Tolerance::makeRel(1.0e-6),
             tol8 = Tolerance::makeRel(1.0e-8);

   // tolerance
   tolmap_.insert(std::make_pair("XTOL",                      tol8));
   tolmap_.insert(std::make_pair("DTOL",                      tol6));
   tolmap_.insert(std::make_pair("OBJ_TOL",                   tol6));
   tolmap_.insert(std::make_pair("PROPAGATION_DTOL",          tol3));

   // integer
   intmap_.insert(std::make_pair("NODE_LIMIT",                100000));
   intmap_.insert(std::make_pair("DEPTH_LIMIT",               100));
   intmap_.insert(std::make_pair("SOLUTION_LIMIT",            100));
   intmap_.insert(std::make_pair("LINE_SEARCH_ITER_LIMIT",    20));
   intmap_.insert(std::make_pair("PROPAGATION_ITER_LIMIT",    50));
   intmap_.insert(std::make_pair("BC3_ITER_LIMIT",            30));
   intmap_.insert(std::make_pair("NEWTON_ITER_LIMIT",         20));
   intmap_.insert(std::make_pair("SPLIT_NB_SLICES",           5));
   intmap_.insert(std::make_pair("LP_ITER_LIMIT",             50));
   intmap_.insert(std::make_pair("FLOAT_PRECISION",           8));
   intmap_.insert(std::make_pair("BB_SPACE_FREQUENCY",        1));

   // double
   dblmap_.insert(std::make_pair("TIME_LIMIT",                100.0));
   dblmap_.insert(std::make_pair("LINE_SEARCH_ARMIJO",        0.09375));
   dblmap_.insert(std::make_pair("LINE_SEARCH_STEP_TOL",      1.0e-8));
   dblmap_.insert(std::make_pair("BC3_PEEL_FACTOR",           2.0));
   dblmap_.insert(std::make_pair("SPLIT_PEEL_FACTOR",         10.0));
   dblmap_.insert(std::make_pair("LP_TIME_LIMIT",             10.0));
   dblmap_.insert(std::make_pair("LOCAL_SOLVER_TIME_LIMIT",   10.0));
   dblmap_.insert(std::make_pair("GRADIENT_DESCENT_TOL",      1.0e-3));

   // string
   strmap_.insert(std::make_pair("BP_NODE_SELECTION",         "DFS"));
   strmap_.insert(std::make_pair("SPLIT_OBJECTIVE",           "NO"));
   strmap_.insert(std::make_pair("SPLIT_SELECTOR",            "MAX_DOM"));
   strmap_.insert(std::make_pair("SPLIT_SLICER",              "BISECTION"));
   strmap_.insert(std::make_pair("PROPAGATION_BASE",          "HC4"));
   strmap_.insert(std::make_pair("PROPAGATION_WITH_MAX_CID",  "YES"));
   strmap_.insert(std::make_pair("PROPAGATION_WITH_POLYTOPE", "NO"));   
   strmap_.insert(std::make_pair("LOCAL_SOLVER_ALGORITHM",    "GRADIENT"));
   strmap_.insert(std::make_pair("LOG_LEVEL",                 "NONE"));
   strmap_.insert(std::make_pair("TRACE",                     "NO"));
   strmap_.insert(std::make_pair("SPLIT_INNER",               "NO"));   
}

int Param::getIntParam(const string& name) const
{
   auto it = intmap_.find(name);

   if (it == intmap_.end())
      THROW("'" << name << "' is not an integral parameter");

   return it->second;
}

int Param::GetIntParam(const string& name)
{
   return instance_.getIntParam(name);
}

void Param::setIntParam(const string& name, int val)
{
   auto it = intmap_.find(name);

   if (it == intmap_.end())
      THROW("'" << name << "' is not an integral parameter");

   intmap_[name] = val;
}

void Param::SetIntParam(const string& name, int val)
{
   instance_.setIntParam(name, val);
}

double Param::getDblParam(const string& name) const
{
   auto it = dblmap_.find(name);

   if (it == dblmap_.end())
      THROW("Symbol '" << name << "' is not a real parameter");

   return it->second;   
}

double Param::GetDblParam(const string& name)
{
   return instance_.getDblParam(name);
}

void Param::setDblParam(const string& name, double val)
{
   auto it = dblmap_.find(name);

   if (it == dblmap_.end())
      THROW("Symbol '" << name << "' is not a real parameter");

   dblmap_[name] = val;
}

void Param::SetDblParam(const string& name, double val)
{
   instance_.setDblParam(name, val);
}

std::string Param::getStrParam(const string& name) const
{
   auto it = strmap_.find(name);

   if (it == strmap_.end())
      THROW("Symbol '" << name << "' is not a string parameter");

   return it->second;      
}

std::string Param::GetStrParam(const string& name)
{
   return instance_.getStrParam(name);
}

void Param::setStrParam(const string& name, const std::string& val)
{
   auto it = strmap_.find(name);

   if (it == strmap_.end())
      THROW("Symbol '" << name << "' is not a string parameter");

   strmap_[name] = val;   
}

void Param::SetStrParam(const string& name, const std::string& val)
{
   instance_.setStrParam(name, val);
}

Tolerance Param::getTolParam(const string& name) const
{
   auto it = tolmap_.find(name);

   if (it == tolmap_.end())
      THROW("Symbol '" << name << "' is not a tolerance parameter");

   return it->second;
}

Tolerance Param::GetTolParam(const string& name)
{
   return instance_.getTolParam(name);
}

void Param::setTolParam(const string& name, const Tolerance& val)
{
   auto it = tolmap_.find(name);

   if (it == tolmap_.end())
      THROW("Symbol '" << name << "' is not a tolerance parameter");

   tolmap_[name] = val;
}

void Param::SetTolParam(const string& name, const Tolerance& val)
{
   instance_.setTolParam(name, val);
}

void Param::throwEx()
{
   THROW("Settings error in file '" << path_ << "' at line " << lineno_);
}

bool Param::isInt(const std::string& s)
{
   if (s.size() == 0) return false;

   size_t i = 0;
   if ((s[0] == '+') || (s[0] == '-')) ++i;

   if (i == s.size()) return false;

   for (size_t j=i; j<s.size(); ++j)
      if (!isDigit(s[j]))
         return false;

   return true;
}

bool Param::isSpace(char c)
{
   return c == ' ' || c == '\t' || c == '\r';
}

bool Param::isDigit(char c)
{
   return c >= '0' && c <= '9';
}

bool Param::isLetter(char c)
{
   return c >= 'A' && c <= 'Z';
}

bool Param::isCharName(char c)
{
   return isLetter(c) || isDigit(c) || c == '_';
}

size_t Param::eatSpaces(const std::string& line, size_t first)
{
   size_t i = first;

   while (i < line.size() && isSpace(line[i]))
      ++i;

   return i;
}

void Param::processParam(const std::string& name, const std::string& val)
{
   // Double
   auto itd = dblmap_.find(name);
   if (itd != dblmap_.end())
   {
      Interval x(val.c_str());
      if (x.isEmpty()) throwEx();

      dblmap_[name] = x.right();
      return;
   }

   // Int
   auto iti = intmap_.find(name);
   if (iti != intmap_.end())
   {
      if (!isInt(val)) throwEx();

      try {
         intmap_[name] = std::stoi(val);
         return;
      }
      catch(std::exception& e)
      {
         throwEx();
      }
   }

   // String
   auto its = strmap_.find(name);
   if (its != strmap_.end())
   {      
      strmap_[name] = val;      
      return;
   }

   // Tolerance: name has a prefix ABS_ or REL_
   if (name.size() >= 5)
   {
      std::string prefix = name.substr(0, 4);
      std::string s = name.substr(4, name.size() - 4);

      auto it = tolmap_.find(s);
      Interval x(val.c_str());

      if ((it != tolmap_.end()) && (!x.isEmpty()) && x.isPositive())
      {
         if (prefix == "ABS_")
         {
            Tolerance tol = Tolerance::makeAbs(x.right());
            tolmap_[s] = tol;
            return;
         }

         if (prefix == "REL_")
         {
            Tolerance tol = Tolerance::makeRel(x.right());
            tolmap_[s] = tol;
            return;
         }
      }
   }

   // no parameter found
   throwEx();
}

void Param::readParam(const std::string& line, size_t first)
{
   size_t i = first;

   while (i < line.size() && isCharName(line[i])) ++i;

   if (i == line.size()) throwEx();

   std::string name = line.substr(first, i - first);

   size_t j = eatSpaces(line, i);

   if (j == line.size()) throwEx();

   if (line[j] != '=') throwEx();

   // eats = 
   ++j;

   size_t k = eatSpaces(line, j);

   if (k == line.size()) throwEx();

   size_t l = k;

   while (l < line.size() && (!isSpace(line[l]))) ++l;

   std::string val = line.substr(k, l - k);

   size_t p = eatSpaces(line, l);
   if (p != line.size()) throwEx();

   processParam(name, val);
}

void Param::processLine(const std::string& line)
{
   size_t i = eatSpaces(line, 0);

   // line with spaces 
   if (i == line.size()) return;

   // comment line
   if (line[i] == '#') return;

   // line with a parameter
   if (isLetter(line[i]))
      readParam(line, i);

   else
      throwEx();
}

void Param::loadParam(const std::string& filename)
{
   path_ = filename;
   std::ifstream f(filename);

   if (f)
   {
      std::string line;
      lineno_ = 0;
      
      while (getline(f, line))
      {         
         ++ lineno_;
         processLine(line);
      }
   }
   else
      THROW("file open error '" << filename << "'");
}

void Param::print(std::ostream& os) const
{
   std::map<std::string, std::string> smap;
   size_t lmax = 0;

   // integers
   for (auto it = intmap_.begin(); it != intmap_.end(); ++it)
   {
      std::ostringstream ostr;
      ostr << it->second;
      smap.insert(std::make_pair(it->first, ostr.str()));
      size_t l = it->first.length();
      if (l > lmax) lmax = l;
   }
   
   // doubles
   for (auto it = dblmap_.begin(); it != dblmap_.end(); ++it)
   {
      std::ostringstream ostr;
      ostr << it->second;
      smap.insert(std::make_pair(it->first, ostr.str()));
      size_t l = it->first.length();
      if (l > lmax) lmax = l;
   }

   // strings
   for (auto it = strmap_.begin(); it != strmap_.end(); ++it)
   {
      smap.insert(std::make_pair(it->first, it->second));
      size_t l = it->first.length();
      if (l > lmax) lmax = l;
   }

   // tolerances
   for (auto it = tolmap_.begin(); it != tolmap_.end(); ++it)
   {
      Tolerance tol = it->second;
      std::ostringstream ostr;
      ostr << tol.getVal();

      std::string name = "";
      if (tol.isAbsolute()) name += "ABS_";
      else name += "REL_";
      name += it->first;

      smap.insert(std::make_pair(name, ostr.str()));
      size_t l = it->first.length();
      if (l > lmax) lmax = l;
   }

   // prints the parameters
   for (auto it = smap.begin(); it != smap.end(); ++it)
   {
      if (it != smap.begin()) os << "\n";
      os << it->first;
      size_t l = it->first.length();
      for (size_t i=0; i<lmax-l; ++i) os << " ";
      os << " = " << it->second;
   }
}

void Param::PrintParam(std::ostream& os)
{
   instance_.print(os);
}

void Param::LoadParam(const std::string& filename)
{
   instance_.loadParam(filename);
}

std::ostream& operator<<(std::ostream& os, const Param& prm)
{
   prm.print(os);
   return os;
}

} // namespace
