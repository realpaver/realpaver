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
#include "realpaver/Double.hpp"
#include "realpaver/Exception.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

Param Param::instance_;

Param::Param() : path_(""), lineno_(0), tolmap_(), intmap_(), dblmap_()
{
   tolmap_.insert(std::make_pair("XTOL", Tolerance::makeRel(1.0e-8)));
   tolmap_.insert(std::make_pair("DTOL", Tolerance::makeRel(1.0e-8)));

   intmap_.insert(std::make_pair("NODE_LIMIT", 100000));
}

Param::~Param()
{}

int Param::getIntParam(const string& name)
{
   auto it = instance_.intmap_.find(name);

   if (it == instance_.intmap_.end())
      THROW("'" << name << "' is not an integral parameter");

   return it->second;
}

void Param::setIntParam(const string& name, int val)
{
   auto it = instance_.intmap_.find(name);

   if (it == instance_.intmap_.end())
      THROW("'" << name << "' is not an integral parameter");

   instance_.intmap_[name] = val;
}

double Param::getDblParam(const string& name)
{
   auto it = instance_.dblmap_.find(name);

   if (it == instance_.dblmap_.end())
      THROW("'" << name << "' is not a real parameter");

   return it->second;   
}

void Param::setDblParam(const string& name, double val)
{
   auto it = instance_.dblmap_.find(name);

   if (it == instance_.dblmap_.end())
      THROW("'" << name << "' is not a real parameter");

   instance_.dblmap_[name] = val;
}

Tolerance Param::getTolParam(const string& name)
{
   auto it = instance_.tolmap_.find(name);

   if (it == instance_.tolmap_.end())
      THROW("'" << name << "' is not a tolerance parameter");

   return it->second;
}

void Param::setTolParam(const string& name, const Tolerance& val)
{
   auto it = instance_.tolmap_.find(name);

   if (it == instance_.tolmap_.end())
      THROW("'" << name << "' is not a tolerance parameter");

   instance_.tolmap_[name] = val;
}

void Param::throwEx()
{
   THROW( "settings error in file '" << path_ << "' at line " << lineno_);
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
   auto itd = instance_.dblmap_.find(name);
   if (itd != dblmap_.end())
   {
      Interval x(val.c_str());
      if (x.isEmpty()) throwEx();

      dblmap_[name] = x.right();
      return;
   }

   // Int
   auto iti = instance_.intmap_.find(name);
   if (iti != intmap_.end())
   {
      if (!isInt(val)) throwEx();

      try {
         intmap_[name] = std::stoi(val);
         return;
      }
      catch(std::exception e)
      {
         throwEx();
      }
   }

   // Tolerance: name has a prefix ABS_ or REL_
   if (name.size() >= 5)
   {
      std::string prefix = name.substr(0, 4);
      std::string s = name.substr(4, name.size() - 4);

      auto it = instance_.tolmap_.find(s);
      Interval x(val.c_str());

      if ((it != instance_.tolmap_.end()) && (!x.isEmpty()) && x.isPositive())
      {
         if (prefix == "ABS_")
         {
            Tolerance tol = Tolerance::makeAbs(x.right());
            instance_.tolmap_[s] = tol;
            return;
         }

         if (prefix == "REL_")
         {
            Tolerance tol = Tolerance::makeRel(x.right());
            instance_.tolmap_[s] = tol;
            return;
         }
      }
   }

   // no parameter found
   instance_.throwEx();
}

void Param::readParam(const std::string& line, size_t first)
{
   size_t i = first;

   while (i < line.size() && isCharName(line[i]))
      ++i;

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

   while (l < line.size() && (!isSpace(line[l])))
      ++l;

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

void Param::init(const std::string& filename)
{
   instance_.path_ = filename;
   std::ifstream f(filename);

   if (f)
   {
      std::string line;
      instance_.lineno_ = 0;
      
      while (getline(f, line))
      {
         ++ instance_.lineno_;
         instance_.processLine(line);
      }
   }
   else
      THROW("file open error '" << filename << "'");
}

void Param::print(std::ostream& os)
{
   os << "# List of registered parameters\n\n";

   os << "# Integral parameters\n";
   for (auto it = instance_.intmap_.begin();
             it != instance_.intmap_.end();
             ++it)
      os << it-> first << " = " << it->second << "\n\n";

   if (instance_.intmap_.empty()) os << "\n";

   os << "# Double parameters\n";
   for (auto it = instance_.dblmap_.begin();
             it != instance_.dblmap_.end();
             ++it)
      os << it-> first << " = " << it->second << "\n\n";

   if (instance_.dblmap_.empty()) os << "\n";

   os << "# Tolerances\n";
   for (auto it = instance_.tolmap_.begin();
             it != instance_.tolmap_.end();
             ++it)
   {
      Tolerance tol = it->second;
      if (tol.isAbsolute())
         os << "ABS_";
      else
         os << "REL_";

      os << it-> first << " = " << tol.getVal() << "\n\n";
   }

   if (instance_.tolmap_.empty()) os << "\n";
}

} // namespace
