#include "fieldmodel.h"
#include "../table.h"

namespace Nut {

void FieldModelBase::addToParent(const QString &name, Table<Type::Model> *parent)
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

bool FieldModelBase::notNull() const
{
    return d->notNull;
}

bool FieldModelBase::isUnique() const
{
    //TODO: implement me
    return false;
}

ConditionalExpression operator ==(const FieldModelBase &f, const QVariant &other)
{
    return {};
}

bool operator==(const FieldModelBase &l, const FieldModelBase &r)
{
    return l.name() == r.name();
}
bool operator!=(const FieldModelBase &l, const FieldModelBase &r)
{
    return !(l == r);
}

} // namespace Nut
