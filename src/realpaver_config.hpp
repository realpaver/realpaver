// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONFIG_HPP
#define REALPAVER_CONFIG_HPP

#include "realpaver_configure.hpp"

static const char REALPAVER_STRING[]  = PACKAGE_STRING;
static const char REALPAVER_NAME[]    = PACKAGE_NAME;
static const char REALPAVER_VERSION[] = PACKAGE_VERSION;

#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_VERSION
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef VERSION
#undef PACKAGE_BUGREPORT

#endif
