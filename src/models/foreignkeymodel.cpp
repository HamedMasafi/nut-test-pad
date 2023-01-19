#include "foreignkeymodel.h"
#include "../table.h"
#include "database.h"

namespace Nut {

Table<Type::Model> *ForeignKeyModelBase::localModel() const
{
    return _localModel;
}

AbstractTableModel *ForeignKeyModelBase::remoteModel()
{
    if (!_remoteModel)
        _remoteModel = _localModel->_parentDatabase->tableByName(_remoteClassName);
    return _remoteModel;
}

ForeignKeyModelBase::ForeignKeyModelBase(Table<Type::Model> *parent,
                                         Table<Model> *remoteModel,
                                         const char *name)
    : _localModel{parent}
//    , _remoteModel{remoteModel}
    , _name{name}
{
    parent->_foreignKeys.insert(name, this);
//    _remoteModel = _localModel->_parentDatabase->tableByName(_remoteClassName);
}

QString ForeignKeyModelBase::name() const
{
    return _name;
}

auto ForeignKeyModelBase::remoteTablePrimaryField() const -> AbstractFieldPhrase *
{
    auto r = _localModel->_parentDatabase->tableByName(_remoteClassName);
    return r->primaryField();
}


QString ForeignKeyModelBase::remoteTableName() const
{
    auto r = _localModel->_parentDatabase->tableByName(_remoteClassName);
    return r->name();
}

QString ForeignKeyModelBase::localTableName() const
{
    return _localModel->name();
}



} // namespace Nut
