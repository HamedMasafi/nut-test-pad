#include "foreignkeymodel.h"
#include "../table.h"

namespace Nut {

QString ForeignKeyModelBase::name() const
{
    return _name;
}

ForeignKeyModelBase::ForeignKeyModelBase(Table<Type::Model> *parent, const char *name) : _name{name}
{
    parent->_foreignKeys.insert(name, this);
}

} // namespace Nut
