#include "fieldmodel.h"
#include "../table.h"

namespace Nut {

void FieldModelBase::addToParent(const QString &name, Table<TableTypeModel> *parent)
{
//    parent->_fields.insert(name, this);
}

const char *FieldModelBase::name() const
{
    return d->name;
}

int FieldModelBase::maxLen() const
{
    return d->maxLen;
}

int FieldModelBase::len() const
{
    return d->len;
}

bool FieldModelBase::isPrimaryKey() const
{
    return d->isPrimaryKey;
}

int FieldModelBase::autoIncrementStart() const
{
    return d->autoIncrement.first;
}

int FieldModelBase::autoIncrementStep() const
{
    return d->autoIncrement.second;
}

ConditionalExpression operator ==(const FieldModelBase &f, const QVariant &other)
{
    return {};
}

} // namespace Nut
