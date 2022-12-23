
#include <QJsonObject>
#include <QJsonValue>

#include "table.h"
#include "phrases/abstractfieldphrase.h"

namespace Nut {

Table<TableTypeModel>::Table(Database<TableTypeModel> *parent, const char *name) : _name(name)
{
    qDebug() << "my name is" << name;
}

AbstractFieldPhrase *Nut::Table<TableTypeModel>::feild(const QString &name) const
{
    for (auto i = _fields.begin(); i != _fields.end(); ++i)
        if ((*i)->name() == name)
            return *i;
    return nullptr;
}

void Table<TableTypeMain>::setFieldValue(const QString &name, const QVariant &value)
{
    if (_fields.contains(name))
        _fields.value(name)->fromVariant(value);
}

QVariant Table<TableTypeMain>::fieldvalue(const QString &name) const
{
    if (!_fields.contains(name))
        return QVariant();
    return _fields.value(name)->toVariant();
}

QJsonObject Table<TableTypeModel>::toJson() const
{
    QJsonObject fieldsObject;
    for (auto i = _fields.begin(); i != _fields.end(); ++i) {
        QJsonObject fieldObject;
        fieldObject.insert("isKey", (*i)->isPrimaryKey());
        fieldObject.insert("autoIncrement",
                           QStringLiteral("%1,%2")
                               .arg((*i)->autoIncrementStart())
                               .arg((*i)->autoIncrementStep()));
        fieldObject.insert("len", (*i)->len());
        fieldObject.insert("maxlen", (*i)->maxLen());
        fieldObject.insert("columnName", (*i)->name());

        fieldsObject.insert(i.key(), fieldObject);
    }
    QJsonObject foreignKeysObject;
    for (auto i = _foreignKeys.begin(); i != _foreignKeys.end(); ++i) {
        QJsonObject foreignKeyObject;

//        foreignKeyObject.insert("name", (*i)->);
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

//const QMap<QString, AbstractFieldPhrase *> &Table<TableTypeModel>::fields() const
//{
//    return _fields;
//}

const QSet<QString> &Table<TableTypeMain>::changedFields() const
{
    return _changedFields;
}

} // namespace Nut
