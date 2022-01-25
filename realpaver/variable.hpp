// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_VARIABLE_HPP
#define REALPAVER_VARIABLE_HPP

#include <memory>
#include "realpaver/interval.hpp"
#include "realpaver/stopping.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of shared variable representation.
 *****************************************************************************/
class VariableRep {
public:
   // constructor
   VariableRep(const std::string& name);

   // copy protection
   VariableRep(const VariableRep&) = delete;
   VariableRep& operator=(const VariableRep&) = delete;

   // destructor
   ~VariableRep() = default;

   // identifier
   size_t id() const;
   void setId(size_t id);

   // name
   const std::string& name() const;
   void setName(const std::string& name);

   // domain
   const Interval& domain() const;
   void setDomain(const Interval& x);

   // type: continuous or discrete
   bool isDiscrete() const;
   void setDiscrete();
   bool isContinuous() const;
   void setContinuous();

   // hash code
   size_t hashCode() const;

   // management of the precision, i.e. the output precision of
   // interval solutions
   Precision precision() const;
   void setPrecision(const Precision& p);

private:
   std::string name_;   // name
   size_t id_;          // id
   Interval domain_;    // initial variable domain
   bool continuous_;    // true for a real variable, false for a discrete one
   Precision eps_;      // output precision of interval solutions
};

inline size_t VariableRep::id() const
{
   return id_;
}

inline void VariableRep::setId(size_t id)
{
   id_ = id;
}

inline const std::string& VariableRep::name() const
{
   return name_;
}

inline void VariableRep::setName(const std::string& name)
{
   name_ = name;
}

inline const Interval& VariableRep::domain() const
{
   return domain_;
}

inline void VariableRep::setDomain(const Interval& x)
{
   domain_ = x;
}

inline bool VariableRep::isDiscrete() const
{
   return !continuous_;
}

inline void VariableRep::setDiscrete()
{
   continuous_ = false;
}

inline bool VariableRep::isContinuous() const
{
   return continuous_;
}

inline void VariableRep::setContinuous()
{
   continuous_ = true;
}

inline size_t VariableRep::hashCode() const
{
   return id_;
}

inline Precision VariableRep::precision() const
{
   return eps_;
}

inline void VariableRep::setPrecision(const Precision& p)
{
   eps_ = p;
}

/*****************************************************************************
 * Class of variables occurring in functions and constraints.
 *****************************************************************************/
class Variable {
public:
   // constructor
   Variable(const std::string& name);

   // copy
   Variable(const Variable&) = default;
   Variable& operator=(const Variable&) = default;

   // destructor
   ~Variable() = default;

   // identifier
   size_t id() const;
   Variable& setId(size_t id);

   // name
   const std::string& name() const;
   Variable& setName(const std::string& name);

   // domain
   const Interval& domain() const;
   Variable& setDomain(const Interval& x);

   // type: continuous or discrete (bool or int)
   bool isDiscrete() const;
   bool isDiscreteBool() const;
   bool isDiscreteInt() const;
   Variable& setDiscrete();
   bool isContinuous() const;
   Variable& setContinuous();


   // hash code
   size_t hashCode() const;

   // output precision of interval solutions
   Precision precision() const;
   Variable& setPrecision(const Precision& p);

   // clone
   Variable clone() const;

   // equality test
   bool operator==(const Variable& other) const;

private:
   std::shared_ptr<VariableRep> rep_;
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const Variable& v);

inline size_t Variable::id() const
{
   return rep_->id();
}

inline Variable& Variable::setId(size_t id)
{
   rep_->setId(id);
   return *this;
}

inline const std::string& Variable::name() const
{
   return rep_->name();
}

inline Variable& Variable::setName(const std::string& name)
{
   rep_->setName(name);
   return *this;
}

inline const Interval& Variable::domain() const
{
   return rep_->domain();
}

inline Variable& Variable::setDomain(const Interval& x)
{
   rep_->setDomain(x);
   return *this;
}

inline bool Variable::isDiscrete() const
{
   return rep_->isDiscrete();
}

inline bool Variable::isDiscreteBool() const
{
   return isDiscrete() && domain().isSetEq(Interval::zeroPlusOne());
}

inline bool Variable::isDiscreteInt() const
{
   return isDiscrete() && domain().isSetNeq(Interval::zeroPlusOne());
}

inline Variable& Variable::setDiscrete()
{
   rep_->setDiscrete();
   return *this;
}

inline bool Variable::isContinuous() const
{
   return rep_->isContinuous();
}

inline Variable& Variable::setContinuous()
{
   rep_->setContinuous();
   return *this;
}

inline size_t Variable::hashCode() const
{
   return rep_->hashCode();
}

inline Precision Variable::precision() const
{
   return rep_->precision();
}

inline Variable& Variable::setPrecision(const Precision& p)
{
   rep_->setPrecision(p);
   return *this;
}

inline bool Variable::operator==(const Variable& other) const
{
   return id() == other.id();
}

/*****************************************************************************
 * Class enclosing an hash function for variables.
 *****************************************************************************/
struct VariableHasher
{
   std::size_t operator()(const Variable& v) const
   {
      return v.hashCode();
   }
};

} // namespace

#endif
