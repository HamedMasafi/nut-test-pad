#pragma once

#include <QList>

#include "global.h"

namespace Nut {

class AbstractTableModel;

template<>
class Database<Type::Model>
{
    QList<AbstractTableModel *> _tables;

public:
    QJsonObject jsonModel() const;
    static Database<Type::Model> fromJsonModel(const QJsonObject &json);

    friend class AbstractTableModel;
    Database<Type::Model> operator|(const Database<Type::Model> &other);

    AbstractTableModel *tableByName(const QString &name) const;
    AbstractTableModel *tableByTableName(const QString &tableName) const;
    QList<AbstractTableModel *> tables() const;

    virtual QString className();

protected:
    virtual Database<Type::Model> &model() const;
};

} // namespace Nut
