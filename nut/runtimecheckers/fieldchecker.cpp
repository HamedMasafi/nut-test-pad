#include "fieldchecker.h"
#include "../table.h"

namespace Nut {

constexpr void addToParent(const char *name, Table<RuntimeChecker> *parent)
{
    Q_UNUSED(name)
    parent->k = true;
}


//constexpr void FieldCheckerBase::addToParent(const char *name, Table<RuntimeChecker> *parent)
//{
//    Q_UNUSED(name)
//}

} // namespace Nut
