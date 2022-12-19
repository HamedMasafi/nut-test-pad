#ifndef TABLEBASE_H
#define TABLEBASE_H

#include "../global.h"

#include "../runtimecheckers/tablechecker.h"
#include "abstracttable.h"

#include <QMap>
#include <QSet>
#include <QString>
#include <QVariant>

namespace Nut {
class FieldBase;
class ForeignKeyModelBase;
class FieldCheckerBase;
class AbstractFieldPhrase;
template<Nut::TableType _Type>
class Database2;

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
protected:
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
protected:
    QMap<QString, AbstractFieldPhrase*> _fields;
    QMap<QString, ForeignKeyModelBase*> _foreignKeys;
public:
    Table() = default;
    Table(Database2<TableTypeModel> *parent, const char *name);

    QJsonObject toJson() const;

    friend class DatasetBase;
    friend class AbstractFieldPhrase;
    friend class ForeignKeyModelBase;
};


template<NUT_TABLE_TEMPLATE T>
class ModelBase : public AbstractModel, public T<TableTypeModel>
{
public:
    ModelBase(Nut::Database2<TableTypeModel> *parent, const char *name)
        : AbstractModel(parent, name)
    {}
    const QMap<QString, AbstractFieldPhrase *> &fields() override{
        return T<TableTypeModel>::_fields;
    }
    const QMap<QString, ForeignKeyModelBase*> &foreignKeys() override{
        return T<TableTypeModel>::_foreignKeys;
    }
};

}
#endif // TABLEBASE_H
