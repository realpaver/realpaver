// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_OBJ_HPP
#define REALPAVER_OBJ_HPP

#include "realpaver_term.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of objective functions.
 *****************************************************************************/
class Obj {
public:
   // optimization direction
   enum Dir {Min, Max};

   // constructor
   Obj(const Dir& d, const Term& f);

   // getters
   Dir getDir() const;
   Term getTerm() const;

   // returns true if the function is constant
   bool isConstant() const;

   // returns true if the function is linear
   bool isLinear() const;

private:
   Dir d_;     // direction
   Term f_;    // function
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const Obj& obj);

// makers
Obj minimize(const Term& f);
Obj maximize(const Term& f);

inline Obj::Dir Obj::getDir() const
{
   return d_;
}

inline Term Obj::getTerm() const
{
   return f_;
}

inline bool Obj::isConstant() const
{
   return f_.isConstant();
}

inline bool Obj::isLinear() const
{
   return f_.isLinear();
}

} // namespace

#endif
