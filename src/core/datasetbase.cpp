#include "datasetbase.h"
#include "database.h"

namespace Nut {

DatasetBase::DatasetBase(Database<Type::Data> *parent, const char *name, const QString &className)
{
    //    parent->_tables.insert(name, this);
}

DatasetBase::DatasetBase(Table<Data> *parent,
                         Table<Type::Model> *model,
                         const char *name,
                         const QString &className)
{

}

DatasetBase::DatasetBase(Database<Data> *parentDatabase, Database<Model> *parentDatabaseModel, const char *name, const QString &className)
{
    _model = parentDatabaseModel->tableByName(className);
}


} // namespace Nut
