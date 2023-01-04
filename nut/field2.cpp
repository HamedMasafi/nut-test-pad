#include "field2.h"
#include "table.h"


namespace Nut {
DBNullType DBNull;

FieldBase::FieldBase(TableRow *parent, const char *name) : _parent{parent}, _name{name}
{
    parent->_fields.insert(name, this);
}

void FieldBase::setChanged()
{
    _parent->_changedFields.insert(_name);
    _parent->_status = Nut::RowStatus::Modified;
}

} // namespace Nut
