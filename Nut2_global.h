#pragma once

#include <QtCore/qglobal.h>

#if defined(NUT2_LIBRARY)
#  define NUT2_EXPORT Q_DECL_EXPORT
#else
#  define NUT2_EXPORT Q_DECL_IMPORT
#endif
