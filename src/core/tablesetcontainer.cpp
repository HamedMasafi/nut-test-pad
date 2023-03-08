#include "tablesetcontainer.h"
#include "dataset.h"

#include <QDebug>

namespace Nut {
TableSetContainer::TableSetContainer()
    : d{new TableSetContainerData}
{}

void TableSetContainer::addTableset(const QString &name, DatasetBase *table)
{
    d->tables.insert(name, table);
}

void TableSetContainer::addTableset(const QString &name, Table<Data> *parent, DatasetBase *table)
{
    qDebug() << "t===" << parent;
    d->tables.insert(name, table);
}

int TableSetContainer::save(Database<Type::Data> *database)
{
    auto self = static_cast<Table<Type::Data>*>(this);

    int n{0};
    for (auto i = d->tables.begin(); i != d->tables.end(); i++) {
        n += i.value()->save(database);
    }
    return n;
}

} // namespace Nut
