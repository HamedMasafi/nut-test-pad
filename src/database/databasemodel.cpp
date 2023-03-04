#include "databasemodel.h"
#include "abstracttable.h"

#include <QJsonObject>

#include <type_traits>

namespace Nut {

QJsonObject Nut::Database<Type::Model>::jsonModel() const
{
    QJsonObject model;

    QJsonObject tablesObject;
    for (auto i = _tables.begin(); i != _tables.end(); ++i) {
        tablesObject.insert((*i)->name(), (*i)->toJson());
    }

    model.insert("tables", tablesObject);
    return model;
}

Database<Type::Model> Nut::Database<Type::Model>::operator|(const Database<Type::Model> &other)
{
    Database<Type::Model> ret;
    ret._tables.append(_tables);
    ret._tables.append(other._tables);
    return ret;
}

AbstractTableModel *Nut::Database<Type::Model>::tableByName(const QString &name) const
{
    auto i = std::find_if(_tables.begin(), _tables.end(), [&name](AbstractTableModel *t){
        return t->className() == name;
    });

    if (i == _tables.end())
        return nullptr;

    return *i;
}

QList<AbstractTableModel *> Database<Type::Model>::tables() const
{
    return _tables;
}

AbstractTableModel *Nut::Database<Type::Model>::tableByTableName(const QString &tableName) const
{
    auto i = std::find_if(_tables.begin(), _tables.end(), [&tableName](AbstractTableModel *t){
        return t->name() == tableName;
    });

    if (i == _tables.end())
        return nullptr;

    return *i;
}

Database<Model> &Nut::Database<Type::Model>::model() const
{
    return *const_cast<DatabaseModel*>(this); // I know! but it will never be called
}

Database<Model> Nut::Database<Type::Model>::fromJsonModel(const QJsonObject &json)
{
    Database<Model> ret;
    auto tablesObject = json.value("tables").toObject();

    for (auto v: tablesObject) {
        auto t = new MockTableModel;

        t->fromJson(v.toObject());
        ret._tables.append(t);
    }
    return ret;
}

QString Nut::Database<Type::Model>::className()
{
    return {};
}

} // namespace Nut
