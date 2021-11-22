// This file is part of realpaver. License: see COPYING file.

#ifndef REALPAVER_DOUBLE_HPP
#define REALPAVER_DOUBLE_HPP

#include <cfenv>
#include <cmath>
#include <limits>

namespace realpaver {

inline double DBL_INF()
{
   return std::numeric_limits<double>::infinity();
}

inline double DBL_MINUS_INF()
{
   return -std::numeric_limits<double>::infinity();
}

inline double DBL_NAN()
{
   return std::numeric_limits<double>::quiet_NaN();
}

inline double DBL_IS_INF(const double& x)
{
   return (x == DBL_INF()) || (x == DBL_MINUS_INF());
}

inline double DBL_IS_NAN(const double& x)
{
   return x != x;
}

inline void DBL_RND_NEAR()
{
   std::fesetround(FE_TONEAREST);
}

inline double DBL_ABS(const double& x)
{
   return (x<0.0) ? (-x) : x;
}

inline double DBL_ADD(const double& x, const double& y)
{
   return x+y;
}

inline double DBL_SUB(const double& x, const double& y)
{
   return x-y;
}

inline double DBL_MUL(const double& x, const double& y)
{
   return x*y;
}

inline double DBL_DIV(const double& x, const double& y)
{
   return x/y;
}

inline double DBL_MIN(const double& x, const double& y)
{
   return x < y ? x : y;
}

inline double DBL_MAX(const double& x, const double& y)
{
   return x > y ? x : y;
}

inline double DBL_USB(const double& x)
{
   return -x;
}

inline double DBL_SGN(const double& x)
{
   if (x == 0.0)
      return 0.0;

   else if (x < 0.0)
      return -1.0;

   else
      return 1.0;
}

inline double DBL_SQR(const double& x)
{
   return x*x;
}

inline double DBL_SQRT(const double& x)
{
   return std::sqrt(x);
}

inline double DBL_POW(const double& x, int n)
{
   return std::pow(x, n);
}

inline double DBL_LOG(const double& x)
{
   return std::log(x);
}

inline double DBL_EXP(const double& x)
{
   return std::exp(x);
}

inline double DBL_COS(const double& x)
{
   return std::cos(x);
}

inline double DBL_SIN(const double& x)
{
   return std::sin(x);
}

inline double DBL_TAN(const double& x)
{
   return std::tan(x);
}

} // namespace

#endif
