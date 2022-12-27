#include "database.h"
#include "dataset.h"
#include "table.h"
#include "models/modelstorage.h"

#include <QDebug>
#include <QJsonObject>

namespace Nut {

Database3::Database3()
{
    QString id = typeid(this).name();
    qDebug() << "model exists" << (ModelStorage::models.contains(id)) << id;

    if (!ModelStorage::models.contains(id))
        ModelStorage::models.insert(id, "a");
}

QList<TableModel *> Database3::model() const
{
    QList<TableModel *>  ret;
    for (const auto &table: _tables)
        ret.append(table->createModel());

    return ret;
}

TableMain *Database3::createTable(const QString &name) const
{
    if (!_tables.contains(name))
        return nullptr;
    return _tables.value(name)->createTable();
}

QJsonObject Database3::jsonModel() const
{
    QJsonObject model;
    for (auto i = _tables.begin(); i != _tables.end(); ++i) {
        auto o = i.value()->createModel();
        model.insert(i.key(), o->toJson());
    }
    return model;
}

QJsonObject Nut::Database<TableTypeModel>::jsonModel() const
{
    QJsonObject model;
    for (auto i = _tables.begin(); i != _tables.end(); ++i) {
//        auto o = i.value()->createModel();
        model.insert((*i)->name(), (*i)->toJson());
    }
    return model;
}

Database<TableTypeModel> Nut::Database<TableTypeModel>::operator|(const Database<TableTypeModel> &other)
{
    Database<TableTypeModel> ret;
    ret._tables.append(_tables);
    ret._tables.append(other._tables);
    return ret;
}

AbstractModel *Nut::Database<TableTypeModel>::tableByName(const QString &name) const
{
    for (const auto &t: _tables)
        if (t->name() == name)
            return t;
    return nullptr;
}

QList<AbstractModel *> Database<TableTypeModel>::tables() const
{
    return _tables;
}

} // namespace Nut
