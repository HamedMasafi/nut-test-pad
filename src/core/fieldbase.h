#ifndef FIELDBASE_H
#define FIELDBASE_H

#include "global.h"

namespace Nut {

template<typename T, Type _Type, typename... Types>
struct FieldB
{
public:
    FieldB(Table<_Type> *, const char *, Types &&...) {}
};

} // namespace Nut

#endif // FIELDBASE_H
