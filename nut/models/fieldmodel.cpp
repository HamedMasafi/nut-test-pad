#include "fieldmodel.h"
#include "../table.h"

namespace Nut {

void FieldModelBase::addToParent(const QString &name, Table<TableTypeModel> *parent)
{
    parent->_fields.insert(name, this);
}

} // namespace Nut
