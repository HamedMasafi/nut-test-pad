#include "databasemodel.h"
#include "abstracttable.h"

#include <QJsonObject>

namespace Nut {

QJsonObject Nut::Database<Type::Model>::jsonModel() const
{
    QJsonObject model;
    for (auto i = _tables.begin(); i != _tables.end(); ++i) {
        //        auto o = i.value()->createModel();
        model.insert((*i)->name(), (*i)->toJson());
    }
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
    for (const auto &t : _tables)
        if (t->className() == name)
            return t;
    return nullptr;
}

QList<AbstractTableModel *> Database<Type::Model>::tables() const
{
    return _tables;
}

AbstractTableModel *Nut::Database<Type::Model>::tableByTableName(const QString &tableName) const
{
    for (const auto &t : _tables)
        if (t->name() == tableName)
            return t;
    return nullptr;
}

} // namespace Nut
