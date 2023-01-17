#include "database.h"
#include "dataset.h"

namespace Nut {

DatasetBase::DatasetBase(Database<Type::Data> *parent, const char *name, const QString &className)
{
    //    parent->_tables.insert(name, this);
}

DatasetBase::DatasetBase(Database<Data> *parentDatabase, Database<Model> *parentDatabaseModel, const char *name, const QString &className)
{
    _model = parentDatabaseModel->tableByName(className);
}


} // namespace Nut
