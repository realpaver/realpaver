// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_obj.hpp"

namespace realpaver {

Obj::Obj(const Dir& d, const Term& f): d_(d), f_(f)
{}

Obj minimize(const Term& f)
{
   return Obj(Obj::Dir::Min, f);
}

Obj maximize(const Term& f)
{
   return Obj(Obj::Dir::Max, f);
}

std::ostream& operator<<(std::ostream& os, const Obj& obj)
{
   if (obj.getDir() == Obj::Dir::Max)
      os << "maximize ";
   else
      os << "minimize ";

   return os << obj.getTerm();
}

} // namespace
