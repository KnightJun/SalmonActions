#pragma once

#include <qglobal.h>

#ifndef BUILD_STATIC
# if defined(SalmonActions_LIB)
#  define SALMONACTIONS_EXPORT Q_DECL_EXPORT
# else
#  define SALMONACTIONS_EXPORT Q_DECL_IMPORT
# endif
#else
# define SALMONACTIONS_EXPORT
#endif
