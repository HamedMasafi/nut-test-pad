#include "foreignkey.h"
#include "table.h"

namespace Nut {

void ForeignKeyBase::setChanged()
{
    _parent->_changedFields.insert(_name);
}

ForeignKeyBase::ForeignKeyBase(TableRow *parent, const char *name)
    : _parent(parent)
    , _name(name)
{
    parent->_foreignKeys.insert(name, this);
}

}
