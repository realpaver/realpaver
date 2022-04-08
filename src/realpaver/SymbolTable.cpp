///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/SymbolTable.hpp"

namespace realpaver {

ParsingSymbol::ParsingSymbol(const std::string& name)
      : name_(name),
        hcode_(hash1(name))
{
   THROW_IF(name == "", "Definition of symbol with no name");
}

ParsingSymbol::~ParsingSymbol()
{}

std::string ParsingSymbol::getName() const
{
   return name_;
}

///////////////////////////////////////////////////////////////////////////////

ConstantSymbol::ConstantSymbol(const std::string& name, const Interval& x)
      : ParsingSymbol(name),
        x_(x)
{
   THROW_IF(x.isEmpty(), "Definition of constant symbol with empty interval");
}

Interval ConstantSymbol::getValue() const
{
   return x_;
}

///////////////////////////////////////////////////////////////////////////////

VariableSymbol::VariableSymbol(Variable v)
      : ParsingSymbol(v.getName()),
        v_(v)
{}

Variable VariableSymbol::getVar() const
{
   return v_;
}


///////////////////////////////////////////////////////////////////////////////

AliasSymbol::AliasSymbol(const std::string& name, const Term& t)
      : ParsingSymbol(name),
        t_(t)
{}

Term AliasSymbol::getTerm() const
{
   return t_;
}

///////////////////////////////////////////////////////////////////////////////

FunctionSymbol::FunctionSymbol(const std::string& name)
      : ParsingSymbol(name),
        args_(),
        t_(0),
        scope_()
{}

void FunctionSymbol::addArgument(const std::string& name)
{
   ASSERT(!hasArgument(name), "Bad argument");

   Variable v(name);
   v.setId(1000000 + args_.size());
   args_.push_back(v);
   scope_.insert(v);
}

Variable FunctionSymbol::getArgument(size_t i) const
{
   ASSERT(i < args_.size(), "Access out of range to an argument @ " << i);

   return args_[i];
}

Variable FunctionSymbol::getVar(const std::string& name) const
{
   ASSERT(hasArgument(name), "Bad argument [" << name << "]");

   for (auto v : args_)
      if (v.getName() == name) return v;

   return Variable("novar");
}

size_t FunctionSymbol::arity() const
{
   return args_.size();
}

bool FunctionSymbol::setTerm(const Term& t)
{
   Scope s;
   t.makeScope(s);

   if (scope_.size() != s.size()) return false;
   if (!scope_.contains(s)) return false;

   t_ = t;
   return true;
}

Term FunctionSymbol::getTerm() const
{
   return t_;
}

bool FunctionSymbol::hasArgument(const std::string& name) const
{
   for (auto v : args_)
      if (v.getName() == name) return true;

   return false;
}

void FunctionSymbol::print(std::ostream& os) const
{
   os << getName() << "(";
   for (size_t i=0; i<args_.size(); ++i)
   {
      if (i != 0) os << ", ";
      os << args_[i].getName();
   }
   os << ") = " << t_;
}

size_t FunctionSymbol::getIndexVar(const Variable& v) const
{
   for (size_t i=0; i<args_.size(); ++i)
      if (args_[i].id() == v.id()) return i;

   return 0;
}

///////////////////////////////////////////////////////////////////////////////

FunctionCall::FunctionCall(FunctionSymbol* f)
      : f_(f),
        lt_()
{}

void FunctionCall::addTerm(Term t)
{
   lt_.push_back(t);
}

FunctionSymbol* FunctionCall::getFunctionSymbol() const
{
   return f_;
}

size_t FunctionCall::nbTerms() const
{
   return lt_.size();
}

Term FunctionCall::getTerm(size_t i) const
{
   return lt_[i];
}

///////////////////////////////////////////////////////////////////////////////

SymbolTable::~SymbolTable()
{
   clear();
}
 
bool SymbolTable::hasSymbol(const std::string& name) const
{
   auto itc = cmap_.find(name);
   if (itc != cmap_.end()) return true;

   auto itv = vmap_.find(name);
   if (itv != vmap_.end()) return true;

   auto itk = keywords_.find(name);
   if (itk != keywords_.end()) return true;

   auto itf = fmap_.find(name);
   if (itf != fmap_.end()) return true;

   auto ita = amap_.find(name);
   if (ita != amap_.end()) return true;

   return false;
}

void SymbolTable::insertKeyword(const std::string& name)
{
   keywords_.insert(name);
}

ConstantSymbol* SymbolTable::findConstant(const std::string& name) const
{
   auto itc = cmap_.find(name);
   return (itc == cmap_.end()) ? nullptr : itc->second;
}

ConstantSymbol* SymbolTable::insertConstant(const std::string& name, const Interval& x)
{
   ConstantSymbol* symbol = new ConstantSymbol(name, x);
   cmap_.insert(std::make_pair(name, symbol));
   return symbol;
}

VariableSymbol* SymbolTable::findVariable(const std::string& name) const
{
   auto itv = vmap_.find(name);
   return (itv == vmap_.end()) ? nullptr : itv->second;   
}

VariableSymbol* SymbolTable::insertVariable(const std::string& name, Variable v)
{
   VariableSymbol* symbol = new VariableSymbol(v);
   vmap_.insert(std::make_pair(name, symbol));
   return symbol;
}

FunctionSymbol* SymbolTable::findFunction(const std::string& name) const
{
   auto itf = fmap_.find(name);
   return (itf == fmap_.end()) ? nullptr : itf->second;  
}

FunctionSymbol* SymbolTable::insertFunction(const std::string& name)
{
   FunctionSymbol* symbol = new FunctionSymbol(name);
   fmap_.insert(std::make_pair(name, symbol));
   return symbol;
}

AliasSymbol* SymbolTable::findAlias(const std::string& name) const
{
   auto ita = amap_.find(name);
   return (ita == amap_.end()) ? nullptr : ita->second;     
}

AliasSymbol* SymbolTable::insertAlias(const std::string& name, const Term& t)
{
   AliasSymbol* symbol = new AliasSymbol(name, t);
   amap_.insert(std::make_pair(name, symbol));
   return symbol;
}

void SymbolTable::clear()
{
   for (auto entry : cmap_) delete entry.second;
   cmap_.clear();

   for (auto entry : vmap_) delete entry.second;
   vmap_.clear();

   for (auto entry : fmap_) delete entry.second;
   fmap_.clear();

   for (auto entry : amap_) delete entry.second;
   amap_.clear();
}

void SymbolTable::pushFunctionCall(FunctionSymbol* f)
{
   call_.push(FunctionCall(f));
}

void SymbolTable::popFunctionCall()
{
   call_.pop();
}

void SymbolTable::addFunctionArgument(Term t)
{
   call_.top().addTerm(t);
}

bool SymbolTable::hasFunctionCall() const
{
   return !call_.empty();
}

std::pair<bool, Term> SymbolTable::processFunCall()
{
   FunctionCall* fc = &call_.top();
   FunctionSymbol* fs = fc->getFunctionSymbol();

   if (fs->arity() != fc->nbTerms())
      return std::make_pair(false, Term(0));

   FunctionCallProcessor vis(fc);
   fs->getTerm().acceptVisitor(vis);

   popFunctionCall();

   return std::make_pair(true, vis.getTerm());
}

///////////////////////////////////////////////////////////////////////////////

FunctionCallProcessor::FunctionCallProcessor(FunctionCall* fc)
      : TermVisitor(),
        fc_(fc),
        t_(Term::SharedRep(nullptr))
{}

void FunctionCallProcessor::apply(const TermConst* t)
{
   t_ = Term(t->getVal());
}

void FunctionCallProcessor::apply(const TermVar* t)
{
   size_t i = fc_->getFunctionSymbol()->getIndexVar(t->var());

   t_ = fc_->getTerm(i);
}

void FunctionCallProcessor::apply(const TermAdd* t)
{
   FunctionCallProcessor vl(fc_);
   t->left()->acceptVisitor(vl);

   FunctionCallProcessor vr(fc_);
   t->right()->acceptVisitor(vr);

   t_ = vl.getTerm() + vr.getTerm();
}

void FunctionCallProcessor::apply(const TermSub* t)
{
   FunctionCallProcessor vl(fc_);
   t->left()->acceptVisitor(vl);

   FunctionCallProcessor vr(fc_);
   t->right()->acceptVisitor(vr);

   t_ = vl.getTerm() - vr.getTerm();
}

void FunctionCallProcessor::apply(const TermMul* t)
{
   FunctionCallProcessor vl(fc_);
   t->left()->acceptVisitor(vl);

   FunctionCallProcessor vr(fc_);
   t->right()->acceptVisitor(vr);

   t_ = vl.getTerm() * vr.getTerm();
}

void FunctionCallProcessor::apply(const TermDiv* t)
{
   FunctionCallProcessor vl(fc_);
   t->left()->acceptVisitor(vl);

   FunctionCallProcessor vr(fc_);
   t->right()->acceptVisitor(vr);

   t_ = vl.getTerm() / vr.getTerm();
}

void FunctionCallProcessor::apply(const TermMin* t)
{
   FunctionCallProcessor vl(fc_);
   t->left()->acceptVisitor(vl);

   FunctionCallProcessor vr(fc_);
   t->right()->acceptVisitor(vr);

   t_ = MIN(vl.getTerm(), vr.getTerm());
}

void FunctionCallProcessor::apply(const TermMax* t)
{
   FunctionCallProcessor vl(fc_);
   t->left()->acceptVisitor(vl);

   FunctionCallProcessor vr(fc_);
   t->right()->acceptVisitor(vr);

   t_ = MAX(vl.getTerm(), vr.getTerm());
}

void FunctionCallProcessor::apply(const TermUsb* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = - vis.getTerm();
}

void FunctionCallProcessor::apply(const TermAbs* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = abs(vis.getTerm());
}

void FunctionCallProcessor::apply(const TermSgn* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = sgn(vis.getTerm());
}

void FunctionCallProcessor::apply(const TermSqr* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = sqr(vis.getTerm());
}

void FunctionCallProcessor::apply(const TermSqrt* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = sqrt(vis.getTerm());
}

void FunctionCallProcessor::apply(const TermPow* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = pow(vis.getTerm(), t->exponent());
}

void FunctionCallProcessor::apply(const TermExp* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = exp(vis.getTerm());
}

void FunctionCallProcessor::apply(const TermLog* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = log(vis.getTerm());
}

void FunctionCallProcessor::apply(const TermCos* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = cos(vis.getTerm());
}

void FunctionCallProcessor::apply(const TermSin* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = sin(vis.getTerm());
}

void FunctionCallProcessor::apply(const TermTan* t)
{
   FunctionCallProcessor vis(fc_);
   t->child()->acceptVisitor(vis);

   t_ = tan(vis.getTerm());
}

Term FunctionCallProcessor::getTerm() const
{
   return t_;
}

} // namespace
