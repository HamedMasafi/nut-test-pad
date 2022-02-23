#ifndef FIELD_H
#define FIELD_H

#include "../core/fieldbase.h"
#include "../namedtype.h"

namespace Nut {

template<typename T, typename... Types>
struct FieldB<T, TableTypeMain, Types...>
{
    constexpr FieldB(TableMain *, const char *name, Types... args)
    {
        using namespace Model;

    }
};

} // namespace Nut

#endif // FIELD_H
