#include "table.h"

namespace Nut {

void Table<TableTypeMain>::setFieldValue(const QString &name, const QVariant &value)
{
    _fields.value(name)->fromVariant(value);
}

QVariant Table<TableTypeMain>::fieldvalue(const QString &name) const
{
    return _fields.value(name)->toVariant();
}

QJsonObject Table<TableTypeModel>::toJson() const
{
    QJsonObject fieldsObject;
    for (auto i = _fields.begin(); i != _fields.end(); ++i) {
        QJsonObject fieldObject;
        fieldObject.insert("isKey", (*i)->_isPrimaryKey);
        fieldObject.insert("autoIncrement",
                           QStringLiteral("%1,%2")
                               .arg((*i)->_autoIncrement.first)
                               .arg((*i)->_autoIncrement.second));
        fieldObject.insert("len", (*i)->_len);
        fieldObject.insert("maxlen", (*i)->_maxLen);
        fieldObject.insert("columnName", (*i)->_name);

        fieldsObject.insert(i.key(), fieldObject);
    }
    QJsonObject foreignKeysObject;
    for (auto i = _foreignKeys.begin(); i != _foreignKeys.end(); ++i) {
        QJsonObject foreignKeyObject;


        foreignKeyObject.insert(i.key(), foreignKeyObject);
    }

    QJsonObject o;
    o.insert("fields", fieldsObject);
    o.insert("foreignKeys", foreignKeysObject);
    return o;
}

QVariant Table<TableTypeMain>::key() const
{
    return _fields.value(keyField)->toVariant();
}

void Table<TableTypeMain>::setKey(const QVariant &value)
{
    _fields.value(keyField)->fromVariant(value);
}

const QMap<QString, FieldModelBase *> &Table<TableTypeModel>::fields() const
{
    return _fields;
}

const QSet<QString> &Table<TableTypeMain>::changedFields() const
{
    return _changedFields;
}

} // namespace Nut
