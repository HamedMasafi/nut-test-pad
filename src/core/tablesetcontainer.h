#pragma once

#include <QMap>
#include <QSharedData>

#include "global.h"

namespace Nut {

template <Type _Type>
class Database;

class DatasetBase;
class TableSetContainer;
class TableSetContainerData : public QSharedData
{
public:
    QMap<QString, DatasetBase *> tables;
    QMap<QString, TableSetContainer *> childs;
};

class TableSetContainer
{
    QSharedDataPointer<TableSetContainerData> d;

public:
    TableSetContainer();

    void addTableset(const QString &name, DatasetBase *table);
    void addTableset(const QString &name, Table<Type::Data> *parent, DatasetBase *table);
    int save(Database<Type::Data> *database);
};

} // namespace Nut
