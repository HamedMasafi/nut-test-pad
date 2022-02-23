#include "foreignkeymodel.h"
#include "../table.h"

namespace Nut {

ForeignKeyModelBase::ForeignKeyModelBase(Table<TableTypeModel> *parent, const char *name)
{
    parent->_foreignKeys.insert(name, this);
}

} // namespace Nut
