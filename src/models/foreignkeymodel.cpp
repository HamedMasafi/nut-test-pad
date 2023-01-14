#include "foreignkeymodel.h"
#include "../table.h"

namespace Nut {

Table<Type::Model> *ForeignKeyModelBase::localModel() const
{
    return _localModel;
}

Table<Type::Model> *ForeignKeyModelBase::remoteModel() const
{
    return _remoteModel;
}

ForeignKeyModelBase::ForeignKeyModelBase(Table<Type::Model> *parent,
                                         Table<Model> *remoteModel,
                                         const char *name)
    : _localModel{parent}
    , _remoteModel{remoteModel}
    , _name{name}
{
    parent->_foreignKeys.insert(name, this);
}

QString ForeignKeyModelBase::name() const
{
    return _name;
}

auto ForeignKeyModelBase::remoteTablePrimaryField() const -> AbstractFieldPhrase *
{
    return _remoteModel->primaryField();
}


QString ForeignKeyModelBase::remoteTableName() const
{
    return _remoteModel->name();
}

QString ForeignKeyModelBase::localTableName() const
{
    return _localModel->name();
}



} // namespace Nut
