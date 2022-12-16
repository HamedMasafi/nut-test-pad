#ifndef TABLEBASE_H
#define TABLEBASE_H

#include "../global.h"

#include "../runtimecheckers/tablechecker.h"

#include <QMap>
#include <QSet>
#include <QString>
#include <QVariant>

namespace Nut {
class FieldBase;
class ForeignKeyModelBase;
class FieldCheckerBase;
class AbstractFieldPhrase;

template<TableType _Type>
class Table
{
public:
    Table() = default;

    friend class FieldBase;
};

using TableClassBase = Table<TableTypeMain>;
using ModelClassBase = Table<TableTypeModel>;

template<>
class Table<TableTypeMain>
{
    QString keyField;
    QMap<QString, FieldBase*> _fields;
    QSet<QString> _changedFields;
public:

    Table() = default;

    friend class FieldBase;

    void setFieldValue(const QString &name, const QVariant &value);
    QVariant fieldvalue(const QString &name) const;

    QVariant key() const;
    void setKey(const QVariant &value);
    const QSet<QString> &changedFields() const;
};

template <>
class Table<TableTypeModel>
{
    QMap<QString, AbstractFieldPhrase*> _fields;
    QMap<QString, ForeignKeyModelBase*> _foreignKeys;
public:
    Table() = default;

    QJsonObject toJson() const;

    const QMap<QString, AbstractFieldPhrase *> &fields() const;

    friend class AbstractFieldPhrase;
    friend class ForeignKeyModelBase;
};
}
#endif // TABLEBASE_H
