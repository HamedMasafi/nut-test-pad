#include "field2.h"
#include "table.h"


namespace Nut {
DBNullType DBNull;

FieldBase::FieldBase(TableMain *parent, const char *name) : _parent{parent}, _name{name}
{
    parent->_fields.insert(name, this);
}

void FieldBase::setChanged()
{
    _parent->_changedFields.insert(_name);
}

} // namespace Nut
