#include "fieldchecker.h"
#include "../table.h"

namespace Nut {

void FieldCheckerBase::addToParent(const QString &name, Table<RuntimeChecker> *parent)
{
    Q_UNUSED(name)
    auto link = new Table<RuntimeChecker>::Link;
    link->data = this;

    if (!parent->first)
        parent->first = link;

    parent->last = link;
}

} // namespace Nut
