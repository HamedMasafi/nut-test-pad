#include "database.h"
#include "dataset.h"
#include "table.h"
#include "models/modelstorage.h"

#include <QDebug>
#include <QJsonObject>

namespace Nut {

Database::Database()
{
    QString id = typeid(this).name();
    qDebug() << "model exists" << (ModelStorage::models.contains(id)) << id;

    if (!ModelStorage::models.contains(id))
        ModelStorage::models.insert(id, "a");
}

QList<TableModel *> Database::model() const
{
    QList<TableModel *>  ret;
    for (const auto &table: _tables)
        ret.append(table->createModel());

    return ret;
}

TableMain *Database::createTable(const QString &name) const
{
    if (!_tables.contains(name))
        return nullptr;
    return _tables.value(name)->createTable();
}

QJsonObject Database::jsonModel() const
{
    QJsonObject model;
    for (auto i = _tables.begin(); i != _tables.end(); ++i) {
        auto o = i.value()->createModel();
        model.insert(i.key(), o->toJson());
    }
    return model;
}

QJsonObject Nut::Database2<TableTypeModel>::jsonModel() const
{
    QJsonObject model;
    for (auto i = _tables.begin(); i != _tables.end(); ++i) {
//        auto o = i.value()->createModel();
        model.insert((*i)->name(), (*i)->toJson());
    }
    return model;
}

} // namespace Nut
